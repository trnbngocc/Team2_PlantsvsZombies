#pragma once

#include "grid.hpp"

namespace pvz {

enum class EntityType {
    PLANT,
    ZOMBIE,
    BULLET
};

class Entity {
public:
    Entity(GridPosition initial_position, int initial_health);
    virtual ~Entity() = default;

    virtual EntityType get_type() const = 0;
    virtual void update(double delta_seconds) = 0;

    virtual void take_damage(int amount);
    bool is_alive() const;
    int get_health() const;

    GridPosition get_position() const;
    void set_position(GridPosition new_position);

protected:
    GridPosition position;
    int health;
    bool active = true;
};

} // namespace pvz