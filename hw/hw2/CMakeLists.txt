cmake_minimum_required(VERSION 3.16.3)

project(hw2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(GTEST_LIB googletest)

# Agregué las siguientes líneas para buscar OpenCL
find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/${GTEST_LIB})
enable_testing()
add_subdirectory(test)

add_library(project INTERFACE)
target_include_directories(project INTERFACE .)

# Ejecutable gof_sequential
add_executable(gof_sequential
    include/grid.h
    src/grid.cpp
    gof_sequential.cpp
)

target_link_libraries(gof_sequential project)

# Ejecutable gof_opencl
add_executable(gof_opencl
    include/grid_opencl.h
    src/grid_opencl.cpp
    gof_opencl.cpp
)

target_link_libraries(gof_opencl project ${OpenCL_LIBRARIES})