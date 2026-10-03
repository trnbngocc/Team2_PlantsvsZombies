#include "game_controller.hpp"
#include "bullet.hpp"
#include "peashooter.hpp"
#include "sunflower.hpp"
#include "wallnut.hpp"
#include "chili_pepper.hpp"


#include <algorithm>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

GameController::GameController()
    : wave_manager(this) {
}

void GameController::_bind_methods() {
    ClassDB::bind_method(
        D_METHOD("start_game"),
        &GameController::start_game
    );

    ClassDB::bind_method(
        D_METHOD("pause_game"),
        &GameController::pause_game
    );

    ClassDB::bind_method(
        D_METHOD("resume_game"),
        &GameController::resume_game
    );

    ClassDB::bind_method(
        D_METHOD("finish_game", "won"),
        &GameController::finish_game
    );

    ClassDB::bind_method(
        D_METHOD("get_state"),
        &GameController::get_state
    );

    ClassDB::bind_method(
        D_METHOD("get_sun"),
        &GameController::get_sun
    );
        ClassDB::bind_method(
        D_METHOD("place_plant", "row", "col", "type"),
        &GameController::place_plant_by_type
    );

    ClassDB::bind_method(
        D_METHOD("collect_sun", "id"),
        &GameController::collect_sun
    );

    ClassDB::bind_method(
        D_METHOD("get_sun_drops"),
        &GameController::get_sun_drops
    );

    ClassDB::bind_method(
        D_METHOD("get_zombies_data"),
        &GameController::get_zombies_data
    );

    ClassDB::bind_method(
        D_METHOD("get_bullets_data"),
        &GameController::get_bullets_data
    );

    ClassDB::bind_method(
        D_METHOD("get_plants_data"),
        &GameController::get_plants_data
    );

}

void GameController::_ready() {
    UtilityFunctions::print(
        "PVZ core GameController is ready."
    );

    // Start the first zombie wave when the scene enters the game.
    start_game();

    // Make sure Godot calls _process(delta).
    set_process(true);
}

void GameController::_process(double delta) {
    if (state == GameState::PLAYING) {
        update_entities(delta);
    }
}

void GameController::start_game() {
    state = GameState::PLAYING;

    // Start the first zombie wave.
    wave_manager.start_wave(1);
}

void GameController::pause_game() {
    if (state == GameState::PLAYING) {
        state = GameState::PAUSED;
    }
}

void GameController::resume_game() {
    if (state == GameState::PAUSED) {
        state = GameState::PLAYING;
    }
}

void GameController::finish_game(bool won) {
    state = won
        ? GameState::VICTORY
        : GameState::DEFEAT;
}

void GameController::add_entity(
    std::unique_ptr<pvz::Entity> entity
) {
    if (entity == nullptr) {
        return;
    }

    entities.push_back(std::move(entity));
}

void GameController::spawn_bullet(
    int row,
    float start_column,
    int damage,
    float speed_cells_per_second
) {
    add_entity(
        std::make_unique<pvz::Bullet>(
            row,
            start_column,
            damage,
            speed_cells_per_second
        )
    );
}

void GameController::update_entities(double delta_seconds) {
    // ==================== SUN ====================

    sun_economy.update(delta_seconds);

    // ==================== ZOMBIE WAVES ====================

    wave_manager.update(delta_seconds);

    // ==================== ENTITY UPDATE ====================

    for (const auto& entity : entities) {
        if (entity != nullptr && entity->is_alive()) {
            entity->tick_status_effects(delta_seconds);

            if (entity->is_alive()) {
                entity->update(delta_seconds);
            }
        }
    }

    // ==================== PLANT ACTIONS ====================

    for (const auto& entity : entities) {
        if (entity != nullptr &&
            entity->is_alive() &&
            entity->get_type() == pvz::EntityType::PLANT) {

            static_cast<pvz::Plant*>(entity.get())->act(*this);
        }
    }

    // ==================== BULLET COLLISIONS ====================

    resolve_bullet_collisions();

    // ==================== CLEANUP ====================

    cleanup_dead_entities();
}

int GameController::get_state() const {
    return static_cast<int>(state);
}

bool GameController::place_plant(
    pvz::GridPosition pos,
    std::unique_ptr<pvz::Plant> plant
) {
    if (plant == nullptr) {
        return false;
    }

    if (!grid.is_valid(pos) || grid.is_occupied(pos)) {
        return false;
    }

    if (sun < plant->get_cost()) {
        return false;
    }

    if (!grid.place_plant(pos, plant.get())) {
        return false;
    }

    sun -= plant->get_cost();

    entities.push_back(std::move(plant));

    return true;
}
bool GameController::place_plant_by_type(int row, int col, int plant_type) {
    pvz::GridPosition pos{row, col};
    std::unique_ptr<pvz::Plant> plant;

    switch (plant_type) {
        case PlantType::PEASHOOTER:
            plant = std::make_unique<pvz::Peashooter>(pos);
            break;
        case PlantType::SUNFLOWER:
            plant = std::make_unique<pvz::Sunflower>(pos);
            break;
        case PlantType::WALLNUT:
            plant = std::make_unique<pvz::WallNut>(pos);
            break;
        case PlantType::CHILI_PEPPER:
            plant = std::make_unique<pvz::ChiliPepper>(pos);
            break;
        default:
            return false;
    }

    return place_plant(pos, std::move(plant));
}


