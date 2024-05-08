#pragma once

#include <pthread.h>
#include <vector>
#include "piece.hpp"
#include "puzzleSolver.hpp"

class ThreadManager {
    struct ThreadArgs {
        std::vector<Piece> pieces;
        PuzzleSolver &solver;
        int start;
        int end;
        
        ThreadArgs(std::vector<Piece> &pcs, PuzzleSolver &ps, const int s, const int e) : pieces(pcs), solver(ps), start(s), end(e) {}
    };

    void *thread_startup(void *args);

    public:
    std::vector<std::vector<std::vector<char>>> createSolverThreads(const std::vector<Piece>& pieces, 
    int numThreads, const bool returnSolutions = true, const char solverEmptySymbol = '.');
};