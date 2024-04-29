#pragma once

#include "global.hpp"

class PuzzleDisplay {
    public:
    static void displayGrid(std::array<std::array<char, gridWidth>, gridHeight> &grid);
};