void GameController::add_zombie(
    std::unique_ptr<pvz::Zombie> zombie
) {
    if (zombie == nullptr) {
        return;
    }

    // Zombie needs the Grid to detect Plants and attack them.
    zombie->set_grid(&grid);

    entities.push_back(std::move(zombie));

    UtilityFunctions::print(
        "GameController: Zombie added. Total entities = ",
        static_cast<int>(entities.size())
    );
}
bool GameController::has_zombie_in_row(int row) const {
    for (const auto& e : entities) {
        if (e->get_type() == pvz::EntityType::ZOMBIE &&
            e->is_alive() &&
            e->get_position().row == row) {

            return true;
        }
    }

    return false;
}

std::vector<pvz::Zombie*> GameController::get_zombies_in_row(
    int row
) const {
    std::vector<pvz::Zombie*> result;

    for (const auto& e : entities) {
        if (e->get_type() == pvz::EntityType::ZOMBIE &&
            e->is_alive() &&
            e->get_position().row == row) {

            result.push_back(
                static_cast<pvz::Zombie*>(e.get())
            );
        }
    }

    return result;
}

void GameController::add_sun(int amount) {
    sun += amount;
}

int GameController::get_sun() const {
    return sun;
}

int GameController::spawn_sun(
    pvz::GridPosition pos,
    int value
) {
    return sun_economy.spawn_sun(pos, value);
}

int GameController::collect_sun(int id) {
    const int value = sun_economy.collect_sun(id);

    if (value > 0) {
        sun += value;
    }

    return value;
}

const std::vector<pvz::SunDrop>&
GameController::get_suns() const {
    return sun_economy.get_suns();
}

void GameController::resolve_bullet_collisions() {
    constexpr float CELL_HIT_RADIUS = 0.5f;

    for (const auto& be : entities) {
        if (be->get_type() != pvz::EntityType::BULLET ||
            !be->is_alive()) {

            continue;
        }

        pvz::Bullet* bullet =
            static_cast<pvz::Bullet*>(be.get());

        for (const auto& ze : entities) {
            if (ze->get_type() != pvz::EntityType::ZOMBIE ||
                !ze->is_alive()) {

                continue;
            }

            if (ze->get_position().row !=
                bullet->get_position().row) {

                continue;
            }

            float dx =
                static_cast<float>(
                    ze->get_position().column
                )
                - bullet->get_column();

            if (dx < 0) {
                dx = -dx;
            }

            if (dx <= CELL_HIT_RADIUS) {
                ze->take_damage(
                    bullet->get_damage()
                );

                bullet->take_damage(
                    bullet->get_health()
                );

                break;
            }
        }
    }
}

void GameController::cleanup_dead_entities() {
    // Remove dead plants from Grid first.
    for (const auto& e : entities) {
        if (!e->is_alive() &&
            e->get_type() == pvz::EntityType::PLANT) {

            grid.remove_plant(
                e->get_position()
            );
        }
    }

    // Remove all dead entities from ownership vector.
    entities.erase(
        std::remove_if(
            entities.begin(),
            entities.end(),
            [](const auto& e) {
                return !e->is_alive();
            }
        ),
        entities.end()
    );
}
Array GameController::get_sun_drops() const {
    Array result;
    for (const auto& s : sun_economy.get_suns()) {
        Dictionary d;
        d["id"] = s.id;
        d["value"] = s.value;
        d["row"] = s.row;
        d["column"] = s.column;
        d["time_left"] = s.time_left;
        result.push_back(d);
    }
    return result;
}

Array GameController::get_zombies_data() const {
    Array result;
    for (const auto& e : entities) {
        if (e != nullptr && e->is_alive() && e->get_type() == pvz::EntityType::ZOMBIE) {
            auto* z = static_cast<pvz::Zombie*>(e.get());
            Dictionary d;
            d["row"] = z->get_position().row;
            d["column"] = static_cast<float>(z->get_position().column);
            d["health"] = z->get_health();
            d["visual_state"] = static_cast<int>(z->get_visual_state());
            d["armor"] = static_cast<int>(z->get_armor_type());
            d["armor_health"] = z->get_armor_health();
            result.push_back(d);
        }
    }
    return result;
}

Array GameController::get_bullets_data() const {
    Array result;
    for (const auto& e : entities) {
        if (e != nullptr && e->is_alive() && e->get_type() == pvz::EntityType::BULLET) {
            auto* b = static_cast<pvz::Bullet*>(e.get());
            Dictionary d;
            d["row"] = b->get_position().row;
            d["column"] = b->get_column();
            result.push_back(d);
        }
    }
    return result;
}

Array GameController::get_plants_data() const {
    Array result;
    for (const auto& e : entities) {
        if (e != nullptr && e->is_alive() && e->get_type() == pvz::EntityType::PLANT) {
            auto* p = static_cast<pvz::Plant*>(e.get());
            Dictionary d;
            d["row"] = p->get_position().row;
            d["column"] = p->get_position().column;
            d["health"] = p->get_health();
            d["cost"] = p->get_cost();
            result.push_back(d);
        }
    }
    return result;
}


} // namespace godot