#pragma once

#include <array>

namespace pvz {

struct GridPosition {
    int row;
    int column;
};

class Grid {
public:
    static constexpr int ROWS = 5;
    static constexpr int COLUMNS = 9;

    bool is_valid(GridPosition position) const;
    bool is_occupied(GridPosition position) const;
    bool occupy(GridPosition position);
    void release(GridPosition position);
    void clear();

private:
    std::array<std::array<bool, COLUMNS>, ROWS> occupied_cells{};
};

} // namespace pvz