#pragma once

#include "entity.hpp"

namespace pvz {

class Plant : public Entity {
public:
    Plant(
        GridPosition initial_position,
        int initial_health,
        int initial_cost
    );

    int get_cost() const;

protected:
    int cost;
};

} // namespace pvz