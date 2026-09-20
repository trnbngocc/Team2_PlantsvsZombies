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
      column_position(static_cast<float>(initial_position.column)) {
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

void Zombie::take_damage(int amount) {
    if (amount <= 0 || !active) {
        return;
    }

    // Nếu Zombie còn giáp, sát thương sẽ đánh vào giáp trước.
    if (has_armor()) {
        armor_health = std::max(0, armor_health - amount);

        // Giáp vỡ thì bỏ trạng thái giáp.
        if (armor_health == 0) {
            armor_type = ArmorType::NONE;
        }

        return;
    }

    // Không còn giáp thì sát thương đánh trực tiếp vào thân Zombie.
    Entity::take_damage(amount);
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

    if (blocker != nullptr && blocker->is_alive()) {
        attack_cooldown += delta_seconds;

        if (attack_cooldown >= ATTACK_INTERVAL) {
            attack_cooldown -= ATTACK_INTERVAL;
            blocker->take_damage(damage);
        }
    } else {
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