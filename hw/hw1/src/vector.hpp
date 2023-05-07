/*
    Archivo: vector.hpp
    --------------------------------
    Implementación de la clase vector y métodos para operar con ellos.

    Author: Cristóbal Alcázar
*/
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>
#include "point.hpp"

template<typename T>
class Vector {

public:
    Vector();
    Vector(T xc, T yc);
    Vector(Point<T> pt) : x(pt.getX()), y(pt.getY()), coords(pt) {}
    // C++ nota: ~Vector() es el destructor de la clase Vector. Se ejecuta
    // de manera automatica cuando el objeto se elimina de la memoria.
    ~Vector();

    T getX() const;
    T getY() const;
    std::string toString() const;

    template<typename U>
    float dot(const Vector<U>& rhs) const {
        return this->x * rhs.x + this->y * rhs.y;
    };

    // C++ nota: al utilizar <typename U = T> podemos usar el metodo sobre
    // un vector sin especificar su tipo, e.g. v1.magnitude(). En caso
    // contrario, debemos especificar v1.magnitude<double>(). Si no se
    // especifica, el compilador trata de deducir el tipo de variable
    // desde T. Sin embargo, como se implementa magnitude() es una
    // funcion libre (no es miembro de Vector), el compilador no puede determinar
    // el tipo. En cambio, al decir que U = T, el compilador toma como defecto
    // el valor de U que es igual al del vector, y así puede deducir el tipo
    // del vector.
    template<typename U = T>
    float magnitude() const{
        return std::sqrt(this->dot<U>(*this));
    }

    // Nota: producto cruz no esta definido en vector 2D. Hay una implementación
    // que simula el producto cruz desde la perspectiva de la perpendicularidad.
    // Recordemos que el producto cruz entre v1 y v2 da como resultado un vector
    // v3 que es perpendicular tanto a v1 como v2. En la simulación de esta
    // propiedad en 2D, el producto cruz entre v1 y v2 solo devuelve un vector
    // perpendicular respecto a v1 y que se calcula a partir de v2.
    // La otra implementación simula el area del paralelograma que se forma
    // como base entre v1, v2 respecto al vector perpendicular a ambos v3 
    // que se obtiene del producto cruz entre v1 y v2. Resulta que el área de
    // este paralelograma es la magnitud del vector perpendicular v3. En esta
    // tarea, consideraremos la simulación del producto cruz como área como
    // definición para vectores en 2D porque se utiliza luego para actualizar
    // las areas de un poligono con el algoritmo descrito en las indicaciones
    // de la tarea.
    float cross(const Vector<T>& rhs) const {
	return (this->x * rhs.getY()) - (this->y * rhs.getX()); 
    };

    template<typename U>
    friend Vector<U> operator+(const Vector<U>& v1, const Vector<U>& v2);

    template<typename U>
    friend Vector<U> operator-(const Vector<U>& v1, const Vector<U>& v2);

    template<typename U>
    friend bool operator==(const Vector<U>& v1, const Vector<U>& v2);

    template<typename U>
    friend bool operator!=(const Vector<U>& v1, const Vector<U>& v2);

    template<typename U>
    friend Vector<U> operator*(const Vector<U>& v1, const float rhs); 

    template<typename U>
    friend Vector<U> operator*(const float lhs, const Vector<U>& v1); 

    // C++ nota: si se remueve el operador friend (no miembro, función amiga)
    // el operador pasa a ser un miembro de la clase. Por lo que se cuenta
    // el objeto que llama al operador = como parte del metodo con la informacion
    // relevante (contiene al objeto). Solo debe tener como argumento el vector
    // que estará a la derecha del operador. Otro punto relevante es que
    // el operador no debe ser definido de la siguiente forma sino arroja
    // error: Vector<T>& Vector<T>::operator=(const Vector<T>& rhs);
    Vector<T>& operator=(const Vector<T>& rhs);

private:
    T x;
    T y;
    Point<T> coords;
};

template<typename T>
Vector<T>::Vector() {
    x = 0;
    y = 0;
    coords = Point<T>(x, y);
}

// C++ nota: para evitar el problema de "shadowing" los parámetros del constructor
// deben tener nombres distintos a los atributos de la clase a instanciar. Sin
// embargo, se puede utilizar el operador arrow ->, que combina la operación de
// de-referencia (i.e. (*pp)) y de selección, en conjunto con el keyworkd "this"
// para referirse a la instancia actual de un objeto y acceder a sus atributos.
//
// Fuente: página 490-491 del libro "Programming Abstractions in C++" de Eric S. Roberts.
template<typename T>
Vector<T>::Vector(T x, T y) {
    this->x = x;
    this->y = y;
    coords = Point<T>(x, y);
}

template<typename T>
Vector<T>::~Vector() {
    //std::cout << "Vector destruído" << std::endl;
}

template<typename T>
T Vector<T>::getX() const{
  return x;
}

template<typename T>
T Vector<T>::getY() const{
  return y;
}

template<typename T>
std::string Vector<T>::toString() const
{
    return coords.toString();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
    os << v.toString() << std::endl;
    return os;
}

template<typename U>
Vector<U> operator+(const Vector<U>& v1, const Vector<U>& v2) {
    return Vector<U>(v1.x + v2.x, v1.y + v2.y);
}

template<typename U>
Vector<U> operator-(const Vector<U>& v1, const Vector<U>& v2) {
    return Vector<U>(v1.x - v2.x, v1.y - v2.y);
}

template<typename U>
bool operator==(const Vector<U>& v1, const Vector<U>& v2) {
    return (v1.x == v2.x && v1.y == v2.y);
}

// C++ nota: ejemplo de reciclar un método ya creado. No 
// es necesario crear las operaciones lógicas de negación
// cuando sabemos que != es la negación de ==. Lo que
// se logra negando !(predicado) 
template<typename U>
bool operator!=(const Vector<U>& v1, const Vector<U>& v2) {
    return !(v1 == v2);
}

template<typename U>
Vector<U> operator*(const Vector <U>& v1, const float rhs) {
   return Vector<U>(v1.x * rhs, v1.y * rhs);
}

template<typename U>
Vector<U> operator*(const float lhs, const Vector <U>& v1) {
   return v1 * lhs;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& rhs) {
	if (this != &rhs) {
		// Duda: ¿no hay una forma más elegante de definir esto?
		this->x = rhs.x;
		this->y = rhs.y;
		this->coords = Point<T>(this->x, this->y);

	}
	return *this;
}

#endif
