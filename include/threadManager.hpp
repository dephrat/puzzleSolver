#pragma once

#include <pthread.h>
#include <vector>
#include "piece.hpp"

class ThreadManager {
    class ThreadArgs {

    };

    public:
    std::vector<std::vector<std::vector<char>>> createSolverThreads(const std::vector<Piece>& pieces, int numThreads, bool returnSolutions = true);
};