#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cstddef>
#include <cassert>
#include "include/utils.hpp"

// Function prototypes
typedef unsigned char ubyte;
std::string toString2D(const ubyte* grid, int nRows, int nCols);
void initializeBoardFromFile(ubyte* grid, int nRows, int nCols, const std::string& filename);
void initializeBoardRandom(ubyte* grid, int nRows, int nCols);


__global__ void gameOfLifeKernel(const ubyte* grid, int nRows, int nCols, 
                                 ubyte* result) {
    // Base on the following implementation.
    // Source: http://www.marekfiser.com/Projects/Conways-Game-of-Life-on-GPU-using-CUDA/2-Basic-implementation#code-listing-2
    uint gridSize = nRows * nCols;

    for (uint cellId = (blockIdx.x * blockDim.x) + threadIdx.x;
         cellId < gridSize;
         cellId += blockDim.x * gridDim.x) {
            uint x = cellId % nCols;
            uint yAbs = cellId - x;
            uint xLeft = (x + nCols - 1) % nCols;
            uint xRight = (x + 1) % nCols;
            uint yAbsUp = (yAbs + gridSize - nCols) % gridSize;
            uint yAbsDown = (yAbs + nCols) % gridSize;

            // Shrink count neighbors using if statements
            uint aliveCells = grid[xLeft + yAbsUp] + grid[x + yAbsUp]
                + grid[xRight + yAbsUp] + grid[xLeft + yAbs] + grid[xRight + yAbs]
                + grid[xLeft + yAbsDown] + grid[x + yAbsDown] + grid[xRight + yAbsDown];
            
            result[x + yAbs] =
                aliveCells == 3 || (aliveCells == 2 && grid[x + yAbs]) ? 1 : 0;
         }
}


/* Main Program */
int main() {

    // Inicializador - Configuracion de parámetros del juego
    initGameConfig();

    // Allocate memory for the game grid
    uint gridSize = NROWS * NCOLS;
    ubyte* grid;
    ubyte* resultGrid;
    cudaMallocManaged(&grid, gridSize * sizeof(ubyte));
    cudaMallocManaged(&resultGrid, gridSize * sizeof(ubyte));

    if (filename == "RANDOM") {
        initializeBoardRandom(grid, NROWS, NCOLS);
    } else if (filename.find(".txt") != std::string::npos) {
        initializeBoardFromFile(grid, NROWS, NCOLS, filename);
    } else {
        std::cout << "Invalid input. Exiting..." << std::endl;
        return 0;
    }

    std::cout << "Welcome to the game of life. We will play " << NITER << " generations...and the universe will be a grid of size " << NROWS << "x" << NCOLS << std::endl;
    std::cout << "Here is your initial board configuration:\n";
    std::cout << toString2D(grid, NROWS, NCOLS) << std::endl;

    // CUDA config
    ushort threadsCount = NCOLS * NROWS;
    assert((NCOLS * NROWS) % threadsCount == 0);
    size_t reqBlocksCount = (NCOLS * NROWS) / threadsCount;
    ushort blocksCount = (ushort)std::min((size_t)32768, reqBlocksCount);

    // Calculate the time it takes for the simulation
    double itime = 0.0;
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < NITER; ++i) {
        // Print the grid
        if (PRETTYPRINT) {
            std::cout << "Generation " << (i + 1) << ":" << std::endl;
            std::cout << toString2D(grid, NROWS, NCOLS) << std::endl;
        }

        // Launch the kernel
        gameOfLifeKernel<<<blocksCount, threadsCount>>>(grid, NROWS, NCOLS, resultGrid);

        // Wait for GPU to finish before processing the following generation
        cudaDeviceSynchronize();

        // Swap values of two objects efficiently
        std::swap(grid, resultGrid);

    }

    auto end = std::chrono::steady_clock::now();
    itime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    // Print the total time in seconds
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Total time: " << itime / 1000000000.0 << " seconds" << std::endl;

    return 0;
}

std::string toString2D(const ubyte* grid, int nRows, int nCols) {
    std::string result;
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            int index = i * nCols + j;
            result += (grid[index] == 1) ? "1 " : ". ";
        }
        result += "\n";
    }
    return result;
}

void initializeBoardRandom(ubyte* grid, int nRows, int nCols) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            int index = i * nCols + j;
            grid[index] = dis(gen);
        }
    }
}

void initializeBoardFromFile(ubyte* grid, int nRows, int nCols, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    uint value;
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            if (file >> value) {
                int index = i * nCols + j;
                grid[index] = value;
            } else {
                throw std::runtime_error("Error reading file: " + filename);
            }
        }
    }

    file.close();
}
