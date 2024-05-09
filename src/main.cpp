#include <chrono>
#include <iostream>
#include <utility>
#include <vector>
#include "global.hpp"
#include "piece.hpp"
#include "pieceDefns.hpp"
#include "puzzleDisplay.hpp"
#include "puzzleSolver.hpp"
#include "threadManager.hpp"

//Create PuzzleSolver, setup Pieces, run the solver, display the results
int main() {
    //Create PuzzleSolver
    PuzzleSolver solver;

    //Setup Pieces
    //When assigning symbols to pieces, don't use EMPTY_SYMBOL (found in global.hpp)
    Piece orangePiece(orangePieceDefn, 'o');
    Piece orangePiece2(orangePieceDefn, '2');
    Piece orangePiece3(orangePieceDefn, '3');
    Piece orangePiece4(orangePieceDefn, '4');
    Piece cyanPiece(cyanPieceDefn, 'c');
    Piece bluePiece(bluePieceDefn, 'b');
    Piece pinkPiece(pinkPieceDefn, 'k');
    Piece yellowPiece(yellowPieceDefn, 'y');
    Piece greenPiece(greenPieceDefn, 'g');
    Piece limePiece(limePieceDefn, 'l');
    Piece redPiece(redPieceDefn, 'r');
    Piece purplePiece(purplePieceDefn, 'p');
    std::vector<Piece> pieces = 
        {orangePiece, cyanPiece, bluePiece, pinkPiece, yellowPiece, greenPiece, limePiece, redPiece, purplePiece};

    //Run the solver, time the attempt
    using Time = std::chrono::steady_clock;
    std::chrono::duration<double, std::milli> fp_ms;
    try {
        const auto start = Time::now();
    #if MULTITHREADING
        pthread_mutex_init(&ThreadManager::mutex, 0);
        PuzzleSolver solver;
        ThreadManager::createSolverThreads(solver.getGrid(), pieces, 0, NUM_THREADS);
        pthread_mutex_destroy(&ThreadManager::mutex);
    #else
        solver.recursiveSolver(pieces, 0, false);
        //solutions = solver.getSolutions();
    #endif
        //bool result = solver.nonRecursiveSolver(pieces);
        const auto end = Time::now();
        fp_ms = end - start;
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << std::endl;
    }

    //Display the results
    PuzzleDisplay::displaySolutions(PuzzleSolver::solutions);
    std::cout << PuzzleSolver::solutions.size() << std::endl;
    std::cout << "Time taken (s): " << fp_ms.count()/1000 << std::endl;   
}

