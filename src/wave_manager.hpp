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

private:
    void spawn_zombie(
        pvz::Zombie::ArmorType armor_type,
        int row,
        float column
    );

    GameController* game_controller = nullptr;
};

} // namespace godot