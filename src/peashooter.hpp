#pragma once
#include "plant.hpp"

namespace pvz {

// Peashooter: bắn đều mỗi INTERVAL giây khi có Zombie cùng hàng VÀ ở phía trước
// (column >= vị trí Peashooter). Mỗi phát có CRIT_CHANCE_PERCENT % là đạn chí mạng
// (damage x CRIT_MULTIPLIER).
class Peashooter : public Plant {
public:
    explicit Peashooter(GridPosition pos);

    EntityType get_type() const override { return EntityType::PLANT; }
    void update(double delta_seconds) override;
    void act(godot::GameController& controller) override;

private:
    // ---- Thông số cân bằng (chỉnh ở đây) ----
    static constexpr int    HEALTH              = 100;
    static constexpr int    COST                = 100;
    static constexpr double INTERVAL            = 1.5;  // giây giữa hai phát bắn
    static constexpr int    BASE_DAMAGE         = 20;
    static constexpr int    CRIT_MULTIPLIER     = 2;    // 20 -> 40
    static constexpr int    CRIT_CHANCE_PERCENT = 20;   // 20% chí mạng

    // Gieo xúc xắc: true nếu phát này là đạn chí mạng.
    static bool roll_critical();

    double timer = 0.0;
    bool ready = false;
};

} // namespace pvz