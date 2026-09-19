#pragma once
#include "plant.hpp"

namespace pvz {

class Sunflower : public Plant {
public:
    explicit Sunflower(GridPosition pos);

    EntityType get_type() const override { return EntityType::PLANT; }
    void update(double delta_seconds) override;
    void act(godot::GameController& controller) override;

private:
    double timer = 0.0;
    static constexpr double INTERVAL = 8.0; // TODO (Plant Developer): chỉnh nếu cần cân bằng
    bool ready = false;
    int sun_amount = 25;
};

} // namespace pvz
