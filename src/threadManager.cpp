#include "threadManager.hpp"

//might want to use helper functions. Validate, Create ThreadArgs, Create threads, Combine solutions, return
std::vector<std::vector<std::vector<char>>> ThreadManager::createSolverThreads(const std::vector<Piece>& pieces, int numThreads, const bool returnSolutions, const char solverEmptySymbol) {
    std::vector<std::vector<std::vector<char>>> solutions;

    //Validation
    int numSquares = gridHeight * gridWidth;
    if (numSquares < 0)
        throw std::runtime_error("Error: Somehow, the number of squares in the grid is negative");
    if (numThreads < 1)
        throw std::runtime_error("Error: Tried to solve the puzzle with less than 1 thread");
    //edge case, 0x0 grid has no solution (ignore possibility of empty pieces)
    if (numSquares == 0)
        return solutions;
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
    std::vector<PuzzleSolver> solvers(numThreads, '.');
    int start = 0;
    for (int i = 0; i < numThreads; ++i) {
        int end = start + squaresPerThread - 1; // squaresPerThread squares
        if (i < remainder) end++; // squaresPerThread + 1 squares
        thread_args.emplace_back(pieces, solvers[i], start, end);
        start = end + 1;
    }
    if (start != numSquares)
        throw std::runtime_error("Error: While creating thread arguments, we either don't assign all squares to threads or assign too many. Possible source of error is my math.");

    //Create threads, give them PuzzleSolver's thread_recursiveSolver method
    std::vector<pthread_t> threads(numThreads);

    pthread_mutex_init(&mutex, 0);
    for (int i = 0; i < numThreads; ++i)
        pthread_create(&(threads[i]), NULL, &thread_startup, static_cast<void*>(&thread_args[i]));
    for (int i = 0; i < numThreads; ++i)
        pthread_join(threads[i], NULL);
    pthread_mutex_destroy(&mutex);

    //return solutions
    if (returnSolutions) {
        //combine all the solutions in solvers into one vector and return it
        for (auto &solver : solvers) {
            auto &solution = solver.getSolutions();
            solutions.insert(solutions.end(), solution.begin(), solution.end());
        }
    }
    return solutions;   
}

void *ThreadManager::thread_startup(void *args) {
    ThreadArgs *x = (ThreadArgs*)args;
    auto &solver = x->solver;
    auto &pieces = x->pieces;
    auto start = x->start;
    auto end = x->end;
    solver.thread_recursiveSolver(pieces, 0, start, end, false);
    pthread_exit(NULL);
    return NULL;
}