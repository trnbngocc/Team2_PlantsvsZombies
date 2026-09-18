#include "game_controller.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

void GameController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_sun_points"), &GameController::get_sun_points);
    ClassDB::bind_method(
        D_METHOD("add_sun_points", "amount"),
        &GameController::add_sun_points
    );
}

void GameController::_ready() {
    UtilityFunctions::print(
        "PVZ C++ extension loaded. Initial sun: ",
        sun_points
    );
}

int GameController::get_sun_points() const {
    return sun_points;
}

void GameController::add_sun_points(int amount) {
    sun_points += amount;
}

} // namespace godot