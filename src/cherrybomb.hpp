#pragma once
#include "plant.hpp"

namespace pvz {

// Cherry-bomb: bom nổ chậm.
//   Giai đoạn 1: đếm ngược FUSE_TIME giây (là một Plant bình thường, có thể bị Zombie cắn chết).
//   Giai đoạn 2: nổ, gây Instant Damage lên mọi Zombie cùng hàng.
//   Giai đoạn 3: Zombie còn sống bị gắn BURNING (DoT), rồi Cherry-bomb tự huỷ.
class CherryBomb : public Plant {
public:
    explicit CherryBomb(GridPosition pos);

    EntityType get_type() const override { return EntityType::PLANT; }
    void update(double delta_seconds) override; // đếm ngược ngòi nổ
    void act(godot::GameController& controller) override; // nổ khi ngòi hết giờ

private:
    // ---- Thông số cân bằng (chỉnh ở đây) ----
    static constexpr int    HEALTH            = 100;   // máu cơ bản, có thể bị Zombie cắn chết trước khi nổ
    static constexpr int    COST              = 150;
    static constexpr double FUSE_TIME         = 2.0;   // giây đếm ngược
    static constexpr int    EXPLOSION_DAMAGE  = 1800;  // Instant Damage
    static constexpr double BURN_DURATION     = 3.0;   // giây
    static constexpr double BURN_TICK_INTERVAL = 0.5;  // giây / lần rút máu
    static constexpr int    BURN_TICK_DAMAGE  = 20;    // 6 tick x 20 = 120 damage

    double fuse_timer = 0.0;
    bool   fuse_finished = false; // ngòi đã cháy hết, chờ act() để nổ
    bool   exploded = false;      // đảm bảo chỉ nổ đúng 1 lần
};

} // namespace pvz