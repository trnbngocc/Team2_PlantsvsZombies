#pragma once

#include "entity.hpp"

namespace pvz {

class Zombie : public Entity {
public:
    Zombie(
        GridPosition initial_position,
        int initial_health,
        int initial_damage,
        float initial_speed
    );

    int get_damage() const;
    float get_speed() const;

protected:
    int damage;
    float speed;
};

} // namespace pvz