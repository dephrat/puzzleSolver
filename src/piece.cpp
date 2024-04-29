#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "piece.hpp"

Piece::Piece(const std::vector<std::vector<bool>>& pieceDefn, char s, bool removeDuplicates) : symbol(s) {
    //Add piece defn to vector
    orientations.push_back(pieceDefn);

    //Generate rotations and flipped rotations, storing each in vector
    std::vector<std::vector<bool>> tempGrid = pieceDefn;
    //rotate three times, add each to orientations
    for (int i = 1; i <= 3; i++) {
        rotate(tempGrid);
        orientations.push_back(tempGrid);
    }

    //flip, add to orientations
    flip(tempGrid);
    orientations.push_back(tempGrid);

    //rotate three times, add each to orientations
    for (int i = 1; i <= 3; i++) {
        rotate(tempGrid);
        orientations.push_back(tempGrid);
    }

    if (removeDuplicates) {
        removeDuplicateOrientations();
    }


}

//Rotate clockwise. Chosen arbitrarily for our purposes. (Other option was counter-clockwise)
void Piece::rotate(std::vector<std::vector<bool>>& pieceGrid) {
    size_t n = pieceGrid.size();
    for (int i = 0; i <= n / 2; ++i) {
        for (int j = i; j <= n-1-i-1; ++j) {
            bool temp = pieceGrid[i][j];
            pieceGrid[i][j] = pieceGrid[n-1-j][i];
            pieceGrid[n-1-j][i] = pieceGrid[n-1-i][n-1-j];
            pieceGrid[n-1-i][n-1-j] = pieceGrid[j][n-1-i];
            pieceGrid[j][n-1-i] = temp;
        }
    }
}

//Flip across a vertical line. Chosen arbitrarily for our purposes. (Other option was across a horizontal line)
void Piece::flip(std::vector<std::vector<bool>>& pieceGrid) {
    size_t n = pieceGrid.size();
    for (int i = 0; i <= n - 1; ++i) { //for each row
        for (int j = 0; j <= n / 2; ++j) { //for each column on the left half of the pieceGrid
            //swap the pieceGrid values
            bool temp = pieceGrid[i][j];
            pieceGrid[i][j] = pieceGrid[i][n-1-j]; //n-1-j is the corresponding index to j on the other side of the square, mirrored across the vertical bisector
            pieceGrid[i][n-1-j] = temp;
        }
    }
}

//Remove duplicates from orientations vector
void Piece::removeDuplicateOrientations() {
    //Edge case: No orientations
    if (orientations.size() == 0) return;

    //Convert orientations into strings. 
    // Note: This will break if the length of an orientation's row exceeds 7! 
    std::vector<std::pair<std::string, int>> orientationStrings;
    for (int i = 0; i < orientations.size(); ++i) {
        std::string s = "";
        for (int j = 0; j < orientations[0].size(); ++j) {
            char c = 0;
            for (int k = 0; k < orientations[0][0].size(); ++k) {
                //this part can break if row length > 7, because char only stores up to 255
                c <<= 1;
                c |= orientations[i][j][k];
            }
            s += c;
        }
        orientationStrings.push_back(make_pair(s, i));
    }

    //Now we perform bucket sort on the strings, find duplicates, and remove them from orientations

    //Radix sort:
    std::vector<std::vector<std::pair<std::string, int>>> buckets(256); //256 possible chars requires 256 buckets
    for (int i = 0; i < orientationStrings[0].first.size(); ++i) { //for each digit
        for (int j = 0; j < orientationStrings.size(); ++j) { //for each string
            buckets[orientationStrings[j].first[i]].push_back(orientationStrings[j]);
        }
        orientationStrings.clear();
        for (auto& bucket : buckets) {
            for (const std::pair<std::string, int> &p : bucket) {
                orientationStrings.push_back(p);
            }
            bucket.clear();
        }
    }

    //Find duplicates and remove them from orientations:

    //mark the duplicates
    std::vector<bool> marked(orientations.size());
    for (int i = 0; i < orientationStrings.size() - 1; ++i) {
        if (orientationStrings[i].first == orientationStrings[i+1].first) {
            marked[orientationStrings[i].second] = true;
        }
    }
    //delete the marked orientations
    auto it = orientations.begin();
    auto markedit = marked.begin();
    while (it != orientations.end()) {
        if (*markedit) {
            orientations.erase(it);
            marked.erase(markedit);
        } else {
            it++;
            markedit++;
        }
    }
}

//Display specified orientations of the Piece

void Piece::displayPiece(int orientationIndex) {
    int start, end; //inclusive bounds
    if (orientationIndex < 0 && orientationIndex >= orientations.size()) {
        start = 0;
        end = orientations.size() - 1;
    } else {
        start = orientationIndex;
        end = orientationIndex;
    }
    for (int i = start; i <= end; ++i) {
        displayOrientation(i);
        std::cout << "\n";
    }
}

//Displays a single specified orientation of the Piece
//Requirement: orientationIndex must be a valid index to orientations. We can assert this because the method is private. 
void Piece::displayOrientation(int orientationIndex) {
    assert(orientationIndex >= 0 && orientationIndex < orientations.size());
    auto orientation = orientations[orientationIndex];
    for (auto row : orientation) {
        for (int col : row) {
            std::cout << (col ? symbol : '0') << " ";
        }
        std::cout << "\n";
    }
}