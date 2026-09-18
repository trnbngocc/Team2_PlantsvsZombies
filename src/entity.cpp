#include "entity.hpp"

#include <algorithm>

namespace pvz {

Entity::Entity(GridPosition initial_position, int initial_health)
    : position(initial_position), health(initial_health) {
}

void Entity::take_damage(int amount) {
    if (amount <= 0 || !active) {
        return;
    }

    health = std::max(0, health - amount);
    active = health > 0;
}

bool Entity::is_alive() const {
    return active;
}

int Entity::get_health() const {
    return health;
}

GridPosition Entity::get_position() const {
    return position;
}

void Entity::set_position(GridPosition new_position) {
    position = new_position;
}

} // namespace pvz