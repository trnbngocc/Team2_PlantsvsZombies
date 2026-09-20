#include "peashooter.hpp"
#include "game_controller.hpp"

namespace pvz {

Peashooter::Peashooter(GridPosition pos)
    : Plant(pos, /*health=*/100, /*cost=*/100) {}

void Peashooter::update(double delta_seconds) {
    if (!is_alive()) return;
    timer += delta_seconds;
    if (timer >= INTERVAL) {
        timer -= INTERVAL;
        ready = true;
    }
}

void Peashooter::act(godot::GameController& controller) {
    if (!ready) return;
    ready = false;

    GridPosition pos = get_position();
    if (!controller.has_zombie_in_row(pos.row)) {
        return;
    }

    // Dùng đúng spawn_bullet() đã có sẵn trong GameController — không cần đổi gì ở đó.
    controller.spawn_bullet(pos.row, static_cast<float>(pos.column) + 1.0f, bullet_damage);
}

} // namespace pvz
