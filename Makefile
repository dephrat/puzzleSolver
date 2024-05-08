CXX=g++
CXXFLAGS=-Iinclude -Wall -std=c++17 -g

OBJ_DIR=build

all: $(OBJ_DIR) puzzleSolver

puzzleSolver: $(OBJ_DIR)/main.o $(OBJ_DIR)/piece.o $(OBJ_DIR)/pieceDefns.o $(OBJ_DIR)/puzzleDisplay.o $(OBJ_DIR)/puzzleSolver.o $(OBJ_DIR)/threadManager.o
	$(CXX) $(CXXFLAGS) -o puzzleSolver $(OBJ_DIR)/main.o $(OBJ_DIR)/piece.o $(OBJ_DIR)/pieceDefns.o $(OBJ_DIR)/puzzleDisplay.o $(OBJ_DIR)/puzzleSolver.o $(OBJ_DIR)/threadManager.o 

$(OBJ_DIR)/main.o: src/main.cpp include/global.hpp include/piece.hpp include/pieceDefns.hpp include/puzzleDisplay.hpp include/puzzleSolver.hpp include/threadManager.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/piece.o: include/piece.hpp src/piece.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c src/piece.cpp -o $(OBJ_DIR)/piece.o

$(OBJ_DIR)/pieceDefns.o: include/pieceDefns.hpp src/pieceDefns.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c src/pieceDefns.cpp -o $(OBJ_DIR)/pieceDefns.o

$(OBJ_DIR)/puzzleDisplay.o: include/puzzleDisplay.hpp src/puzzleDisplay.cpp include/global.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c src/puzzleDisplay.cpp -o $(OBJ_DIR)/puzzleDisplay.o

$(OBJ_DIR)/puzzleSolver.o: include/puzzleSolver.hpp src/puzzleSolver.cpp include/global.hpp include/piece.hpp include/puzzleDisplay.hpp include/threadManager.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c src/puzzleSolver.cpp -o $(OBJ_DIR)/puzzleSolver.o

$(OBJ_DIR)/threadManager.o: include/threadManager.hpp src/threadManager.cpp include/piece.hpp include/puzzleSolver.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c src/threadManager.cpp -o $(OBJ_DIR)/threadManager.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.gch puzzleSolver
