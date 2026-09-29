#include "cherrybomb.hpp"
#include "game_controller.hpp"

namespace pvz {

CherryBomb::CherryBomb(GridPosition pos)
    : Plant(pos, HEALTH, COST) {}

void CherryBomb::update(double delta_seconds) {
    if (!is_alive() || fuse_finished) {
        return;
    }

    fuse_timer += delta_seconds;
    if (fuse_timer >= FUSE_TIME) {
        fuse_finished = true;
    }
}

void CherryBomb::act(godot::GameController& controller) {
    // Chưa hết ngòi hoặc đã nổ rồi -> không làm gì.
    if (!fuse_finished || exploded) {
        return;
    }
    exploded = true;

    const GridPosition center = get_position();

    // Quét toàn bộ Zombie cùng hàng.
    for (Zombie* z : controller.get_zombies_in_row(center.row)) {
        // Giai đoạn 2: Instant Damage.
        z->take_damage(EXPLOSION_DAMAGE);

        // Giai đoạn 3: gắn BURNING lên Zombie còn sống.
        // apply_status() tự ghi đè nếu Zombie đã cháy -> không stack DoT.
        if (z->is_alive()) {
            StatusEffect burn;
            burn.type = StatusEffectType::BURNING;
            burn.remaining_duration = BURN_DURATION;
            burn.damage_per_tick = BURN_TICK_DAMAGE;
            burn.tick_interval = BURN_TICK_INTERVAL;
            z->apply_status(burn);
        }
    }

    // Tự huỷ; GameController::cleanup_dead_entities() sẽ gỡ khỏi Grid và entities.
    take_damage(get_health());
}

} // namespace pvz