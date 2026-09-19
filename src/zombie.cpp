#include "zombie.hpp"

namespace pvz {

Zombie::Zombie(
    GridPosition initial_position,
    int initial_health,
    int initial_damage,
    float initial_speed
)
    : Entity(initial_position, initial_health),
      damage(initial_damage),
      speed(initial_speed) {
}

int Zombie::get_damage() const {
    return damage;
}

float Zombie::get_speed() const {
    return speed;
}

} // namespace pvz