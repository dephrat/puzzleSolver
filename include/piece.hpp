#pragma once

#include <vector>

struct Piece {
    std::vector<std::vector<std::vector<bool>>> orientations;
    char symbol; //determines how the piece's squares shows up in the display

    //I might need to set removeDuplicates to be true by default in the declaration
    Piece(const std::vector<std::vector<bool>>& pieceDefn, char, bool removeDuplicates = true); 
    void removeDuplicateOrientations();
    void displayPiece(int orientationIndex = -1);

    private:
    void rotate(std::vector<std::vector<bool>>& pieceGrid);
    void flip(std::vector<std::vector<bool>>& pieceGrid);
    void displayOrientation(int);
};