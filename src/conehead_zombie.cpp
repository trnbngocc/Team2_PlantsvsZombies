#include "conehead_zombie.hpp"

namespace pvz {

ConeheadZombie::ConeheadZombie(GridPosition pos)
    : Zombie(pos, /*health=*/370, /*damage=*/10, /*speed=*/0.15f) {}

} // namespace pvz
