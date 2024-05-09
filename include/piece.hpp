#pragma once

#include <vector>
#include "global.hpp"

struct Piece {
    std::vector<std::vector<std::vector<bool>>> orientations;
    char symbol; //determines how the piece's squares shows up in the display

    void removeDuplicateOrientations();
    void displayPiece(int orientationIndex = -1);

    Piece(const std::vector<std::vector<bool>>& pieceDefn, char, bool removeDuplicates = true); 

    private:
    void rotate(std::vector<std::vector<bool>>& pieceGrid);
    void flip(std::vector<std::vector<bool>>& pieceGrid);
    void displayOrientation(int);
};