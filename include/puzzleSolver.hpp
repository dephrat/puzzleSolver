#pragma once

#include <iostream> //for debugging
#include <vector>
#include "global.hpp"
#include "piece.hpp"
#include "puzzleDisplay.hpp"
#include "threadManager.hpp"

class PuzzleSolver {
    std::vector<std::vector<char>> grid;
    char emptySymbol;
    static inline int solutionsFound = 0;
    //maybe include a data structure to hold all the found solutions
    std::vector<std::vector<std::vector<char>>> solutions;

    bool fitInGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol);
    void removeFromGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol, 
    const bool allowPartial = false);

    public:
    static inline int numSolutions = 1;

    bool nonRecursiveSolver(const std::vector<Piece>& pieces);
    bool recursiveSolver(const std::vector<Piece>& pieces, const int depth,
        const bool displaySolutions = true, const bool storeSolutions = true);
    bool thread_recursiveSolver(const std::vector<Piece>& pieces, const int depth, const int start, 
        const int end, const bool displaySolutions = true, const bool storeSolutions = true);
    std::vector<std::vector<std::vector<char>>> &getSolutions() { return solutions; }

    PuzzleSolver(const char emptySymbol);
};