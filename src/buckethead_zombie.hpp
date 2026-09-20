#pragma once
#include "zombie.hpp"

namespace pvz {

class BucketheadZombie : public Zombie {
public:
    explicit BucketheadZombie(GridPosition pos);

    EntityType get_type() const override { return EntityType::ZOMBIE; }
    void update(double delta_seconds) override { default_update(delta_seconds); }
};

} // namespace pvz
