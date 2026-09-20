#include "regular_zombie.hpp"

namespace pvz {

RegularZombie::RegularZombie(GridPosition pos)
    // TODO (Zombie Developer): tinh chỉnh speed — hiện 0.15 ô/giây (~60s băng hết sân).
    : Zombie(pos, /*health=*/200, /*damage=*/10, /*speed=*/0.15f) {}

} // namespace pvz
