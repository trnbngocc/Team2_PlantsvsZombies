#include "wave_manager.hpp"
#include "game_controller.hpp"

#include <memory>

namespace godot {

WaveManager::WaveManager(GameController* game_controller_ref)
    : game_controller(game_controller_ref) {
}

void WaveManager::spawn_regular(int row, float column) {
    if (game_controller == nullptr) {
        return;
    }

    auto zombie = std::make_unique<pvz::Zombie>(
        pvz::GridPosition{row, static_cast<int>(column)},
        200,    // HP
        10,     // Damage
        0.15f,  // Speed
        pvz::Zombie::ArmorType::NONE,
        0       // Armor HP
    );

    game_controller->add_zombie(std::move(zombie));
}

void WaveManager::spawn_conehead(int row, float column) {
    if (game_controller == nullptr) {
        return;
    }

    auto zombie = std::make_unique<pvz::Zombie>(
        pvz::GridPosition{row, static_cast<int>(column)},
        200,    // HP
        10,     // Damage
        0.15f,  // Speed
        pvz::Zombie::ArmorType::CONE,
        170     // Armor HP
    );

    game_controller->add_zombie(std::move(zombie));
}

void WaveManager::spawn_buckethead(int row, float column) {
    if (game_controller == nullptr) {
        return;
    }

    auto zombie = std::make_unique<pvz::Zombie>(
        pvz::GridPosition{row, static_cast<int>(column)},
        200,    // HP
        10,     // Damage
        0.13f,  // Speed
        pvz::Zombie::ArmorType::BUCKET,
        450     // Armor HP
    );

    game_controller->add_zombie(std::move(zombie));
}

} // namespace godot