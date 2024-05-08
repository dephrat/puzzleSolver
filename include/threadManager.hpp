#pragma once

#include <pthread.h>
#include <vector>
#include "piece.hpp"
#include "puzzleSolver.hpp"

class PuzzleSolver;

class ThreadManager {
    struct ThreadArgs {
        std::vector<Piece> pieces;
        PuzzleSolver &solver;
        int start;
        int end;
        
        ThreadArgs(const std::vector<Piece> &pcs, PuzzleSolver &ps, const int s, const int e) : pieces(pcs), solver(ps), start(s), end(e) {}
    };

    static void *thread_startup(void *args);

    public:
    static inline pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    std::vector<std::vector<std::vector<char>>> createSolverThreads(const std::vector<Piece>& pieces, 
    int numThreads, const bool returnSolutions = true, const char solverEmptySymbol = '.');

    ThreadManager() {}
};