#include "threadManager.hpp"

std::vector<std::vector<std::vector<char>>> ThreadManager::createSolverThreads(const std::vector<Piece>& pieces, int numThreads, bool returnSolutions) {
    //create thread arguments using vector<ThreadArgs>

    //create threads, give them PuzzleSolver's thread_recursiveSolver method
    

    //



    //let's say I create threads here and give them thread_recursiveSolver
    //each thread runs thread_recursiveSolver, stores everything in their solver instance
    //i join all the threads, but the solvers still exist? only if I pass them to the thread by reference

    //use the solvers to create the return value if needed




    //each solver instance contains the solutions found by the thread which used it
    //the solver instance only exists within createSolverThreads, so perhaps optionally combine each
    //  thread's solver instance's solutions into one big solver thread and display or return them

    //
    
}