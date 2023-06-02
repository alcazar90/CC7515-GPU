#include "../include/grid_opencl.h"
#include <stdexcept>
#include <CL/cl2.hpp>
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>


using namespace std;

template <typename T>
Grid<T>::Grid() : nRows(0), nCols(0) {
}

template <typename T>
Grid<T>::Grid(int nRows, int nCols)
{
    this->nRows = nRows;
    this->nCols = nCols;
    this->grid = std::vector<T>(nRows * nCols);
}

template <typename T>
Grid<T>::~Grid() {
}

template <typename T>
int Grid<T>::numRows() const {
    return nRows;
}

template <typename T>
int Grid<T>::numCols() const {
    return nCols;
}

template <typename T>
T& Grid<T>::operator[](int y) {
    return grid.at(y);
}

// ----------------------------------------------------------
// OpenCL methods
// ----------------------------------------------------------

template <typename T>
void Grid<T>::toGPU(cl::Platform platform, cl::Device device, cl::Context context)
{
    std::cout << "Initializing OpenCL grid..." << std::endl;
    // Verificar que la grilla tenga un tamaño mayor a cero
    if (this->nRows <= 0 || this->nCols <= 0) {
        std::cerr << "Error: la grilla debe tener un tamaño mayor a cero." << std::endl;
        return;
    }

    // The context is assiged to the device and platform
    this->context = context;

    // The queue is created to push commands to the device
    this->queue = cl::CommandQueue(context, device);

    std::cout << "Creating a buffer..." << std::endl;
    
    // The buffer is created with the size of the grid
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, sizeof(T) * this->nRows * this->nCols);
    
    // We verify that the buffer has the same size as the grid
    if (buffer.getInfo<CL_MEM_SIZE>() != sizeof(T) * nRows * nCols) {
        std::cerr << "Error: la dimensión del buffer no es igual al tamaño de la grilla." << std::endl;
        return;
    }

    std::cout << "Copying grid to buffer..." << std::endl;
    
    // We copy the data from the grid to the buffer
    cl_int err = queue.enqueueWriteBuffer(buffer, CL_TRUE, 0, sizeof(T) * this->nRows * this->nCols, this->grid.data());

    // We verify that the data was copied correctly
    if (err != CL_SUCCESS) {
        std::cerr << "Error al copiar los datos de la grilla al buffer: " << err << std::endl;
        return;
    }

    std::cout << "Filling buffer with zeros..." << std::endl;
    
    // We fill the buffer with zeros
    err = queue.enqueueFillBuffer(buffer, 0, 0, sizeof(T) * this->nRows * this->nCols, NULL, NULL);
    
    // We verify that the buffer was filled correctly
    if (err != CL_SUCCESS) {
        std::cerr << "Error al inicializar el buffer: " << err << std::endl;
        return;
    }

    std::cout << "OpenCL grid initialized." << std::endl;

    // We assign the buffer to the gridBuffer attribute
    this->gridBuffer = buffer;
}

template <typename T>
cl::CommandQueue Grid<T>::getQueue()
{
    return this->queue;
}

template <typename T>
cl::Context Grid<T>::getContext()
{
    return this->context;
}

template <typename T>
cl::Program Grid<T>::getProgram()
{
    return this->program;
}

template <typename T>
cl::Buffer Grid<T>::getBuffer()
{
    return this->gridBuffer;
}

template <typename T>
cl::Device Grid<T>::getDevice()
{
    return this->device;
}

template <typename T>
void Grid<T>::populateRandomOpenCL(double percentage)
{
    // Seed the random number generator with the current time
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    
    // std::vector<T> flatData(nRows * nCols);
    for (int i = 0; i < nRows * nCols; i++) {
        this->grid[i] = (distribution(generator) < percentage) ? 1 : 0;
    }
    this->getQueue().enqueueWriteBuffer(this->getBuffer(), CL_TRUE, 0, sizeof(T) * nRows * nCols, this->grid.data());
}

template <typename T>
void Grid<T>::staticPopulate(){
    // create a vector with pre-defined values
    std::vector<T> flatData(nRows * nCols);

    // https://en.wikipedia.org/wiki/Wikipedia:Featured_picture_candidates/Game_of_Life_glider#/media/File:Game_of_life_animated_glider.gif
    for (int i = 0; i < nRows * nCols; i++) {
        flatData[i] = (i == 1 || i == (nCols + 2) || i == (2 * nCols) || i == (2 * nCols + 1) || i == (2 * nCols + 2)) ? 1 : 0;
    }

    this->getQueue().enqueueWriteBuffer(this->getBuffer(), CL_TRUE, 0, sizeof(T) * nRows * nCols, flatData.data());
}

template <typename T>
void Grid<T>::initializeBoardFromFile(int nRows, int nCols, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::vector<T> grid(nRows * nCols);
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
    this->getQueue().enqueueWriteBuffer(this->getBuffer(), CL_TRUE, 0, sizeof(T) * nRows * nCols, grid.data());
    file.close();
}

template class Grid<int>;