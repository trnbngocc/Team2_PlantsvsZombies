#include "zombie.hpp"
#include "plant.hpp"

#include <algorithm>

namespace pvz {

Zombie::Zombie(
    GridPosition initial_position,
    int initial_health,
    int initial_damage,
    float initial_speed,
    ArmorType initial_armor,
    int initial_armor_health
)
    : Entity(initial_position, initial_health),
      damage(initial_damage),
      speed(initial_speed),
      armor_type(initial_armor),
      armor_health(std::max(0, initial_armor_health)),
      column_position(static_cast<float>(initial_position.column)),
      max_health(std::max(0, initial_health)) {
}

EntityType Zombie::get_type() const {
    return EntityType::ZOMBIE;
}

void Zombie::update(double delta_seconds) {
    default_update(delta_seconds);
}

int Zombie::get_damage() const {
    return damage;
}

float Zombie::get_speed() const {
    return speed;
}

Zombie::ArmorType Zombie::get_armor_type() const {
    return armor_type;
}

int Zombie::get_armor_health() const {
    return armor_health;
}

bool Zombie::has_armor() const {
    return armor_type != ArmorType::NONE && armor_health > 0;
}

Zombie::VisualState Zombie::get_visual_state() const {
    // Zombie đã chết.
    if (!is_alive() || get_health() <= 0) {
        return VisualState::DEAD;
    }

    // Zombie còn sống nhưng HP <= 50% HP tối đa.
    //
    // Dùng phép nhân để tránh vấn đề chia số nguyên:
    //     health * 2 <= max_health
    //
    // Ví dụ:
    //     max = 100, hp = 50  -> ONE_ARM_LOST
    //     max = 100, hp = 51  -> NORMAL
    //     max = 101, hp = 50  -> ONE_ARM_LOST
    //     max = 101, hp = 51  -> ONE_ARM_LOST
    if (max_health > 0 &&
        get_health() * 2 <= max_health) {
        return VisualState::ONE_ARM_LOST;
    }

    return VisualState::NORMAL;
}

void Zombie::receive_damage(int amount) {
    if (amount <= 0 || !active) {
        return;
    }

    // Armor hấp thụ damage trước.
    if (has_armor()) {
        armor_health = std::max(0, armor_health - amount);

        if (armor_health == 0) {
            armor_type = ArmorType::NONE;
        }

        return;
    }

    // Sau khi armor hết, damage mới đi vào HP.
    Entity::receive_damage(amount);
}

void Zombie::set_grid(Grid* grid_ref) {
    grid = grid_ref;
}

void Zombie::default_update(double delta_seconds) {
    if (!is_alive() || grid == nullptr) {
        return;
    }

    GridPosition current = get_position();

    Plant* blocker = grid->get_plant_at(current);

    // Có Plant trong cùng ô -> Zombie ăn Plant.
    //
    // ONE_ARM_LOST không ảnh hưởng damage hoặc attack interval.
    // Zombie vẫn sử dụng đúng một logic attack duy nhất.
    if (blocker != nullptr && blocker->is_alive()) {
        attack_cooldown += delta_seconds;

        if (attack_cooldown >= ATTACK_INTERVAL) {
            attack_cooldown -= ATTACK_INTERVAL;
            blocker->take_damage(damage);
        }
    }
    else {
        // Không có Plant -> Zombie tiếp tục đi sang trái.
        column_position -= speed * static_cast<float>(delta_seconds);

        set_position(
            GridPosition{
                current.row,
                static_cast<int>(column_position)
            }
        );

        if (column_position < 0.0f) {
            // TODO: inform GameController of defeat.
        }
    }
}

} // namespace pvz