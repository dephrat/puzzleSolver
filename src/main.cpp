#include <chrono>
#include <iostream>
#include <utility>
#include <vector>
#include "global.hpp"
#include "piece.hpp"
#include "pieceDefns.hpp"
#include "puzzleDisplay.hpp"
#include "puzzleSolver.hpp"

/*
void removeFromGrid(std::vector<std::vector<char>> &grid, const std::vector<std::vector<bool>> &orientation, 
int row, int col, char symbol, bool allowPartial = false) {
    if (allowPartial) {
        //in this case, it's okay if only part of the orientation has been placed in the grid
        for (int r = -2; r < 3; ++r) {
            for (int c = -2; c < 3; ++c) {
                //if out of bounds, continue
                if (row+r < 0 || row+r >= grid.size() || col+c < 0 || col+c >= grid[0].size()) continue;
                //else if we find our symbol, remove it
                if (grid[row + r][col + c] == symbol) {
                    if (orientation[r+2][c+2]) grid[row + r][col + c] = '-';
                    else throw std::runtime_error("Error: Detected unexpected occurrence of current symbol, possibly a repeat usage in piece definitions (removeFromGrid, 1)");
                }
            }         
        }        
    } else {
        //in this case, the whole orientation must be found in the grid
        size_t m = orientation.size();
        size_t n = orientation[0].size();
        for (int r = -2; r < 3; ++r) {     //design these loops so that
            for (int c = -2; c < 3; ++c) { //we're always within orientation bounds!
                assert(r+2 < m && c+2 < n);
                bool inGridBounds = row+r >= 0 && row+r < gridHeight && col+c >= 0 && col+c < gridWidth;
                bool inOrientation = orientation[r+2][c+2];
                if (inGridBounds) {
                    if (grid[row + r][col + c] == symbol) {
                        if (inOrientation) grid[row + r][col + c] = '-';
                        else throw std::runtime_error("Error: Detected unexpected occurrence of current symbol, possibly a repeat usage in piece definitions (removeFromGrid, 2)");
                    } else if (inOrientation) {
                        throw std::runtime_error("Error: Unable to find piece square during full removal (removeFromGrid)");
                    }
                } else if (inOrientation) {
                    throw std::runtime_error("Error: Tried full removal when part of the full piece would be out of bounds (removeFromGrid)");
                }                
            }
        }
    }
}

//Fit the piece in the grid at the specified spot if possible, returns false otherwise.
//Currently inserts the piece by aligning the center of the 5x5 grid with the specified location. May need updates if I allow larger/unique pieces.
bool fitInGrid(std::vector<std::vector<char>>& grid, const std::vector<std::vector<bool>> &orientation, 
int row, int col, char symbol) {
    //go from -2 to +2 relative to center. This will have to be adjusted if we allow for more variety in piece sizes.
    //(-2, -2) is (currently) the posn of the top left corner of the orientation grid
    for (int r = -2; r < 3; ++r) {
        for (int c = -2; c < 3; ++c) {
            if (orientation[r+2][c+2] == false) continue;            
            if (row+r < 0 || row+r >= grid.size() || col+c < 0 || col+c >= grid[0].size()) {
                removeFromGrid(grid, orientation, row, col, symbol, true);
                return false;
            }
            char current = grid[row + r][col + c];
            if (current == symbol) {
                throw std::runtime_error("Error: Detected unexpected occurrence of current symbol, possibly a repeat usage in piece definitions (fitInGrid)"); 
            } else if (current == '-') {
                grid[row + r][col + c] = symbol;
            } else {
                removeFromGrid(grid, orientation, row, col, symbol, true);
                return false;
            }
        }        
    }
    return true;
}

//For now, this is the recursive function that handles all the execution. 
//I may use this to create threads and move the recursive solving to another function later.
// pieces: vector of all pieces to be fit in
// depth: index of the piece that that iteration is trying to place
//Requirement: depth must be positive
bool startup(const std::vector<Piece>& pieces, std::vector<std::vector<char>>& grid, const int depth) {
if (depth == 8 || depth < 4) std::cout << depth << "\n";
    assert(depth >= 0);
    if (depth >= pieces.size()) return true;
    //for each orientation of the current piece
    for (auto orientation : pieces[depth].orientations) {
        //for each square in the grid (should be accessible across recursive iterations, same grid)
        for (int row = 0; row < gridHeight; ++row) {
            for (int col = 0; col < gridWidth; ++col) {
                //try to fit the current piece
                bool pieceFits = fitInGrid(grid, orientation, row, col, pieces[depth].symbol);
                if (pieceFits) {
                    //recurse with the next piece
                    bool result = startup(pieces, grid, depth + 1);
                    if (result) {
                        return true;
                    } else {
                        removeFromGrid(grid, orientation, row, col, pieces[depth].symbol);
                    }
                    
                }
            }
        }
    }
    return false;
}

*/

//Setup Pieces, create PuzzleSolver, run startup, display results
int main() {

    PuzzleSolver solver('.');

    //When picking the symbols, don't use '-'. '-' is the default used for empty squares.
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
    //    {orangePiece};
        {orangePiece, cyanPiece, bluePiece, pinkPiece, yellowPiece, greenPiece, limePiece, redPiece, purplePiece};

    try {
        using Time = std::chrono::steady_clock;
        const auto start = Time::now();
        bool result = solver.recursiveStartup(pieces, 0);
        const auto end = Time::now();
        std::chrono::duration<double, std::milli> fp_ms = end - start;
        std::cout << (result ? "Success!" : "Failure.") << std::endl;
        std::cout << "Time taken (s): " << fp_ms.count()/1000 << std::endl;
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << std::endl;
    }

    PuzzleDisplay::displayGrid(solver.getGrid());
}

