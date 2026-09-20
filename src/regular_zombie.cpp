#include "regular_zombie.hpp"

namespace pvz {

RegularZombie::RegularZombie(GridPosition pos)
    : Zombie(
        pos,
        /*body_health=*/200,
        /*damage=*/10,
        /*speed=*/0.15f,
        ArmorType::NONE,
        /*armor_health=*/0
    ) {
}

} // namespace pvz