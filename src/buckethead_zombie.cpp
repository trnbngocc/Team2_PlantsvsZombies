#include "buckethead_zombie.hpp"

namespace pvz {

BucketheadZombie::BucketheadZombie(GridPosition pos)
    : Zombie(pos, /*health=*/650, /*damage=*/10, /*speed=*/0.13f) {}

} // namespace pvz
