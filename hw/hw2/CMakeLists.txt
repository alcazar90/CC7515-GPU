cmake_minimum_required(VERSION 3.16.3)

project(hw2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(GTEST_LIB googletest)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/${GTEST_LIB})
enable_testing()
add_subdirectory(test)

add_library(project INTERFACE)
target_include_directories(project INTERFACE .)

set(SRC_FILES
    include/grid.h
    src/grid.cpp
    gof_sequential.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
