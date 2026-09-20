#pragma once
#include "zombie.hpp"

namespace pvz {

class RegularZombie : public Zombie {
public:
    explicit RegularZombie(GridPosition pos);

    EntityType get_type() const override { return EntityType::ZOMBIE; }
    void update(double delta_seconds) override { default_update(delta_seconds); }
};

} // namespace pvz
