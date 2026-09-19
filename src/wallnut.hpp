#pragma once
#include "plant.hpp"

namespace pvz {

// Wall-nut: máu cao, không hành động chủ động — update()/act() gần như rỗng.
class WallNut : public Plant {
public:
    explicit WallNut(GridPosition pos);

    EntityType get_type() const override { return EntityType::PLANT; }
    void update(double delta_seconds) override {} // không cần làm gì
    // act() dùng bản mặc định của Plant (không làm gì) — không cần override
};

} // namespace pvz
