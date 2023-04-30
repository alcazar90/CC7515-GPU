# version of CMake required to create the Makefile
cmake_minimum_required(VERSION 3.16.3)

# Name of the project
project(hw1)

# Setting C++ standard to C++17 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# For setting a directory to the executable. I am setting it to the bin folder
# in the root directory (TODO: what's the best practice for this?)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Adding the source files (every .cpp that is required to compile) to the 
# SRC_FILES variable
set(SRC_FILES
    src/point.hpp
    src/vector.hpp
    src/polygon.hpp
    main.cpp
)

# We link the compiled object files to the executable. The main.cpp needs to
# compile with the point.cpp file
add_executable(main ${SRC_FILES})
