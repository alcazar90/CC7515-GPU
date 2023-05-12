/*
    File: grid.h

    Interfaz basado en los métodos de la clase Grid de la librería StanfordCPPLib
    Ref: https://web.stanford.edu/dept/cs_edu/resources/cslib_docs/Grid
*/
#ifndef GRID_H
#define GRID_H

#include <vector>

template <typename T>
class Grid {
public:
    Grid();
    Grid(int nRows, int nCols);

    ~Grid();

    // Return the number of rows/columns in the grid
    int numRows() const;
    int numCols() const;

    /*
    // Returns the element at the specified row/col
    T get(int x, int y) const;

    // Remove all elements from this grid
    void clear();

    // Sets every element in this grid to the given value
    void fill(T value);

    // Returns true if this grid has 0 rows and/or 0 columns
    bool isEmpty() const;

    // Returns true if the specified row/col is inside the bounds of this grid
    bool inBounds(int row, int col) const;

    // Reinitializes this grid to have the specified number of rows and columns
    void resize(int nRows, int nCols);

    // Replaces the element at the specified row/col with a new value
    void set(int row, int col, T value);

    // Returns the total number of elements in the grid
    int size() const;

    // Returns a printable single-line string of this grid
    string toString() const;

    // Returns a printable 2-D string representation of this grid.
    string toString2D() const;
    */

private:
    std::vector<std::vector<T>> grid;
    int nRows;
    int nCols;
};
#endif
