#pragma once

#include <memory>
#include <vector>

#include <godot_cpp/classes/node.hpp>

#include "entity.hpp"
#include "grid.hpp"

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

protected:
    static void _bind_methods();

public:
    void _ready() override;

    void start_game();
    void pause_game();
    void resume_game();
    void finish_game(bool won);

    void add_entity(std::unique_ptr<pvz::Entity> entity);

    int get_state() const;
};

} // namespace godot