#pragma once
#include "plant.hpp"

namespace pvz {

// Cherry-bomb: nổ đúng 1 lần trong act() (không cần đếm giờ trong update()),
// gây damage rồi tự huỷ.
class CherryBomb : public Plant {
public:
    explicit CherryBomb(GridPosition pos);

    EntityType get_type() const override { return EntityType::PLANT; }
    void update(double delta_seconds) override {} // không cần đếm giờ
    void act(godot::GameController& controller) override;

private:
    bool exploded = false;
    int explosion_damage = 1800;
};

} // namespace pvz
