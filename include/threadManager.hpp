#pragma once

#include <pthread.h>
#include <vector>
#include "piece.hpp"
#include "puzzleSolver.hpp"

class ThreadManager {
    class ThreadArgs {
        PuzzleSolver &solver;
        int start;
        int end;
        
        public:
        ThreadArgs(PuzzleSolver &ps, const int s, const int e) : solver(ps), start(s), end(e) {}
    };

    public:
    std::vector<std::vector<std::vector<char>>> createSolverThreads(const std::vector<Piece>& pieces, 
    int numThreads, const bool returnSolutions = true, const char solverEmptySymbol = '.');
};