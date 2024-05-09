#pragma once

#include <iostream> //for debugging
#include <vector>
#include "global.hpp"
#include "piece.hpp"
#include "puzzleDisplay.hpp"
#include "threadManager.hpp"

class PuzzleSolver {
    std::vector<std::vector<char>> grid;
    static inline int solutionsFound = 0;

    bool fitInGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol);
    void removeFromGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol, 
    const bool allowPartial = false);

    public:
    static inline std::vector<std::vector<std::vector<char>>> solutions;
    static const int numSolutions = NUM_SOLUTIONS;
    
    bool nonRecursiveSolver(const std::vector<Piece>& pieces);
    bool recursiveSolver(const std::vector<Piece>& pieces, const int depth,
        const bool displaySolutions = true, const bool storeSolutions = true);
    void thread_recursiveSolver(const std::vector<Piece>& pieces, const int depth, const int start, 
    const int end, int& finished);
    const std::vector<std::vector<char>> &getGrid() { return grid; }
    static int getSolutionsFound() { return solutionsFound; }

    PuzzleSolver();
    PuzzleSolver(const std::vector<std::vector<char>> &g) : grid(g) {}
};