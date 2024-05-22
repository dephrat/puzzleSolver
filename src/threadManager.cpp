#include "threadManager.hpp"

//might want to use helper functions. Validate, Create ThreadArgs, Create threads, Combine solutions, return
//storeSolutionsInPuzzleSolver: determines whether we want to store the solutions in PuzzleSolver's static solutions vector
bool ThreadManager::createSolverThreads(const std::vector<std::vector<char>> &grid, const std::vector<Piece>& pieces, int depth, int numThreads) {
    //Validation
    if (numSquares < 0)
        throw std::runtime_error("Error: Somehow, the number of squares in the grid is negative");
    if (numThreads < 1)
        throw std::runtime_error("Error: Tried to solve the puzzle with less than 1 thread");
    //edge case, list of 0x0 solutions is just the grid itself. This check should be irrelevant since I'm just using 10x10 grid for now, but maybe becomes relevant in the future if I decide to add more grid sizes.
    if (numSquares == 0)
        return true;
    //remove excess threads
    if (numThreads > numSquares)
        numThreads = numSquares;

    //Create thread arguments
    std::vector<ThreadManager::ThreadArgs> thread_args;

    //split up thread starts/ends based on how many squares they should cover
    //threads 0 to remainder-1 will cover squaresPerThread + 1 squares
    //the remaining threads will cover squaresPerThread squares
    int squaresPerThread = numSquares / numThreads;
    int remainder = numSquares - squaresPerThread * numThreads;
    //I did some math to confirm this works for non-negative numSquares and positive numThreads.
    //This works for non-negative numSquares and positive numThreads. 
    //  I leave the proof as an exercise for the reader.
    
    std::vector<PuzzleSolver> solvers(numThreads, PuzzleSolver(grid));
    std::vector<int> finished(numThreads, 0);
    int start = 0;
    for (int i = 0; i < numThreads; ++i) {
        int end = start + squaresPerThread - 1; // squaresPerThread squares
        if (i < remainder) end++; // squaresPerThread + 1 squares
        thread_args.emplace_back(pieces, solvers[i], depth, start, end, finished[i]);
        start = end + 1;
    }
    if (start != numSquares)
        throw std::runtime_error("Error: While creating thread arguments, we either don't assign all squares to threads or assign too many. Possible source of error is my math.");

    //Create threads, give them PuzzleSolver's thread_recursiveSolver method
    std::vector<pthread_t> threads(numThreads);

    for (int i = 0; i < numThreads; ++i)
        pthread_create(&(threads[i]), NULL, &thread_startup, static_cast<void*>(&thread_args[i]));    
    for (int i = 0; i < numThreads; ++i)
        pthread_join(threads[i], NULL);
    //if any of the threads say they're finished, then we got enough solutions and we can stop 
    for (auto thread_arg : thread_args) {
        if (thread_arg.finished == 1) {
            pthread_mutex_lock(&mutex);
            bool throwError = PuzzleSolver::getSolutionsFound() < PuzzleSolver::numSolutions;
            pthread_mutex_unlock(&mutex);
            if (throwError)
                throw std::runtime_error("Error: Thread said we're finished but we haven't found enough solutions");
            return true;
        }
    }
    return false; //otherwise, we need to keep searching
}

void *ThreadManager::thread_startup(void *args) {
    ThreadArgs *x = (ThreadArgs*)args;
    auto &solver = x->solver;
    auto &pieces = x->pieces;
    int depth = x->depth;
    auto start = x->start;
    auto end = x->end;
    int &finished = x->finished;
    
    solver.thread_recursiveSolver(pieces, depth, start, end, finished);
    
    pthread_exit(NULL);
    return NULL;
}