#include "cherrybomb.hpp"
#include "game_controller.hpp"

namespace pvz {

CherryBomb::CherryBomb(GridPosition pos)
    : Plant(pos, /*health=*/1, /*cost=*/150) {}

void CherryBomb::act(godot::GameController& controller) {
    if (exploded) {
        return;
    }
    exploded = true;

    GridPosition center = get_position();

    // TODO (Plant Developer): hiện chỉ gây damage cho Zombie CÙNG HÀNG (đơn giản
    // hoá). Muốn nổ đúng 3x3 ô, cần Core Architect bổ sung thêm 1 hàm truy vấn
    // theo bán kính vào GameController.
    for (Zombie* z : controller.get_zombies_in_row(center.row)) {
        z->take_damage(explosion_damage);
    }

    take_damage(get_health()); // tự huỷ ngay sau khi nổ
}

} // namespace pvz
