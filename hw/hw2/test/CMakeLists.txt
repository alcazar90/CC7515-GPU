cmake_minimum_required(VERSION 3.16.3)

add_executable(
    TestHW2
    test_grid.cpp
)

target_link_libraries(
    TestHW2
    gtest_main
    project
)

include(GoogleTest)
gtest_discover_tests(TestHW2)
