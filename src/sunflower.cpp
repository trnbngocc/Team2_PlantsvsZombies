#include "sunflower.hpp"
#include "game_controller.hpp"

#include <random>

namespace pvz {

Sunflower::Sunflower(GridPosition pos)
    : Plant(pos, HEALTH, COST) {}

bool Sunflower::roll_purple() {
    // Bộ sinh số ngẫu nhiên dùng chung cho mọi Sunflower, seed một lần.
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dice(0, 99);
    return dice(rng) < PURPLE_CHANCE_PERCENT;
}

void Sunflower::update(double delta_seconds) {
    if (!is_alive()) return;
    timer += delta_seconds;
    if (timer >= INTERVAL) {
        timer -= INTERVAL;
        ready = true;
    }
}

void Sunflower::act(godot::GameController& controller) {
    if (!ready) return;
    ready = false;

    const int sun_value = roll_purple() ? PURPLE_SUN_VALUE : GOLD_SUN_VALUE;

    // Sun rơi ra bản đồ (SunEconomy), người chơi click để nhặt.
    // Cần GameController::spawn_sun() — Core tích hợp sau khi nhận sun_economy.*.
    controller.spawn_sun(get_position(), sun_value);
}

} // namespace pvz