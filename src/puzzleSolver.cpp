#include "puzzleSolver.hpp"

PuzzleSolver::PuzzleSolver(const char emptySymbol) : emptySymbol(emptySymbol) {
    grid.resize(gridHeight, std::vector<char>(gridWidth, emptySymbol));
}

//Fit the piece in the grid at the specified spot if possible, returns false otherwise.
//Currently inserts the piece by aligning the center of the 5x5 grid with the specified location. May need updates if I allow larger/unique pieces.
bool PuzzleSolver::fitInGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol) {
    //go from -2 to +2 relative to center. This will have to be adjusted if we allow for more variety in piece sizes.
    //(-2, -2) is (currently) the posn of the top left corner of the orientation grid
    for (int r = -2; r < 3; ++r) {
        for (int c = -2; c < 3; ++c) {
            if (orientation[r+2][c+2] == false) continue;            
            if (row+r < 0 || row+r >= grid.size() || col+c < 0 || col+c >= grid[0].size()) {
                removeFromGrid(orientation, row, col, symbol, true);
                return false;
            }
            char current = grid[row + r][col + c];
            if (current == symbol) {
                throw std::runtime_error("Error: Detected unexpected occurrence of current symbol, possibly a repeat usage in piece definitions (fitInGrid)"); 
            } else if (current == emptySymbol) {
                grid[row + r][col + c] = symbol;
            } else {
                removeFromGrid(orientation, row, col, symbol, true);
                return false;
            }
        }        
    }
    return true;
}

void PuzzleSolver::removeFromGrid(const std::vector<std::vector<bool>> &orientation, const int row, const int col, const char symbol, 
const bool allowPartial) {
    if (allowPartial) {
        //in this case, it's okay if only part of the orientation has been placed in the grid
        for (int r = -2; r < 3; ++r) {
            for (int c = -2; c < 3; ++c) {
                //if out of bounds, continue
                if (row+r < 0 || row+r >= grid.size() || col+c < 0 || col+c >= grid[0].size()) continue;
                //else if we find our symbol, remove it
                if (grid[row + r][col + c] == symbol) {
                    if (orientation[r+2][c+2]) grid[row + r][col + c] = emptySymbol;
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
                        if (inOrientation) grid[row + r][col + c] = emptySymbol;
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

//For now, this is the recursive function that handles all the execution. 
//I may use this to create threads and move the recursive solving to another function later.
// pieces: vector of all pieces to be fit in
// depth: index of the piece that that iteration is trying to place
//Requirement: depth must be positive
bool PuzzleSolver::recursiveStartup(const std::vector<Piece>& pieces, const int depth) {
    if (depth == 8 || depth < 4) std::cout << depth << "\n";
    assert(depth >= 0);
    if (depth >= pieces.size()) return true;
    //for each orientation of the current piece
    for (auto orientation : pieces[depth].orientations) {
        //for each square in the grid (should be accessible across recursive iterations, same grid)
        for (int row = 0; row < gridHeight; ++row) {
            for (int col = 0; col < gridWidth; ++col) {
                //try to fit the current piece
                bool pieceFits = fitInGrid(orientation, row, col, pieces[depth].symbol);
                if (pieceFits) {
                    //recurse with the next piece
                    bool result = recursiveStartup(pieces, depth + 1);
                    if (result) {
                        return true;
                    } else {
                        removeFromGrid(orientation, row, col, pieces[depth].symbol);
                    }
                    
                }
            }
        }
    }
    return false;
}

