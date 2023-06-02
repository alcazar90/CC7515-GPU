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
#include <CL/cl2.hpp>
#include <time.h> 
#include <chrono>
#include <cassert>
#include "include/utils.hpp"

using namespace std;

// Function prototypes
std::string toString2D(vector<int> grid, int nRows, int nCols);


int main(int argc, char* argv[]) {

    // Inicializador - Configuracion de parámetros del juego
    initGameConfig(argc, argv);

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

    if(filename == "RANDOM"){
    // Fill with random cells alive;
        board_in->populateRandomOpenCL(0.4);
    } else {
        board_in->initializeBoardFromFile(NROWS, NCOLS, filename);    
    }

    // The kernel source code is created as a string
    const char *kernelSource =
        "__kernel void Kernel(__global const int* input, __global int* output, int nRows, int nCols) {\n"
        "    int worldSize = nRows * nCols;\n"
        "    int cell = get_group_id(0) * get_local_size(0) + get_local_id(0);\n" // blockId * blockSize + threadId 
        "    for (cell; cell < worldSize; cell += get_local_size(0) * get_num_groups(0)) {\n"
        "        int x = cell % nRows;\n"
        "        int y = cell - x;\n"
        "        int xLeft = (x + nRows - 1) % nRows;\n"
        "        int xRight = (x + 1) % nRows;\n"
        "        int yUp = (y + worldSize - nRows) % worldSize;\n"
        "        int yDown = (y + nRows) % worldSize;\n"
        "        int aliveCells = input[xLeft + yUp] + input[x + yUp] + input[xRight + yUp] + input[xLeft + y]\n"
        "            + input[xRight + y] + input[xLeft + yDown] + input[x + yDown] + input[xRight + yDown];\n"
        "        output[x + y] = aliveCells == 3 || (aliveCells == 2 && input[x + y]) ? 1 : 0;\n"
        "    }\n"
        "}\n";

    // The program is created from the source code
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

    // show the CL_DEVICE_MAX_COMPUTE_UNITS of the device
    cl_uint max_compute_units;
    device.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &max_compute_units);
    std::cout << "CL_DEVICE_MAX_COMPUTE_UNITS: " << max_compute_units << std::endl;

    // OpenCL config
    cl::NDRange localWorkSize(THREADS);
    assert((NCOLS * NROWS) % THREADS == 0);
    cl::NDRange globalWorkSize(NROWS * NCOLS);
        
    cl::Event event;
    cl::Event kernelEvent;
    cl::Event copyEvent;
    std::vector<int> grid(0,0);

    // Create an event only for printing the grid
    if(PRETTYPRINT){
        grid = std::vector<int>(NROWS * NCOLS, 0);
    }

    // Create a variable to store the total time   
    double itime = 0.0;
    auto start = std::chrono::steady_clock::now();

    // for (int i = 0; i < NITER; i++) {

    //     // std::cout << "Generation " << i << "\n" << std::endl;

    //     // Execute the kernel
    //     err = board_in->getQueue().enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(NROWS, NCOLS), cl::NullRange, NULL, &kernelEvent);
    //     if (err != CL_SUCCESS) {
    //         std::cerr << "Error executing kernel: " << err << std::endl;
    //         return 1;
    //     }

    //     kernelEvent.wait();

    //     // Copy the data from the buffer of "board_out" to the buffer of "board_in"
    //     err = board_in->getQueue().enqueueCopyBuffer(board_out->getBuffer(), board_in->getBuffer(), 0, 0, sizeof(int) * NROWS * NCOLS);
    //     if (err != CL_SUCCESS) {
    //         std::cerr << "Error copying data from buffer: " << err << std::endl;
    //         return 1;
    //     }


    //     // read the buffer from the device
    //     cl_int err1 = board_out->getQueue().enqueueReadBuffer(board_out->getBuffer(), CL_TRUE, 0, sizeof(int) * NROWS * NCOLS, grid.data(), NULL, &event);
    //     if (err1 != CL_SUCCESS) {
    //         std::cerr << "Error reading buffer: " << err1 << std::endl;
    //         return 1;
    //     }

    //     event.wait();
    //     for (int i = 0; i < NROWS; i++) {
    //         for (int j = 0; j < NCOLS; j++) {
    //             std::cout << grid[i * NCOLS + j] << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << "---------------------------------------" << std::endl;
    // }
    
    for (int i = 0; i < NITER; i++) {

        if(PRETTYPRINT){
            std::cout << "Generation " << i << "\n" << std::endl;
            // read the buffer from the device
            cl_int err1 = board_in->getQueue().enqueueReadBuffer(board_in->getBuffer(), CL_TRUE, 0, sizeof(int) * NROWS * NCOLS, grid.data(), NULL, &event);
            if (err1 != CL_SUCCESS) {
                 std::cerr << "Error reading buffer PRETTYPRINT: " << err1 << std::endl;
                 return 1;
             }

            // Wait for the event to finish
            event.wait();

            // Print the grid
            std::cout << toString2D(grid, NROWS, NCOLS) << std::endl;
        }

        // Execute the kernel
        // Args: kernel, offset, global size (number of work-items), local size (number of work-items per work-group)
        // err = board_in->getQueue().enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(NROWS, NCOLS), cl::NullRange, NULL, &kernelEvent);
        err = board_in->getQueue().enqueueNDRangeKernel(kernel, cl::NullRange, globalWorkSize, localWorkSize, NULL, &kernelEvent);
        if (err != CL_SUCCESS) {
            std::cerr << "Error executing kernel: " << err << std::endl;
            return 1;
        }

        // Wait for the kernel execution to complete
        kernelEvent.wait();

        // Copy the data from the buffer of "board_out" to the buffer of "board_in"
        // err = board_in->getQueue().enqueueCopyBuffer(board_out->getBuffer(), board_in->getBuffer(), 0, 0, sizeof(int) * NROWS * NCOLS);
        err = board_in->getQueue().enqueueCopyBuffer(board_out->getBuffer(), board_in->getBuffer(), 0, 0, sizeof(int) * NROWS * NCOLS, NULL, &copyEvent);
        if (err != CL_SUCCESS) {
            std::cerr << "Error copying data from buffer: " << err << std::endl;
            return 1;
        }

        copyEvent.wait();
    }

    auto end = std::chrono::steady_clock::now();
    itime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "---------------------------------------" << std::endl;
    // print the total time in seconds
    std::cout << itime / 1000000000.0 << std::endl;

    // free memory allocated for board_in and board_out
    delete board_in;
    delete board_out;
    return 0;
}

// create toString2D function
string toString2D(vector<int> grid, int nRows, int nCols) {
    string gridString = "";
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            gridString += grid[i * nCols + j] == 1 ? "1 " : ". ";
        }
        gridString += "\n";
    }
    return gridString;
}