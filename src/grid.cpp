#include "grid.hpp"

namespace pvz {

bool Grid::is_valid(GridPosition position) const {
    return position.row >= 0 && position.row < ROWS
        && position.column >= 0 && position.column < COLUMNS;
}

bool Grid::is_occupied(GridPosition position) const {
    return is_valid(position)
        && occupied_cells[position.row][position.column];
}

bool Grid::occupy(GridPosition position) {
    if (!is_valid(position) || is_occupied(position)) {
        return false;
    }

    occupied_cells[position.row][position.column] = true;
    return true;
}

void Grid::release(GridPosition position) {
    if (is_valid(position)) {
        occupied_cells[position.row][position.column] = false;
    }
}

void Grid::clear() {
    for (auto &row : occupied_cells) {
        row.fill(false);
    }
}

} // namespace pvz