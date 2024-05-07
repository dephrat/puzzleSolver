#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "global.hpp"

class PuzzleDisplay {
    public:
    static void displayGrid(const std::vector<std::vector<char>> &grid, const int numNewlines = 1);
    static void displayOrientation(const std::vector<std::vector<bool>> &orientation);
};