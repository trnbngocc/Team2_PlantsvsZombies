#pragma once

#include <godot_cpp/classes/node.hpp>

namespace godot {

class GameController : public Node {
    GDCLASS(GameController, Node)

private:
    int sun_points = 50;

protected:
    static void _bind_methods();

public:
    void _ready() override;

    int get_sun_points() const;
    void add_sun_points(int amount);
};

} // namespace godot