#pragma once

#include "entity.hpp"
#include "grid.hpp"

namespace pvz {

class Zombie : public Entity {
public:
    enum class ArmorType {
        NONE,
        CONE,
        BUCKET
    };

    Zombie(
        GridPosition initial_position,
        int initial_health,
        int initial_damage,
        float initial_speed,
        ArmorType initial_armor = ArmorType::NONE,
        int initial_armor_health = 0
    );

    EntityType get_type() const override;
    void update(double delta_seconds) override;

    int get_damage() const;
    float get_speed() const;

    ArmorType get_armor_type() const;
    int get_armor_health() const;
    bool has_armor() const;

    void take_damage(int amount) override;

    // GameController gọi ngay khi spawn Zombie,
    // để Zombie có thể tự hỏi Grid trong update() của chính nó.
    void set_grid(Grid* grid_ref);

protected:
    // Logic di chuyển/tấn công dùng chung cho Zombie.
    void default_update(double delta_seconds);

    int damage;
    float speed;
    Grid* grid = nullptr;

private:
    double attack_cooldown = 0.0;
    static constexpr double ATTACK_INTERVAL = 1.0;

    float column_position = 0.0f;

    ArmorType armor_type;
    int armor_health;
};

} // namespace pvz