#include <chrono>
#include <iostream>
#include <utility>
#include <vector>
#include "global.hpp"
#include "piece.hpp"
#include "pieceDefns.hpp"
#include "puzzleDisplay.hpp"
#include "puzzleSolver.hpp"

//Setup Pieces, create PuzzleSolver, run startup, display results
int main() {
    char emptySymbol = '.';
    PuzzleSolver solver(emptySymbol);

    //When assigning symbols to pieces, don't use the emptySymbol.
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
        {orangePiece, cyanPiece, bluePiece, pinkPiece, yellowPiece, greenPiece, limePiece, redPiece};//, purplePiece};

    //Attempt to solve the puzzle, time the attempt
    try {
        using Time = std::chrono::steady_clock;
        const auto start = Time::now();
        bool result = solver.recursiveSolver(pieces, 0);
        //bool result = solver.nonRecursiveSolver(pieces);
        const auto end = Time::now();
        std::chrono::duration<double, std::milli> fp_ms = end - start;
        std::cout << (result ? "Success!" : "Failure.") << std::endl;
        std::cout << "Time taken (s): " << fp_ms.count()/1000 << std::endl;
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << std::endl;
    }

    //Displays the final board state)
    PuzzleDisplay::displayGrid(solver.getGrid());
}

