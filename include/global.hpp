#pragma once

#include <array>
#include <stddef.h>
#include <vector>

#define MULTITHREADING true
#define NUM_SOLUTIONS 3996300
#define EMPTY_SYMBOL '.'

constexpr size_t gridWidth = 10;
constexpr size_t gridHeight= 10;
constexpr size_t numSquares = gridWidth * gridHeight;
//require at least num pieces + 1 elements, and all elements must be positive
//can reduce to at least num pieces I think, since now we check whether the piece we placed is the last piece so we don't recurse again
//constexpr std::array<int,11> thread_countPerDepth {2,2,2, 3, 3, 3, 2, 1, 1};
//constexpr std::array<int,11> thread_countPerDepth {3,5,3,3,3,3,3,1,1,1,-1};
//constexpr std::array<int,11> thread_countPerDepth {1,1,1,1,1,1,1,1,1, -1};

constexpr std::array<int,11> thread_countPerDepth {1,10,1,1,1,1,1,1,1,1,1};