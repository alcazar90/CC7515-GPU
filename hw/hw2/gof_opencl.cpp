/*
    File: The Conway's Game of Life Implementation with OpenCL
    ----------------------------------------------------------
    Author: Cristóbal Alcázar; Christopher Stears
    Date: 2023-05-12
    Version: 1.0
*/

#include "include/grid_opencl.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <CL/cl2.hpp>
#include <fstream>
#include <time.h> 
#include <chrono>

using namespace std;

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

    // create a grid of size NROWS x NCOLS on the heap
    Grid<int>* board_in = new Grid<int>(NROWS, NCOLS);
    Grid<int>* board_out = new Grid<int>(NROWS, NCOLS);

    // Get the default platform
    cl::Platform platform = cl::Platform::getDefault();

    // Get the first available device on the platform
    cl::Device device = cl::Device::getDefault();

    // Create a context for the device
    cl::Context context(device);

    // Pass the board_in to the GPU
    board_in->toGPU(platform, device, context);

    // Pass the out grid to the GPU
    board_out->toGPU(platform, device, context);

    // Fill with random cells alive;
    board_in->populateRandomOpenCL(0.3);

    // The kernel source code is created as a string
    // The main idea is to count the number of neighbours of each cell
    // | 1 | 2 | 3 |
    // | 4 | X | 5 |
    // | 6 | 7 | 8 |
    // The current cell is X. The neighbours are 1, 2, 3, 4, 5, 6, 7, 8
    const char* kernelSource =
    "__kernel void Kernel(__global const int* input, __global int* output, int nRows, int nCols) {\n"
    " int x = get_global_id(0);\n"
    " int y = get_global_id(1);\n"
    " int idx = x * nCols + y;\n"
    " int neighbours = 0;\n"
    " if(x > 0) {\n"
    "   neighbours += input[idx - nCols];\n"
    "   if(y > 0) {\n"
    "       neighbours += input[idx - nCols - 1];\n"
    "   }\n"
    "   if(y < nCols - 1) {\n"
    "       neighbours += input[idx - nCols + 1];\n"
    "   }\n"
    " }\n"
    " if(x < nRows - 1) {\n"
    "   neighbours += input[idx + nCols];\n"
    "   if(y > 0) {\n"
    "       neighbours += input[idx + nCols - 1];\n"
    "   }\n"
    "   if(y < nCols - 1) {\n"
    "       neighbours += input[idx + nCols + 1];\n"
    "   }\n"
    " }\n"
    " if(y > 0) {\n"
    "   neighbours += input[idx - 1];\n"
    " }\n"
    " if(y < nCols - 1) {\n"
    "   neighbours += input[idx + 1];\n"
    " }\n"
    " if(input[idx] == 1) {\n"
    "   if(neighbours < 2 || neighbours > 3) {\n"
    "       output[idx] = 0;\n"
    "   } else {\n"
    "       output[idx] = 1;\n"
    "   }\n"
    " } else {\n"
    "   if(neighbours == 3) {\n"
    "       output[idx] = 1;\n"
    "   } else {\n"
    "       output[idx] = 0;\n"
    "   }\n"
    " }\n"
    "}\n";

    cl::Program program(board_in->getContext(), kernelSource);
    auto err = program.build("-cl-std=CL1.2");
    if (err != CL_SUCCESS) {
        std::cerr << "Error building program: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(board_in->getDevice()) << std::endl;
        return 1;
    }

    // The kernel is created from the program
    cl::Kernel kernel(program, "Kernel");

    // Set the arguments of the kernel
    err = kernel.setArg(0, board_in->getBuffer());
    err |= kernel.setArg(1, board_out->getBuffer());
    err |= kernel.setArg(2, NROWS);
    err |= kernel.setArg(3, NCOLS);

    if (err != CL_SUCCESS) {
        std::cerr << "Error setting kernel arguments: " << err << std::endl;
        return 1;
    }   

    // Create an event only for printing the grid
    // cl::Event event;
    // std::vector<int> grid(NROWS * NCOLS, 0);

    // Create a variable to store the total time   
    double itime = 0.0;
    auto start = std::chrono::steady_clock::now();
    // Execute the kernel
    for (int i = 0; i < NITER; i++) {

        // std::cout << "Generation " << i << "\n" << std::endl;

        // Execute the kernel
        err = board_in->getQueue().enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(NROWS, NCOLS), cl::NullRange);
        if (err != CL_SUCCESS) {
            std::cerr << "Error executing kernel: " << err << std::endl;
            return 1;
        }

        // Copy the data from the buffer of "board_out" to the buffer of "board_in"
        err = board_in->getQueue().enqueueCopyBuffer(board_out->getBuffer(), board_in->getBuffer(), 0, 0, sizeof(int) * NROWS * NCOLS);
        if (err != CL_SUCCESS) {
            std::cerr << "Error copying data from buffer: " << err << std::endl;
            return 1;
        }


        // // read the buffer from the device
        // cl_int err1 = board_out->getQueue().enqueueReadBuffer(board_out->getBuffer(), CL_TRUE, 0, sizeof(int) * NROWS * NCOLS, grid.data(), NULL, &event);
        // if (err1 != CL_SUCCESS) {
        //     std::cerr << "Error reading buffer: " << err1 << std::endl;
        //     return 1;
        // }

        // event.wait();
        // for (int i = 0; i < NROWS; i++) {
        //     for (int j = 0; j < NCOLS; j++) {
        //         std::cout << grid[i * NCOLS + j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << "---------------------------------------" << std::endl;
    }

    auto end = std::chrono::steady_clock::now();
    itime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "---------------------------------------" << std::endl;
    // print the total time in seconds
    std::cout << "Total time: " << itime / 1000000000.0 << " seconds" << std::endl;

    // free memory allocated for board_in and board_out
    delete board_in;
    delete board_out;
    return 0;
}