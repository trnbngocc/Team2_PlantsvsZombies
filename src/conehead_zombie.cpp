#include "conehead_zombie.hpp"

namespace pvz {

ConeheadZombie::ConeheadZombie(GridPosition pos)
    : Zombie(
        pos,
        /*body_health=*/200,
        /*damage=*/10,
        /*speed=*/0.15f,
        ArmorType::CONE,
        /*armor_health=*/170
    ) {
}

} // namespace pvz