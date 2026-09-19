#include "zombie.hpp"
#include "plant.hpp" // cần type đầy đủ vì gọi blocker->take_damage()/is_alive()

namespace pvz {

Zombie::Zombie(
    GridPosition initial_position,
    int initial_health,
    int initial_damage,
    float initial_speed
)
    : Entity(initial_position, initial_health),
      damage(initial_damage),
      speed(initial_speed),
      column_position(static_cast<float>(initial_position.column)) {}

void Zombie::set_grid(Grid* grid_ref) {
    grid = grid_ref;
}

void Zombie::default_update(double delta_seconds) {
    if (!is_alive() || grid == nullptr) {
        return;
    }

    GridPosition current = get_position();
    Plant* blocker = grid->get_plant_at(current);

    if (blocker != nullptr && blocker->is_alive()) {
        // Bị chặn — đứng yên, cắn theo tick
        attack_cooldown += delta_seconds;
        if (attack_cooldown >= ATTACK_INTERVAL) {
            attack_cooldown -= ATTACK_INTERVAL;
            blocker->take_damage(damage);
        }
    } else {
        // Không bị chặn — di chuyển sang trái (column giảm dần)
        column_position -= speed * static_cast<float>(delta_seconds);
        set_position(GridPosition{current.row, static_cast<int>(column_position)});

        if (column_position < 0.0f) {
            // TODO (Zombie/Difficulty Developer + Core Architect): Zombie vượt
            // qua hết bàn cờ — cần cách báo hiệu cho GameController để thua game.
        }
    }
}

int Zombie::get_damage() const { return damage; }
float Zombie::get_speed() const { return speed; }

} // namespace pvz
