#pragma once
#include "zombie.hpp"

namespace pvz {

class ConeheadZombie : public Zombie {
public:
    explicit ConeheadZombie(GridPosition pos);

    EntityType get_type() const override { return EntityType::ZOMBIE; }
    void update(double delta_seconds) override { default_update(delta_seconds); }
};

} // namespace pvz
