#pragma once
#include "plant.hpp"

namespace pvz {

// Sunflower: mỗi INTERVAL giây rớt Sun. Mỗi lần có PURPLE_CHANCE_PERCENT % ra
// "Mặt trời tím" (PURPLE_SUN_VALUE Sun), còn lại là "Mặt trời vàng" (GOLD_SUN_VALUE Sun).
class Sunflower : public Plant {
public:
    explicit Sunflower(GridPosition pos);

    EntityType get_type() const override { return EntityType::PLANT; }
    void update(double delta_seconds) override;
    void act(godot::GameController& controller) override;

private:
    // ---- Thông số cân bằng (chỉnh ở đây) ----
    static constexpr int    HEALTH                = 100;
    static constexpr int    COST                  = 50;
    static constexpr double INTERVAL              = 8.0;  // giây giữa hai lần rớt Sun
    static constexpr int    GOLD_SUN_VALUE        = 25;   // Mặt trời vàng
    static constexpr int    PURPLE_SUN_VALUE      = 37;   // Mặt trời tím (~1.5 lần)
    static constexpr int    PURPLE_CHANCE_PERCENT = 20;   // 20% ra Mặt trời tím

    // Gieo xúc xắc: true nếu lần này ra Mặt trời tím.
    static bool roll_purple();

    double timer = 0.0;
    bool ready = false;
};

} // namespace pvz