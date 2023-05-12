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

TEST_F(TestGrid, TestGetGridCellElement) {
    //std::cout << "grid.get(0, 0): " << grid.get(0, 0) << std::endl;
    EXPECT_EQ(grid.get(0, 0), 0);
    EXPECT_EQ(grid.get(9, 4), 0);

    // out of bounds
    ASSERT_THROW(grid.get(-100, -100), std::out_of_range);
    ASSERT_THROW(grid.get(-2, 2), std::out_of_range);
    ASSERT_THROW(grid.get(2, -2), std::out_of_range);
    ASSERT_THROW(grid.get(100, 100), std::out_of_range);
}

TEST_F(TestGrid, TestFillGrid) {
    //std::cout << "grid.get(0, 0): " << grid.get(0, 0) << std::endl;
    grid.fill(1);
    EXPECT_EQ(grid.get(0, 0), 1);
    EXPECT_EQ(grid.get(9, 4), 1);

    grid.fill(-1);
    //std::cout << "grid.get(0, 0): " << grid.get(0, 0) << std::endl;
    EXPECT_EQ(grid.get(0, 0), -1);
    EXPECT_EQ(grid.get(9, 4), -1);
}


TEST_F(TestGrid, TestClearGrid) {
    grid.fill(1);
    EXPECT_EQ(grid.get(0, 0), 1);

    // clear grid: se debería re-instanciar la grilla para que todas las celdas
    // esten en 0. Las dimensiones originales se debieran mantener.
    grid.clear();
    EXPECT_EQ(grid.get(0, 0), 0);
    EXPECT_EQ(grid.numRows(), 10);
    EXPECT_EQ(grid.numCols(), 5);
}

TEST_F(TestGrid, TestEmptyGrid) {
    EXPECT_TRUE(empty_grid.isEmpty());
    EXPECT_FALSE(grid.isEmpty());
}

TEST_F(TestGrid, TestCellInBounds) {
    EXPECT_TRUE(grid.inBounds(0, 0));
    EXPECT_TRUE(grid.inBounds(9, 4));

    EXPECT_FALSE(grid.inBounds(-1, 1));
    EXPECT_FALSE(grid.inBounds(1, -1));
    EXPECT_FALSE(grid.inBounds(-100, -100));
}

TEST_F(TestGrid, TestSetCellValue) {
    grid.set(0, 0, 1);
    EXPECT_EQ(grid.get(0, 0), 1);

    grid.set(9, 4, 100);
    EXPECT_EQ(grid.get(9, 4), 100);
}

TEST_F(TestGrid, TestGridSize) {
    EXPECT_EQ(grid.size(), grid.numRows() * grid.numCols());
    EXPECT_EQ(empty_grid.size(), empty_grid.numRows() * empty_grid.numCols());
}

TEST_F(TestGrid, TestGridtoString) {
    std::string expected_empty_grid = "empty";
    EXPECT_EQ(empty_grid.toString(), expected_empty_grid);
    EXPECT_EQ(empty_grid.toString2D(), expected_empty_grid);

    std::string expected_nonempty_grid1D = "[0, 0, 0, 0, 0 | 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0 | 0, 0, 0, 0, 0]";
    std::string expected_nonempty_grid2D = "\n[0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0,\n 0, 0, 0, 0, 0]";
    EXPECT_EQ(grid.toString(), expected_nonempty_grid1D);
    EXPECT_EQ(grid.toString2D(), expected_nonempty_grid2D);
    //grid.set(0, 0, 1);
    //grid.set(9, 4, grid.size());
    //std::cout << "grid.toString2D(): " << grid.toString2D() << std::endl;
}

TEST_F(TestGrid, TestGridResize) {
    grid.resize(5, 5);
    EXPECT_EQ(grid.numRows(), 5);
    EXPECT_EQ(grid.numCols(), 5);
}
