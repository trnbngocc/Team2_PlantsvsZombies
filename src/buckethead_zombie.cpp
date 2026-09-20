#include "buckethead_zombie.hpp"

namespace pvz {

BucketheadZombie::BucketheadZombie(GridPosition pos)
    : Zombie(
        pos,
        /*body_health=*/200,
        /*damage=*/10,
        /*speed=*/0.13f,
        ArmorType::BUCKET,
        /*armor_health=*/450
    ) {
}

} // namespace pvz