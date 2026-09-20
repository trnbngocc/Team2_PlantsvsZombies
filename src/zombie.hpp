#pragma once

#include "entity.hpp"
#include "grid.hpp"

namespace pvz {

class Zombie : public Entity {
public:
    Zombie(
        GridPosition initial_position,
        int initial_health,
        int initial_damage,
        float initial_speed
    );

    int get_damage() const;
    float get_speed() const;

    // MỚI: GameController gọi ngay khi spawn Zombie, để Zombie tự hỏi Grid
    // được trong update() của chính nó.
    void set_grid(Grid* grid_ref);

protected:
    // MỚI: logic di chuyển/tấn công dùng CHUNG cho cả 3 loại Zombie hiện có.
    // Mỗi class con vẫn PHẢI tự viết update() (bắt buộc từ Entity), nhưng có
    // thể chỉ gọi default_update(delta) nếu hành vi giống nhau — hoặc tự viết
    // logic riêng nếu Zombie đó cần hành vi đặc biệt (VD: Zombie nhảy qua ô).
    void default_update(double delta_seconds);

    int damage;
    float speed;
    Grid* grid = nullptr;

private:
    double attack_cooldown = 0.0;
    static constexpr double ATTACK_INTERVAL = 1.0; // mỗi 1 giây cắn 1 lần khi bị chặn
    float column_position = 0.0f;
};

} // namespace pvz
