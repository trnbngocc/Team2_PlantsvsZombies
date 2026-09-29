#include <cassert>
#include <iostream>

#include "zombie.hpp"

int main() {

    // ==========================================
    // 1. REGULAR ZOMBIE
    // ==========================================
    pvz::Zombie regular(
        {0, 5},
        200,    // HP
        10,     // Damage
        0.15f,  // Speed
        pvz::Zombie::ArmorType::NONE,
        0       // Armor HP
    );

    assert(regular.get_health() == 200);
    assert(regular.get_damage() == 10);
    assert(regular.get_speed() == 0.15f);
    assert(
        regular.get_armor_type() ==
        pvz::Zombie::ArmorType::NONE
    );
    assert(regular.get_armor_health() == 0);
    assert(!regular.has_armor());


    // ==========================================
    // 2. CONEHEAD ZOMBIE
    // ==========================================
    pvz::Zombie conehead(
        {0, 5},
        200,    // HP
        10,     // Damage
        0.15f,  // Speed
        pvz::Zombie::ArmorType::CONE,
        170     // Armor HP
    );

    assert(conehead.get_health() == 200);
    assert(conehead.get_damage() == 10);
    assert(conehead.get_speed() == 0.15f);
    assert(
        conehead.get_armor_type() ==
        pvz::Zombie::ArmorType::CONE
    );
    assert(conehead.get_armor_health() == 170);
    assert(conehead.has_armor());


    // ==========================================
    // 2.1. CONEHEAD ARMOR TEST
    // ==========================================

    // Hit armor: 170 -> 160
    // Body HP must stay 200
    conehead.take_damage(10);

    assert(conehead.get_armor_health() == 160);
    assert(conehead.get_health() == 200);

    // Reduce remaining armor: 160 -> 0
    // Body HP must still stay 200
    conehead.take_damage(160);

    assert(conehead.get_armor_health() == 0);
    assert(conehead.get_health() == 200);

    assert(
        conehead.get_armor_type() ==
        pvz::Zombie::ArmorType::NONE
    );

    assert(!conehead.has_armor());

    // Armor is gone.
    // The next hit damages the body.
    conehead.take_damage(10);

    assert(conehead.get_health() == 190);


    // ==========================================
    // 3. BUCKETHEAD ZOMBIE
    // ==========================================
    pvz::Zombie buckethead(
        {0, 5},
        200,    // HP
        10,     // Damage
        0.13f,  // Speed
        pvz::Zombie::ArmorType::BUCKET,
        450     // Armor HP
    );

    assert(buckethead.get_health() == 200);
    assert(buckethead.get_damage() == 10);
    assert(buckethead.get_speed() == 0.13f);
    assert(
        buckethead.get_armor_type() ==
        pvz::Zombie::ArmorType::BUCKET
    );
    assert(buckethead.get_armor_health() == 450);
    assert(buckethead.has_armor());


    // ==========================================
    // 3.1. BUCKETHEAD ARMOR TEST
    // ==========================================

    // Hit armor: 450 -> 440
    // Body HP must stay 200
    buckethead.take_damage(10);

    assert(buckethead.get_armor_health() == 440);
    assert(buckethead.get_health() == 200);


    // ==========================================
    // 4. VISUAL STATE TEST
    // ==========================================
    pvz::Zombie zombie(
        {0, 5},
        100,
        20,
        1.0f
    );

    // HP = 100 -> NORMAL
    assert(
        zombie.get_visual_state() ==
        pvz::Zombie::VisualState::NORMAL
    );

    // 49 damage -> HP = 51 -> NORMAL
    zombie.take_damage(49);

    assert(zombie.get_health() == 51);

    assert(
        zombie.get_visual_state() ==
        pvz::Zombie::VisualState::NORMAL
    );

    // 1 damage -> HP = 50 -> ONE_ARM_LOST
    zombie.take_damage(1);

    assert(zombie.get_health() == 50);

    assert(
        zombie.get_visual_state() ==
        pvz::Zombie::VisualState::ONE_ARM_LOST
    );

    // 49 damage -> HP = 1 -> ONE_ARM_LOST
    zombie.take_damage(49);

    assert(zombie.get_health() == 1);

    assert(
        zombie.get_visual_state() ==
        pvz::Zombie::VisualState::ONE_ARM_LOST
    );

    // 1 damage -> HP = 0 -> DEAD
    zombie.take_damage(1);

    assert(zombie.get_health() == 0);
    assert(!zombie.is_alive());

    assert(
        zombie.get_visual_state() ==
        pvz::Zombie::VisualState::DEAD
    );

    std::cout << "Zombie tests passed.\n";

    return 0;
}