/*
    File: The Conway's Game of Life Sequential Implementation
    ----------------------------------------------------------
    Author: Cristóbal Alcázar; Christopher Stears
    Date: 2023-05-12
    Version: 1.0
*/
#include "include/grid.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <time.h>
#include <chrono>
#include "include/utils.hpp"

using namespace std;

/* Function prototypes */
bool isAlive(Grid<int> &grid, int x, int y);
int countNeighbours(Grid<int> &grid, int x, int y);

/* Main Program */
int main() {

    // Inicializador - Configuracion de parámetros del juego
    initGameConfig();

    Grid<int> board;

    // initialize a grid of size NROWS x NCOLS
    board.resize(NROWS, NCOLS);

    if (filename == "RANDOM") {
        // Fill with random cells alive;
        board.populateRandom(0.5);
    } else if (filename.find(".txt") != std::string::npos) {
        board.initializeFromFile(filename);
    } else {
        std::cout << "Invalid input. Exiting..." << std::endl;
        return 0;
    }

    std::cout << "Welcome to the game of life. We will play " << NITER << " generations...and the universe will be a grid of size " << NROWS << "x" << NCOLS << std::endl;
    std::cout << "Here is your initial board configuration:\n";
    std::cout << board.toString2D() << std::endl;

    // Calculate the time it takes the simulation
    double itime = 0.0;
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < NITER; i++) {

        // Print the board if PRETTYPRINT is true
        if (PRETTYPRINT) {
            std::cout << "\nGeneration " << i + 1 << ":" << std::endl;
            std::cout << board.toString2D() << std::endl;
        }

        // Ininitialize a vector of tuples to keep track of the cells alive for
        // the next generation
        std::vector<std::tuple<int, int>> nextGen;

        // Iterate over the grid check if the cell is alive or not
        // and count the number of neighbors
        for (int j = 0; j < NROWS; j++) {
            for (int k = 0; k < NCOLS; k++) {
                int liveNeighbours = countNeighbours(board, j, k);

                if (isAlive(board, j, k)) {
                    // check if the cell survives
                    if (liveNeighbours == 2 || liveNeighbours == 3) {
                        nextGen.emplace_back(j, k);
                    }
                } else {
                    // check if the cell is born
                    if (liveNeighbours == 3) {
                        nextGen.emplace_back(j, k);
                    } 
                }
            }
        }
        // Clear the grid and set the next generation
        board.clear();

        for (auto &cell : nextGen) {
            board.set(std::get<0>(cell), std::get<1>(cell), 1);
        }
    }
    auto end = std::chrono::steady_clock::now();
    itime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "---------------------------------------" << std::endl;
    // print the total time in seconds
    std::cout << "Total time: " << itime / 1000000000.0 << " seconds" << std::endl;

    return 0;
}

/* 
 * Function: isAlive
 * -----------------------------------------------------------------------------
 * Returns true if the cell at (x, y) is alive, false otherwise.
 * A cell is alive, it survives to the next generation if it has 2 or 3 live
 * neighbors; otherwise, it dies.
 * 
 */
bool isAlive(Grid<int> &grid, int x, int y) {
    if (grid.get(x, y) == 1) {
        return true;
    }
    return false;
}

/*
 * Function: countNeighbours
 * -----------------------------------------------------------------------------
 * Returns the number of live neighbors of the cell at (x, y).
*/
int countNeighbours(Grid<int>& grid, int x, int y) {
    int count = 0;
    int nRows = grid.numRows();
    int nCols = grid.numCols();

    // Check top
    if (grid.get((x + nRows - 1) % nRows, y) == 1) {
        count++;
    }

    // Check bottom
    if (grid.get((x + 1) % nRows, y) == 1) {
        count++;
    }

    // Check left
    if (grid.get(x, (y + nCols - 1) % nCols) == 1) {
        count++;
    }

    // Check right
    if (grid.get(x, (y + 1) % nCols) == 1) {
        count++;
    }

    // Check top-left
    if (grid.get((x + nRows - 1) % nRows, (y + nCols - 1) % nCols) == 1) {
        count++;
    }

    // Check top-right
    if (grid.get((x + nRows - 1) % nRows, (y + 1) % nCols) == 1) {
        count++;
    }

    // Check bottom-left
    if (grid.get((x + 1) % nRows, (y + nCols - 1) % nCols) == 1) {
        count++;
    }

    // Check bottom-right
    if (grid.get((x + 1) % nRows, (y + 1) % nCols) == 1) {
        count++;
    }

    return count;
}