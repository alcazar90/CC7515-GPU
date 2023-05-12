#include "../include/grid.h"
#include <stdexcept>

using namespace std;

template <typename T>
Grid<T>::Grid() : nRows(0), nCols(0) {
}

/*
// Otra forma de inicializar, ¿cuál es la diferencia?
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

template <typename T>
std::vector<T> &Grid<T>::operator[](int y) {
    return grid[y];
}

template <typename T>
T Grid<T>::get(int x, int y) const {
    // verificar si la celda (x, y) se encuentra en los limites de la grilla
    if (x < 0 || x >= nRows || y < 0 || y >= nCols) {
        throw std::out_of_range("Invalid index");
    }
    const std::vector<T>& row = grid[x];
    return row[y];
}

template <typename T>
void Grid<T>::fill(T value) {
    for (int i = 0; i < nRows; i++) {
        std::vector<T>& row = grid[i];
        for (int j = 0; j < nCols; j++) {
            row[j] = value;
        }
    }
}

// TODO: no es l a forma más óptima de hacerlo, pero funciona.
// Agregar un método que entrega las coordenadas de las celdas que tienen 1s
// y las que tienen 0s. De esta forma, solo se selecciona una celda aleatoria
// del conjunto de celdas con 0s.
template <typename T>
void Grid<T>::populateRandom(double percentage) {
    int total = nRows * nCols;
    int num_ones = total * percentage;
    int num_zeros = total - num_ones;
    this->fill(0);
    for (int i = 0; i < num_ones; i++) {
        // C++ nota: rand() % nRows, da un número aleatorio entre 0 y nRows - 1
        int x = rand() % nRows;
        int y = rand() % nCols;
        while (true) {
            if (get(x, y) == 0) {
                break; 
            } else {
                x = rand() % nRows;
                y = rand() % nCols;
            }
        }
        this->set(x, y, 1);
    }
}

template <typename T>
void Grid<T>::clear()
{
    this->fill(0);
}

template <typename T>
bool Grid<T>::isEmpty() const {
    if (nRows == 0 || nCols == 0) {
        return true;
    }
    return false;
}

template <typename T>
bool Grid<T>::inBounds(int row, int col) const {
    if ((row >= 0 && row < nRows) && (col >= 0 && col < nCols)) {
        return true;
    }
    return false;
}

template <typename T>
void Grid<T>::set(int x, int y, T v) {
    std::vector<T>& row = grid[x];
    row[y] = v;
}

template <typename T>
int Grid<T>::size() const {
    return this->nRows * this->nCols;
}

template <typename T>
string Grid<T>::toString() const {
    std::string result;
    if (this->isEmpty()) {
        result += "empty";
        return result;
    }
    result += "[";
    for (int i = 0; i < nRows; i++) {
        const std::vector<T>& row = grid[i];
        for (int j = 0; j < nCols; j++) {
            result += std::to_string(row[j]);
            if (j == nCols - 1 && i == nRows - 1) {
                result += "]";
            } else if (j == nCols - 1) {
                result += " | ";
            } else {
                result += ", ";
            }
        }
    }
    return result;
}

template <typename T>
string Grid<T>::toString2D() const {
    std::string result;
    if (this->isEmpty()) {
        result += "empty";
        return result;
    }
    result += "\n[";
    for (int i = 0; i < nRows; i++) {
        const std::vector<T>& row = grid[i];
        for (int j = 0; j < nCols; j++) {
            result += std::to_string(row[j]);
            if (j == nCols - 1 && i == nRows - 1) {
                result += "]";
            } else if (j == nCols - 1) {
                result += ",\n ";
            } else {
                result += ", ";
            }
        }
    }
    return result;
}

template <typename T>
void Grid<T>::resize(int nRows, int nCols) {
    this->nRows = nRows;
    this->nCols = nCols;
    this->grid = std::vector<std::vector<T>>(nRows, std::vector<T>(nCols, 0));
}

template class Grid<int>;
