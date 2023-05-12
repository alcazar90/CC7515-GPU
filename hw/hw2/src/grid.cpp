#include "../include/grid.h"

using namespace std;

template <typename T>
Grid<T>::Grid() : nRows(0), nCols(0) {
}

/*
template <typename T>
Grid<T>::Grid(int nRows, int nCols) : nRows(nRows), nCols(nCols), grid(nRows, std::vector<T>(nCols, 0)){
}
*/

template <typename T>
Grid<T>::Grid(int nRows, int nCols) {
    this->nRows = nRows;
    this->nCols = nCols;
    this->grid = std::vector<std::vector<T>>(nRows, std::vector<T>(nCols, 0));
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

template class Grid<int>;

/*
template <typename T>
T Grid<T>::get(int x, int y) const
{
    return T();
}

template <typename T>
void Grid<T>::clear()
{
}

template <typename T>
void Grid<T>::fill(T value)
{
}

template <typename T>
bool Grid<T>::isEmpty() const
{
    return false;
}

template <typename T>
bool Grid<T>::inBounds(int row, int col) const
{
    return false;
}

template <typename T>
void Grid<T>::resize(int nRows, int nCols)
{
}

template <typename T>
void Grid<T>::set(int row, int col, T value)
{
}

template <typename T>
int Grid<T>::size() const
{
    return 0;
}

template <typename T>
string Grid<T>::toString() const
{
    return string();
}

template <typename T>
string Grid<T>::toString2D() const
{
    return string();
}
*/