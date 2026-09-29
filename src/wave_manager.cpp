#include "wave_manager.hpp"
#include "game_controller.hpp"

#include <memory>

namespace godot {

WaveManager::WaveManager(GameController* game_controller_ref)
    : game_controller(game_controller_ref) {
}

void WaveManager::spawn_zombie(
    pvz::Zombie::ArmorType armor_type,
    int row,
    float column
) {
    if (game_controller == nullptr) {
        return;
    }

    int health = 200;
    int damage = 10;
    float speed = 0.15f;
    int armor_health = 0;

    switch (armor_type) {
        case pvz::Zombie::ArmorType::NONE:
            break;

        case pvz::Zombie::ArmorType::CONE:
            armor_health = 170;
            break;

        case pvz::Zombie::ArmorType::BUCKET:
            speed = 0.13f;
            armor_health = 450;
            break;
    }

    auto zombie = std::make_unique<pvz::Zombie>(
        pvz::GridPosition{
            row,
            static_cast<int>(column)
        },
        health,
        damage,
        speed,
        armor_type,
        armor_health
    );

    game_controller->add_zombie(std::move(zombie));
}


void WaveManager::spawn_regular(int row, float column) {
    spawn_zombie(
        pvz::Zombie::ArmorType::NONE,
        row,
        column
    );
}


void WaveManager::spawn_conehead(int row, float column) {
    spawn_zombie(
        pvz::Zombie::ArmorType::CONE,
        row,
        column
    );
}


void WaveManager::spawn_buckethead(int row, float column) {
    spawn_zombie(
        pvz::Zombie::ArmorType::BUCKET,
        row,
        column
    );
}


void WaveManager::start_wave(int wave_number) {
    if (wave_number <= 0) {
        return;
    }

    const float spawn_column = 8.0f;

    // Wave 1: 3 Regular Zombies
    if (wave_number == 1) {
        spawn_regular(0, spawn_column);
        spawn_regular(2, spawn_column);
        spawn_regular(4, spawn_column);
        return;
    }

    // Wave 2: Regular + Conehead
    if (wave_number == 2) {
        spawn_regular(0, spawn_column);
        spawn_regular(2, spawn_column);
        spawn_conehead(4, spawn_column);
        return;
    }

    // Wave 3: Regular + Conehead + Buckethead
    spawn_regular(0, spawn_column);
    spawn_conehead(1, spawn_column);
    spawn_regular(2, spawn_column);
    spawn_buckethead(3, spawn_column);
}

} // namespace godot