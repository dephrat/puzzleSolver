#pragma once

#include <vector>
#include "global.hpp"

class PuzzleDisplay {
    public:
    static void displayGrid(const std::vector<std::vector<char>> &grid);
    static void displayOrientation(const std::vector<std::vector<bool>> &orientation);
};