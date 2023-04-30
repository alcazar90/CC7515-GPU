/*
    Archivo: point.hpp
    --------------------------------
    Implementación de la clase punto y métodos para operar con ellos.

    Author: Cristóbal Alcázar
*/
#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <string>
#include <iostream>


template<typename T>
class Point {

public:
    Point();
    Point(T xc, T yc);
    ~Point();
    T getX() const;
    T getY() const;
    std::string toString() const;

    // Se declaran con "friend" porque requieren acceder a los miembros privados
    // de la clase. Además, se debe especificar un template porque el método
    // retornará un objeto Point con el mismo tipo de dato que los objetos Point
    // de entrada, y pueden ser float o int. Por eso, se debe declarar una
    // clase template tipo U.
    template<typename U>
    friend Point<U> operator+(const Point<U>& pt1, const Point<U>& pt2);

    template<typename U>
    friend bool operator==(const Point<U>& pt1, const Point<U>& pt2);

    Point<T>& operator=(const Point<T>& rhs);

    double distance(const Point<T>& rhs);

    T& operator[](int index) {
        if (index == 0) {
            return point_coords.first;
        } else if (index == 1) {
            return point_coords.second;
        } else {
            // Handle invalid index
            throw std::out_of_range("Invalid index");
        }
    };

private:
    std::pair<T, T> point_coords;
};

template<typename T>
Point<T>::Point() {
    std::pair<T, T> p;
    p.first = 0;
    p.second = 0;
    point_coords = p;
};

template<typename T>
Point<T>::Point(T xc, T yc) {
    std::pair<T, T> p;
    p.first = xc;
    p.second = yc;
    point_coords = p;
}
template <typename T>
Point<T>::~Point() {
    // Destructor
    std::cout << "Punto destruído" << std::endl;
};

template<typename T>
T Point<T>::getX() const {
    return point_coords.first;
};

template<typename T>
T Point<T>::getY() const {
    return point_coords.second;
};

template<typename T>
std::string Point<T>::toString() const {
    return "(" + std::to_string(point_coords.first) + ", " + std::to_string(point_coords.second) + ")";
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Point<T>& pt) {
    os << pt.toString() << std::endl;
    return os;
};

template<typename U>
Point<U> operator+(const Point<U>& pt1, const Point<U>& pt2) {
    return Point<U>(pt1.point_coords.first + pt2.point_coords.first, 
                    pt1.point_coords.second + pt2.point_coords.second);
};

// TODO: Modificar función para que pueda comparar un punto entero con otro 
// flotante. Siempre castear el int al método más flexible (float)
template<typename U>
bool operator==(const Point<U>& pt1, const Point<U>& pt2) {
    return (pt1.point_coords.first == pt2.point_coords.first) && 
           (pt1.point_coords.second == pt2.point_coords.second);
};

template<typename T>
Point<T>& Point<T>::operator=(const Point<T>& rhs) {
    if (this != &rhs) {
        this->point_coords.first = rhs.point_coords.first;
        this->point_coords.second = rhs.point_coords.second;
    }
        return *this;
};

template<typename T>
double Point<T>::distance(const Point<T>& rhs) {
    return {std::sqrt(std::pow(point_coords.first - rhs.point_coords.first, 2) 
                    + std::pow(point_coords.second - rhs.point_coords.second, 2))};
};

#endif
