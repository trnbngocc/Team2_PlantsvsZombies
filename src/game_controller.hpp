#pragma once

#include <memory>
#include <vector>

#include <godot_cpp/classes/node.hpp>

#include "entity.hpp"
#include "grid.hpp"
#include "plant.hpp"   // MỚI
#include "zombie.hpp"  // MỚI

namespace godot {

class GameController : public Node {
    GDCLASS(GameController, Node)

public:
    enum class GameState {
        SETUP,
        PLAYING,
        PAUSED,
        VICTORY,
        DEFEAT
    };

private:
    GameState state = GameState::SETUP;

    // GameController owns and manages all gameplay entities.
    std::vector<std::unique_ptr<pvz::Entity>> entities;

    // GameController owns the game board.
    pvz::Grid grid;

    int sun = 50; // MỚI — Sun khởi điểm

protected:
    static void _bind_methods();

public:
    void _ready() override;

    void start_game();
    void pause_game();
    void resume_game();
    void finish_game(bool won);

    void add_entity(std::unique_ptr<pvz::Entity> entity);

    void spawn_bullet(
        int row,
        float start_column,
        int damage,
        float speed_cells_per_second = 5.0F
    );

    void update_entities(double delta_seconds);

    int get_state() const;

    // ==================== MỚI ====================

    // UI gọi khi người chơi đặt cây — tự kiểm tra ô trống + đủ Sun, tự trừ Sun,
    // tự đăng ký vào Grid. Trả về false nếu không đặt được (không thêm vào entities).
    bool place_plant(pvz::GridPosition pos, std::unique_ptr<pvz::Plant> plant);

    // Zombie Developer / Wave Manager gọi khi spawn Zombie — tự gán Grid cho Zombie.
    void add_zombie(std::unique_ptr<pvz::Zombie> zombie);

    // Peashooter gọi trong act()
    bool has_zombie_in_row(int row) const;

    // CherryBomb (và các Plant khác cần danh sách Zombie thật) gọi trong act()
    std::vector<pvz::Zombie*> get_zombies_in_row(int row) const;

    // Sunflower gọi trong act()
    void add_sun(int amount);
    int get_sun() const;

private:
    void resolve_bullet_collisions(); // MỚI
    void cleanup_dead_entities();     // MỚI
};

} // namespace godot
