#pragma once

#include <godot_cpp/classes/node.hpp>

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

protected:
    static void _bind_methods();

public:
    void _ready() override;

    void start_game();
    void pause_game();
    void resume_game();
    void finish_game(bool won);

    int get_state() const;
};

} // namespace godot