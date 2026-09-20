#include "wallnut.hpp"

namespace pvz {

WallNut::WallNut(GridPosition pos)
    // TODO (Plant Developer): chỉnh health nếu cần cân bằng (thường 300-400).
    : Plant(pos, /*health=*/400, /*cost=*/50) {}

} // namespace pvz
