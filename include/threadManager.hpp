#pragma once

#include <pthread.h>
#include <unordered_map>
#include <vector>
#include "global.hpp"
#include "piece.hpp"
#include "puzzleSolver.hpp"

class PuzzleSolver;

class ThreadManager {
    struct ThreadArgs {
        std::vector<Piece> pieces;
        PuzzleSolver &solver;
        int depth;
        int start;
        int end;
        int &finished;
        
        ThreadArgs(const std::vector<Piece> &pcs, PuzzleSolver &ps, const int d, const int s, const int e, int &f)
        : pieces(pcs), solver(ps), depth(d), start(s), end(e), finished(f) {}
    };

    static void *thread_startup(void *args);

    public:
    static inline pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    static bool createSolverThreads(const std::vector<std::vector<char>> &grid, const std::vector<Piece>& pieces, const int depth, int numThreads);
};