#include "sunflower.hpp"
#include "game_controller.hpp"

namespace pvz {

Sunflower::Sunflower(GridPosition pos)
    : Plant(pos, /*health=*/100, /*cost=*/50) {}

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
    controller.add_sun(sun_amount);
}

} // namespace pvz
