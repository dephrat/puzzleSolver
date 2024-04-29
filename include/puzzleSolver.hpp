#pragma once

#include <iostream> //for debugging
#include <vector>
#include "global.hpp"
#include "piece.hpp"

class PuzzleSolver {
    std::vector<std::vector<char>> grid;
    static char defaultSymbol;

    bool fitInGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol);
    void removeFromGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol, 
    const bool allowPartial = false);

    public:
    char getDefaultSymbol() { return defaultSymbol; }
    bool nonRecursiveStartup(const std::vector<Piece>& pieces, const int depth);
    bool recursiveStartup(const std::vector<Piece>& pieces, const int depth);
    
    PuzzleSolver(const char defaultSymbol);
};

char PuzzleSolver::defaultSymbol = 'z';