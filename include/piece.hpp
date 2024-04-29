#pragma once

#include <array>
#include <vector>

struct Piece {
    std::vector<std::array<std::array<bool, 5>, 5>> orientations;
    char symbol; //determines how the piece's squares shows up in the display

    //I might need to set removeDuplicates to be true by default in the declaration
    Piece(const std::array<std::array<bool, 5>, 5>& pieceDefn, char, bool removeDuplicates = true); 
    void removeDuplicateOrientations();
    void displayPiece(int orientationIndex = -1);

    private:
    void rotate(std::array<std::array<bool, 5>, 5>& pieceGrid);
    void flip(std::array<std::array<bool, 5>, 5>& pieceGrid);
    void displayOrientation(int);
};