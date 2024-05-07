#pragma once

#include <iostream> //for debugging
#include <vector>
#include "global.hpp"
#include "piece.hpp"
#include "puzzleDisplay.hpp"

class PuzzleSolver {
    std::vector<std::vector<char>> grid;
    char emptySymbol;
    static inline int solutionsFound = 0;
    //maybe include a data structure to hold all the found solutions

    bool fitInGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol);
    void removeFromGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol, 
    const bool allowPartial = false);

    public:
    bool nonRecursiveSolver(const std::vector<Piece>& pieces);
    bool recursiveSolver(const std::vector<Piece>& pieces, const int depth, int numSolutions = 1, bool displaySolutions = true);

    PuzzleSolver(const char emptySymbol);
};