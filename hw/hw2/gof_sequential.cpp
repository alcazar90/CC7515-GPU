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

using namespace std;

/* Function prototypes */
bool isAlive(Grid<int> &grid, int x, int y);
int countNeighbours(Grid<int> &grid, int x, int y);

/* Main Program */
int main() {
    int NITER;
    int NROWS;
    int NCOLS;
    std::cout << "Please enter the number of iterations: ";
    std::cin >> NITER;

    std::cout << "Now we will be set the grid size..." << std::endl;
    std::cout << "Please enter the number of rows: " << std::endl;
    std::cin >> NROWS;
    std::cout << "Please enter the number of columns: " << std::endl;
    std::cin >> NCOLS;

    std::cout << "Welcome to the game of life. We will play " << NITER << " generations...and the universe will be a grid of size " << NROWS << "x" << NCOLS << std::endl;

    // initialize a grid of size NROWS x NCOLS
    Grid<int> board(NROWS, NCOLS);

    // Fill with random cells alive;
    board.populateRandom(0.5);

    for (int i = 0; i < NITER; i++) {
        std::cout << "Generation " << i << "\n" << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::cout << board.toString2D() << std::endl;
        // Ininitialize a vector of tuples to keep track of the cells alive for
        // the next generation
        std::vector<std::tuple<int, int>> nextGen;

        // Iterate over the grid check if the cell is alive or not
        // and count the number of neighbors
        for (int j = 0; j < NROWS; j++) {
            for (int k = 0; k < NCOLS; k++) {
                if (isAlive(board, j, k)) {
                    // check if the cell survives
                    if (countNeighbours(board, j, k) >= 2) {
                        // survives
                        nextGen.emplace_back(j, k);
                    }
                } else {
                    // check if the cell is born
                    if (countNeighbours(board, j, k) == 3) {
                        // born
                        nextGen.emplace_back(j, k);
                    } 
                }
            }
        }
        // clear grid and set the next generation
        board.clear();
        for (auto &cell : nextGen) {
            board.set(std::get<0>(cell), std::get<1>(cell), 1);
        }
    }
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
int countNeighbours(Grid<int> &grid, int x, int y) {
    int count = 0;

    // check top
    if (x > 0 && grid.get(x - 1, y) == 1) {
        count++;
    }

    // check bottom
    if (x < grid.numRows() - 1 && grid.get(x + 1, y) == 1) {
        count++;
    }

    // check left
    if (y > 0 && grid.get(x, y - 1) == 1) {
        count++;
    }

    // check right
    if (y < grid.numCols() - 1 && grid.get(x, y + 1) == 1) {
        count++;
    }

    return count;
}
