#include <array>
#include <iostream>
#include "puzzleDisplay.hpp"

void PuzzleDisplay::displayGrid(std::array<std::array<char, gridWidth>, gridHeight> &grid) {
    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            std::cout << grid[row][col] << " ";
        }
        std::cout << "\n";
    }
}