#include "wave_manager.hpp"
#include "game_controller.hpp"

#include <memory>
#include <random>

namespace godot {

WaveManager::WaveManager(GameController* game_controller_ref)
    : game_controller(game_controller_ref) {
}

namespace {
    std::random_device random_device;
    std::mt19937 random_engine(random_device());

    int random_int(int min_value, int max_value) {
        std::uniform_int_distribution<int> distribution(
            min_value,
            max_value
        );

        return distribution(random_engine);
    }
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

    current_wave = wave_number;

    // Random number of zombies in this wave
    if (wave_number == 1) {
        zombies_to_spawn = random_int(3, 5);
    }
    else if (wave_number == 2) {
        zombies_to_spawn = random_int(4, 6);
    }
    else {
        zombies_to_spawn = random_int(5, 8);
    }

    spawn_timer = 0.0;
    next_spawn_delay = random_int(1,4);
}

void WaveManager::update(double delta_seconds) {
    if (game_controller == nullptr) {
        return;
    }

    if (zombies_to_spawn <= 0) {
        return;
    }

    spawn_timer += delta_seconds;

    if (spawn_timer < next_spawn_delay) {
        return;
    }

    // Random row from 0 to 4
    int row = random_int(0, 4);

    // Random zombie type
    int zombie_type = random_int(0, 2);

    if (zombie_type == 0) {
        spawn_regular(row, 8.0f);
    }
    else if (zombie_type == 1) {
        spawn_conehead(row, 8.0f);
    }
    else {
        spawn_buckethead(row, 8.0f);
    }

    zombies_to_spawn--;

    spawn_timer = 0.0;
    next_spawn_delay = random_int(1, 4);
}

} // namespace godot