#include "game_controller.hpp"
#include "bullet.hpp"

#include <algorithm> // MỚI — cho std::remove_if
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void GameController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("start_game"), &GameController::start_game);
    ClassDB::bind_method(D_METHOD("pause_game"), &GameController::pause_game);
    ClassDB::bind_method(D_METHOD("resume_game"), &GameController::resume_game);
    ClassDB::bind_method(
        D_METHOD("finish_game", "won"),
        &GameController::finish_game
    );
    ClassDB::bind_method(D_METHOD("get_state"), &GameController::get_state);
    ClassDB::bind_method(D_METHOD("get_sun"), &GameController::get_sun); // MỚI
}

void GameController::_ready() {
    UtilityFunctions::print("PVZ core GameController is ready.");
}

void GameController::start_game() {
    state = GameState::PLAYING;
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
    state = won ? GameState::VICTORY : GameState::DEFEAT;
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
    add_entity(std::make_unique<pvz::Bullet>(
        row,
        start_column,
        damage,
        speed_cells_per_second
    ));
}

void GameController::update_entities(double delta_seconds) {
    // Bước 1: cập nhật mọi entity còn sống (giữ nguyên như code gốc)
    for (const auto& entity : entities) {
        if (entity != nullptr && entity->is_alive()) {
            entity->update(delta_seconds);
        }
    }

    // Bước 2 (MỚI): cho mọi Plant còn sống cơ hội hành động (bắn/sinh Sun/nổ...)
    for (const auto& entity : entities) {
        if (entity != nullptr && entity->is_alive() &&
            entity->get_type() == pvz::EntityType::PLANT) {
            static_cast<pvz::Plant*>(entity.get())->act(*this);
        }
    }

    // Bước 3 (MỚI): xử lý va chạm Bullet-Zombie
    resolve_bullet_collisions();

    // Bước 4 (MỚI): dọn entity đã chết khỏi danh sách + khỏi Grid
    cleanup_dead_entities();
}

int GameController::get_state() const {
    return static_cast<int>(state);
}

// ==================== MỚI ====================

bool GameController::place_plant(pvz::GridPosition pos, std::unique_ptr<pvz::Plant> plant) {
    if (plant == nullptr) {
        return false;
    }
    if (!grid.is_valid(pos) || grid.is_occupied(pos)) {
        return false;
    }
    if (sun < plant->get_cost()) {
        return false;
    }

    sun -= plant->get_cost();
    grid.place_plant(pos, plant.get()); // Grid chỉ giữ raw pointer để tra cứu
    entities.push_back(std::move(plant)); // entities giữ quyền sở hữu thật
    return true;
}

void GameController::add_zombie(std::unique_ptr<pvz::Zombie> zombie) {
    if (zombie == nullptr) {
        return;
    }
    zombie->set_grid(&grid);
    entities.push_back(std::move(zombie));
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

std::vector<pvz::Zombie*> GameController::get_zombies_in_row(int row) const {
    std::vector<pvz::Zombie*> result;
    for (const auto& e : entities) {
        if (e->get_type() == pvz::EntityType::ZOMBIE &&
            e->is_alive() &&
            e->get_position().row == row) {
            result.push_back(static_cast<pvz::Zombie*>(e.get()));
        }
    }
    return result;
}

void GameController::add_sun(int amount) { sun += amount; }
int GameController::get_sun() const { return sun; }

void GameController::resolve_bullet_collisions() {
    // TODO (Core Architect): CELL_HIT_RADIUS có thể cần tinh chỉnh sau khi test thật.
    constexpr float CELL_HIT_RADIUS = 0.5f;

    for (const auto& be : entities) {
        if (be->get_type() != pvz::EntityType::BULLET || !be->is_alive()) {
            continue;
        }
        pvz::Bullet* bullet = static_cast<pvz::Bullet*>(be.get());

        for (const auto& ze : entities) {
            if (ze->get_type() != pvz::EntityType::ZOMBIE || !ze->is_alive()) {
                continue;
            }
            if (ze->get_position().row != bullet->get_position().row) {
                continue;
            }

            float dx = static_cast<float>(ze->get_position().column) - bullet->get_column();
            if (dx < 0) dx = -dx;

            if (dx <= CELL_HIT_RADIUS) {
                ze->take_damage(bullet->get_damage());
                bullet->take_damage(bullet->get_health()); // huỷ bullet ngay sau khi trúng
                break;
            }
        }
    }
}

void GameController::cleanup_dead_entities() {
    for (const auto& e : entities) {
        if (!e->is_alive() && e->get_type() == pvz::EntityType::PLANT) {
            grid.remove_plant(e->get_position());
        }
    }

    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [](const auto& e) { return !e->is_alive(); }),
        entities.end()
    );
}

} // namespace godot
