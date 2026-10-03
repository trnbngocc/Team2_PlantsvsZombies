#pragma once

#include "zombie.hpp"

namespace godot {

class GameController;

class WaveManager {
public:
    explicit WaveManager(GameController* game_controller);

    void spawn_regular(int row, float column);
    void spawn_conehead(int row, float column);
    void spawn_buckethead(int row, float column);

    void start_wave(int wave_number);

    // Update wave timing
    void update(double delta_seconds);

private:
    void spawn_zombie(
        pvz::Zombie::ArmorType armor_type,
        int row,
        float column
    );

    GameController* game_controller = nullptr;

    // Wave state
    int current_wave = 0;
    int zombies_to_spawn = 0;

    // Timer for next zombie
    double spawn_timer = 0.0;
    double next_spawn_delay = 0.0;
};

} // namespace godot