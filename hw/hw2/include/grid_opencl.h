/*
    File: grid_opencl.h
*/
#include <vector>
#include <string>

// Include the OpenCL C++ bindings
#include <CL/cl2.hpp>

template <typename T>
class Grid {
public:
    Grid();
    Grid(int nRows, int nCols);

    ~Grid();

    // Return the number of rows/columns in the grid
    int numRows() const;
    int numCols() const;

    T& operator[](int y);

    // Returns the element at the specified row/col
    T get(int x, int y) const;

    // ----------------------------------------------------------
    // OpenCL methods
    // ----------------------------------------------------------

    // to gpu memory
    void toGPU(cl::Platform platform, cl::Device device, cl::Context context);

    // return the queue of opencl
    cl::CommandQueue getQueue();

    // return the context of opencl
    cl::Context getContext();

    // return the program of opencl
    cl::Program getProgram();

    // return the kernel of opencl
    cl::Kernel getKernel();

    // return the buffer of opencl
    cl::Buffer getBuffer();

    // return the device of opencl
    cl::Device getDevice();

    // populateRandomGPU fills % of the grid with 1s using OpenCL
    void populateRandomOpenCL(double percentage);

    // static populate
    void staticPopulate();

    // from txt file
    void initializeBoardFromFile(int nRows, int nCols, const std::string& filename);

private:
    std::vector<T> grid;
    int nRows;
    int nCols;
    cl::Buffer gridBuffer;
    cl::Context context;
    cl::CommandQueue queue;
    cl::Program program;
    cl::Kernel kernel;
    cl::Device device;
};
