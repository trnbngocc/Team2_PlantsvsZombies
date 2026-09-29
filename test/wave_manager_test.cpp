#include <cassert>
#include <iostream>

#include "game_controller.hpp"
#include "wave_manager.hpp"

int main() {

    godot::GameController game_controller;
    godot::WaveManager wave_manager(&game_controller);

    // ==========================================
    // 1. TEST WAVE MANAGER FUNCTIONS
    // ==========================================

    wave_manager.spawn_regular(0, 8.0f);
    wave_manager.spawn_conehead(1, 8.0f);
    wave_manager.spawn_buckethead(2, 8.0f);

    std::cout << "WaveManager spawn functions executed.\n";


    // ==========================================
    // 2. TEST REGULAR ZOMBIE
    // ==========================================

    pvz::Zombie regular(
        {0, 8},
        200,
        10,
        0.15f,
        pvz::Zombie::ArmorType::NONE,
        0
    );

    assert(regular.get_health() == 200);
    assert(regular.get_damage() == 10);
    assert(regular.get_speed() == 0.15f);
    assert(regular.get_armor_type() ==
           pvz::Zombie::ArmorType::NONE);
    assert(regular.get_armor_health() == 0);


    // ==========================================
    // 3. TEST CONEHEAD ZOMBIE
    // ==========================================

    pvz::Zombie conehead(
        {1, 8},
        200,
        10,
        0.15f,
        pvz::Zombie::ArmorType::CONE,
        170
    );

    assert(conehead.get_health() == 200);
    assert(conehead.get_damage() == 10);
    assert(conehead.get_speed() == 0.15f);
    assert(conehead.get_armor_type() ==
           pvz::Zombie::ArmorType::CONE);
    assert(conehead.get_armor_health() == 170);


    // ==========================================
    // 4. TEST BUCKETHEAD ZOMBIE
    // ==========================================

    pvz::Zombie buckethead(
        {2, 8},
        200,
        10,
        0.13f,
        pvz::Zombie::ArmorType::BUCKET,
        450
    );

    assert(buckethead.get_health() == 200);
    assert(buckethead.get_damage() == 10);
    assert(buckethead.get_speed() == 0.13f);
    assert(buckethead.get_armor_type() ==
           pvz::Zombie::ArmorType::BUCKET);
    assert(buckethead.get_armor_health() == 450);


    std::cout << "All WaveManager and Zombie tests passed.\n";

    return 0;
}