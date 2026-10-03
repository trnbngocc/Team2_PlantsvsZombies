#pragma once

#include <memory>
#include <vector>

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>


#include "entity.hpp"
#include "grid.hpp"
#include "sun_economy.hpp"
#include "plant.hpp"
#include "zombie.hpp"
#include "wave_manager.hpp"

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
    enum PlantType {
        PEASHOOTER = 0,
        SUNFLOWER = 1,
        WALLNUT = 2,
        CHILI_PEPPER = 3
    };


private:
    GameState state = GameState::SETUP;

    // GameController owns and manages all gameplay entities.
    std::vector<std::unique_ptr<pvz::Entity>> entities;

    // GameController owns the game board.
    pvz::Grid grid;

    // GameController owns Sun drops on the battlefield.
    pvz::SunEconomy sun_economy;

    // GameController owns and manages zombie waves.
    WaveManager wave_manager;

    int sun = 50;

protected:
    static void _bind_methods();


public:
    GameController();

    void _ready() override;

    // Godot game loop.
    void _process(double delta) override;

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

    // ==================== PLANT ====================

    // Hàm đặt cây dùng để bind ra Godot (UI gọi trực tiếp từ GDScript)
    bool place_plant_by_type(int row, int col, int plant_type);

    // C++ core place_plant
    bool place_plant(
        pvz::GridPosition pos,
        std::unique_ptr<pvz::Plant> plant
    );

    // ==================== UI / QUERY DATA ====================

    // Trả về danh sách Sun đang rơi để UI vẽ
    Array get_sun_drops() const;

    // Trả về danh sách Zombie, Đạn, Cây để UI vẽ lên màn hình mỗi frame
    Array get_zombies_data() const;
    Array get_bullets_data() const;
    Array get_plants_data() const;


    // ==================== ZOMBIE ====================

    // Zombie Developer / Wave Manager gọi khi spawn Zombie —
    // tự gán Grid cho Zombie.
    void add_zombie(std::unique_ptr<pvz::Zombie> zombie);

    // Peashooter gọi trong act().
    bool has_zombie_in_row(int row) const;

    // ChiliPepper và các Plant khác cần danh sách Zombie thật
    // gọi hàm này trong act().
    std::vector<pvz::Zombie*> get_zombies_in_row(int row) const;

    // ==================== SUN ====================

    // Sunflower gọi trong act().
    void add_sun(int amount);
    int get_sun() const;

    // SunEconomy interface.
    int spawn_sun(pvz::GridPosition pos, int value);
    int collect_sun(int id);
    const std::vector<pvz::SunDrop>& get_suns() const;

private:
    void resolve_bullet_collisions();
    void cleanup_dead_entities();
};

} // namespace godot