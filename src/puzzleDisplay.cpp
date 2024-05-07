#include "puzzleDisplay.hpp"

void PuzzleDisplay::displayGrid(const std::vector<std::vector<char>> &grid, const int numNewlines) {
    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            std::cout << grid[row][col] << " ";
        }
        std::cout << "\n";
    }
    std::string newlines = "";
    for (int i = 0; i < numNewlines; ++i) {
        newlines += "\n";
    } 
    std::cout << newlines;
}

void PuzzleDisplay::displayOrientation(const std::vector<std::vector<bool>> &orientation) {
    for (size_t row = 0; row < orientation.size(); ++row) {
        for (size_t col = 0; col < orientation[0].size(); ++col) {
            std::cout << orientation[row][col] << " ";
        }
        std::cout << "\n";
    }
}