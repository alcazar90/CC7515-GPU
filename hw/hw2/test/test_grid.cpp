#include <gtest/gtest.h>
#include "../include/grid.h"
// TODO: eliminar "../src/grid.cpp" y usar el makefile. Problema con el archivo
// para vincular el código de grid.cpp
#include "../src/grid.cpp"

using namespace std;

// Using the same data configuration for multiple tests
// Fixtures: https://github.com/google/googletest/blob/main/docs/primer.md#test-fixtures-using-the-same-data-configuration-for-multiple-tests-same-data-multiple-tests
class TestGrid : public ::testing::Test {
    protected:
    void SetUp() override {
        //empty_grid = Grid<int>;
        grid = Grid<int>(10, 5);
    }

    // void TearDown() override {}
    Grid<int> grid;
    Grid<int> empty_grid;
    };

//TEST_F (i.e. fixture) permite usar clases seteadas fuera del test. Es útil
// cuando queremos utilizar un mismo objeto en varios tests.
TEST_F(TestGrid, TestInitGridDefault) {
    EXPECT_EQ(empty_grid.numRows(), 0);
    EXPECT_EQ(empty_grid.numCols(), 0);
}

TEST_F(TestGrid, TestInitGridWithSize) {
    EXPECT_EQ(grid.numRows(), 10);
    EXPECT_EQ(grid.numCols(), 5);
}
