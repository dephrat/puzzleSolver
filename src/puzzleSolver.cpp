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

//This version of the function will return all valid solutions
// pieces: vector of all pieces to be fit in
// depth: index of the piece that that iteration is trying to place
//Requirement: depth must be non-negative
//Note: If numSolutions is negative, this function will return all possible solutions
//Note: Here, the return value indicates whether we've reached our target numSolutions
bool PuzzleSolver::recursiveSolver(const std::vector<Piece>& pieces, const int depth, const int numSolutions, bool displaySolutions) {
    assert(depth >= 0);
    if (numSolutions >= 0 && PuzzleSolver::solutionsFound >= numSolutions) return true;
    
    //if the depth surpasses the number of pieces, then all pieces have been placed and we have a solution
    if (depth >= pieces.size()) {
        ++PuzzleSolver::solutionsFound;
        if (displaySolutions)
            PuzzleDisplay::displayGrid(grid);
        //if we've found enough solutions, then we can stop
        return numSolutions >= 0 && PuzzleSolver::solutionsFound >= numSolutions;
    }

    //for each orientation of the current piece
    for (auto orientation : pieces[depth].orientations) {
        //for each square in the grid (should be accessible across recursive iterations, same grid)
        for (int row = 0; row < gridHeight; ++row) {
            for (int col = 0; col < gridWidth; ++col) {
                //try to fit the current piece
                bool pieceFits = fitInGrid(orientation, row, col, pieces[depth].symbol);
                if (pieceFits) {
                    //recurse with the next piece
                    bool result = recursiveSolver(pieces, depth + 1, numSolutions, displaySolutions);
                    if (result) { //if it was successful, then we've found enough solutions and we can stop
                        return true;
                    } else {
                        removeFromGrid(orientation, row, col, pieces[depth].symbol);
                    }
                }
            }
        }
    }
    //if we reach this, then we haven't found enough valid solutions, so return false
    return false;
}


//Eliminates the need for recursion
bool PuzzleSolver::nonRecursiveSolver(const std::vector<Piece>& pieces) {
    std::vector<std::pair<int, int>> locationTracker(pieces.size(), std::make_pair(0, 0)); //If unplaced, stores the next square to be tried. If placed, this is the square of the piece.
    std::vector<int> orientationTracker(pieces.size(), 0); //Stores which orientation each piece is on
    std::vector<bool> placedTracker(pieces.size(), false); //Stores whether a piece is currently placed.
    int depth = 0;
    //depth will represent the index of the piece we're trying to fit
    while (true) {
        if (depth >= 8 || depth < 3) std::cout << depth << "\n";
        //assume right now that piece[depth] exists 
        //and that orientationTracker[depth] is a valid orientations index

        if (depth >= int(size(pieces))) {
            return true;
        }
        if (depth < 0) {
            return false;
        }
        if (orientationTracker[depth] >= pieces[depth].orientations.size()) {
            throw std::runtime_error("Error: Invalid orientations index, theoretically possible because a piece had no orientations (nonRecursiveSolver)");
        }
        if (locationTracker[depth].first >= gridHeight || locationTracker[depth].second >= gridWidth) {
            throw std::runtime_error("Error: Invalid grid location (nonRecursiveSolver)");
        }

        //try to fit current orientation of piece[depth] using piece[depth].orientations[orientationTracker[depth]]
        auto currentOrientation = pieces[depth].orientations[orientationTracker[depth]];
        int row = locationTracker[depth].first;
        int col = locationTracker[depth].second;

        bool pieceCanGoHere;
        if (placedTracker[depth] == true) {
            removeFromGrid(currentOrientation, row, col, pieces[depth].symbol);
            placedTracker[depth] = false;
            pieceCanGoHere = false;
        } else {
            pieceCanGoHere = fitInGrid(currentOrientation, row, col, pieces[depth].symbol);
        }
        if (pieceCanGoHere) { //if it fits, move to the next piece
            placedTracker[depth] = true;
            depth++;
        } else if (row < gridHeight - 1 || col < gridWidth - 1) { //if there are more spots to try
            //prepare to try the next spot
            if (col == gridWidth - 1) {
                col = 0;
                row++;
            } else {
                col++;
            }
            locationTracker[depth] = std::make_pair(row, col);
        } else if (orientationTracker[depth] < pieces[depth].orientations.size() - 1) { //if there are more orientations to try
            //go back to the start and try the next orientation
            locationTracker[depth] = std::make_pair(0, 0);
            orientationTracker[depth]++;
        } else if (depth > 0) { //if there are earlier pieces to try
            //reset orientation and location for the next time we try this piece
            locationTracker[depth] = std::make_pair(0, 0);
            orientationTracker[depth] = 0;
            //go to the previous piece
            depth--;
        } else {
            return false;
        }
        
        
        
        
    }
}