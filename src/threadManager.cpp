#include "threadManager.hpp"


/*pthread_mutex_t myMutex;
    pthread_mutex_init(&myMutex,0);
        for (int i = 0; i < numThreads; ++i) {
            pthread_create(&(threads[i]), NULL, &thread_startup, (void*)(&(my_args[i])));
        }
        for (int i = 0; i < numThreads; ++i) {
            pthread_join(threads[i], NULL);
        }
        pthread_mutex_destroy(&myMutex);*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

std::vector<std::vector<std::vector<char>>> ThreadManager::createSolverThreads(const std::vector<Piece>& pieces, int numThreads, const bool returnSolutions, const char solverEmptySymbol) {
    std::vector<std::vector<std::vector<char>>> solutions;
    
    if (numThreads < 1)
        throw std::runtime_error("Error: Tried to solve the puzzle with less than 1 thread");

    int numSquares = gridHeight * gridWidth;
    if (numSquares < 0)
        throw std::runtime_error("Error: Somehow, the number of squares in the grid is negative");

    //If the grid has size 0x0, then just return no solutions. 
    //  (Technically if the pieces have no squares then they would "fit", but that's stupid)
    if (numSquares == 0)
        return solutions;

    //Each thread must cover at least one square, so this removes excessive threads
    if (numThreads > numSquares)
        numThreads = numSquares;

    //Either every thread can cover the same number of squares,
    // or the first few(?) threads cover 1 extra square. In either case, exactly numSquares squares are covered.
    int squaresPerThread = numSquares / numThreads;
    int remainder = numSquares - squaresPerThread * numThreads;
    //the threads 0 to remainder-1 will cover squaresPerThread + 1 squares
    //the remaining threads will cover squaresPerThread squares
    //I did some math to confirm this works for non-negative numSquares and positive numThreads.
    //I leave the proof as an exercise for the reader.


    //Create thread arguments
    std::vector<ThreadArgs> thread_args;
    std::vector<PuzzleSolver> solvers(numThreads, '.');
    
    int start = 0;
    for (int i = 0; i < numThreads; ++i) {
        int end = start + squaresPerThread - 1; // squaresPerThread squares
        if (i < remainder) end++; // squaresPerThread + 1 squares
        thread_args.emplace_back(solvers[i], start, end);
        start = end + 1;
    }
    if (start != numSquares)
        throw std::runtime_error("Error: While creating thread arguments, we either don't assign all squares to threads or assign too many. Possible source of error is my math.");

    //create threads, give them PuzzleSolver's thread_recursiveSolver method
    std::vector<pthread_t> threads(numThreads);

    pthread_mutex_t myMutex;
    pthread_mutex_init(&myMutex, 0);
    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&(threads[i]), NULL, &thread_startup, (void*)(&(thread_args[i])));
    }
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&myMutex);




    //return solutions
    if (returnSolutions) {
        //combine all the stuff into one vector and return it
    } else {
        //return some default small vector
    }
    
}

void *ThreadManager::thread_startup(void *args) {
    using Time = std::chrono::steady_clock;
    const auto timer_start = Time::now();

    ThreadArgs *x = (ThreadArgs*)args;
    auto solver = x->solver;
    auto pieces = x->pieces;
    auto start = x->start;
    auto end = x->end;
    bool res = solver.thread_recursiveSolver(pieces, 0, start, end);

    const auto timer_end = Time::now();

    pthread_mutex_lock(&myMutex);

    PuzzleDisplay::displayGrid(solver.getGrid());
    std::chrono::duration<double, std::milli> fp_ms = timer_end - timer_start;
    std::cout << "Thread time taken (s): " << fp_ms.count()/1000 << std::endl;
    std::cout << std::endl;

    pthread_mutex_unlock(&myMutex);
    pthread_exit(NULL);
    return NULL;
}