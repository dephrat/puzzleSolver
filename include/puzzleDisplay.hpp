#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "global.hpp"

class PuzzleDisplay {
    static void displayNewlines(const int numNewlines);

    public:
    static void displayGrid(const std::vector<std::vector<char>> &grid, const int = 1);
    static void displayOrientation(const std::vector<std::vector<bool>> &orientation, const int = 1);
};