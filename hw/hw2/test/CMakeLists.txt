cmake_minimum_required(VERSION 3.16.3)

# requiere libreria por metodos con __device__ en grid.h
#find_package(CUDA REQUIRED)

# Tests related to grid.h methods
# ==============================================================================
add_executable(
    testGrid
    test_grid.cpp
)

target_link_libraries(
    testGrid
    gtest_main
    project
)

# Tests related to gof_sequential.cpp
# ==============================================================================
add_executable(
    testGofSequential
    test_gofSequential.cpp
)

target_link_libraries(
    testGofSequential
    gtest_main
    project
)


include(GoogleTest)
gtest_discover_tests(testGrid)
gtest_discover_tests(testGofSequential)
