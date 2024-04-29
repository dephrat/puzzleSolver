#pragma once

#include <array>
#include <iostream> //for debugging
#include <vector>
#include "global.hpp"
#include "piece.hpp"

class PuzzleSolver {
    std::array<std::array<char, gridWidth>, gridHeight> grid;

    bool fitInGrid(const std::array<std::array<bool, 5>, 5> &orientation, const int row, const int col, const char symbol);
    void removeFromGrid(const std::array<std::array<bool, 5>, 5> &orientation, const int row, const int col, const char symbol, 
    const bool allowPartial = false);

    public:
    bool nonRecursiveStartup(const std::vector<Piece>& pieces, const int depth);
    bool recursiveStartup(const std::vector<Piece>& pieces, const int depth);
    
    PuzzleSolver(const char defaultSymbol);
};