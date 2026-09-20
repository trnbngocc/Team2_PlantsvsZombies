#pragma once
#include "plant.hpp"

namespace pvz {

class Peashooter : public Plant {
public:
    explicit Peashooter(GridPosition pos);

    EntityType get_type() const override { return EntityType::PLANT; }
    void update(double delta_seconds) override;
    void act(godot::GameController& controller) override;

private:
    double timer = 0.0;
    static constexpr double INTERVAL = 1.5;
    bool ready = false;
    int bullet_damage = 20;
};

} // namespace pvz
