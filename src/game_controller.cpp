#include "game_controller.hpp"

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

void GameController::update_entities(double delta_seconds) {
    for (const auto& entity : entities) {
        if (entity != nullptr && entity->is_alive()) {
            entity->update(delta_seconds);
        }
    }
}

int GameController::get_state() const {
    return static_cast<int>(state);
}

} // namespace godot