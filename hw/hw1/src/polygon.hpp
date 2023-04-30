/*:
 * File: polygon.hpp
 * --------------------
 *
 * Author: Cristóbal Alcázar
 *
 */
#ifndef POLYGON_H
#define POLYGON_H

#include <cmath>
#include <iostream>
#include <vector>
#include "point.hpp"
#include "vector.hpp"

template<typename T>
class Polygon {

public:
    // Nota C++: inicialización directa por lista. Es posible inicializar
    // antes de crear una implementación para el constructor. Evita la
    // creación de objetos temporales como iterar por una lista de elementos
    // y copiar en otro vector.
    Polygon() : N(0) {}
    Polygon(std::vector<Point<T>> pts) : N(pts.size()), polygon(pts) {}

    ~Polygon();

    // metodo para obtener la cantidad de puntos
    int getNumPoints() const { return N; }

    // metodo para agregar un punto al final del poligono
    // C++ nota: std::vector.emplace_back(): construye el objeto Point<T> directamente
    // en el vector. std::vector.push_back() potencialmente puede crear una copia.
    template<typename U>
    void addPoint(const Point<U>& pt) {
	polygon.emplace_back(pt);
	N++;
    }

    // metodo que indique si un poligono esta en counterclockwise o no (indicado en instrucciones)
    //bool Polygon<T>::isCounterclockwise();
    
    bool isCounterclockwise() const {
        double area = 0.0;
        for (int i = 0; i < N; i++) { 
            const Vector<T>& v1(this->polygon[i]);
	    const Vector<T>& v2(this->polygon[(i + 1) % N]);
	    area += v1.cross(v2);
        }
        return area > 0.0;
    }

    // operadores sobre cargados para operator[](int i) obtener el punto en la posicion i
    const Point<T>& operator[](int index) const {    
        if (index < 0 || index >= N) {
            throw std::out_of_range("Invalid index");
        } 
	return polygon[index];
     }

    // operador ostream
    friend std::ostream& operator<<(std::ostream& os, const Polygon<T>& poly) {
        os << "[";
        for (int i = 0; i < poly.N; i++) {
	    if (i > 0) os << ", ";
	    os << poly.polygon[i];
        }
        os << "]";
        return os;
    }

private:
    int N;
    std::vector<Point<T>> polygon;
};

template<typename T>
Polygon<T>::~Polygon() {
    std::cout << "Vector destruído" << std::endl;
}

#endif

