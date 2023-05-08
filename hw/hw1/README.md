## Assignment 1

Este proyecto incluye las implementaciones de las clases punto, vector,
y poligono de la tarea 1 del curso CC7515 - Computación en GPU.

### Instrucciones para compilar 

Para compilar el proyecto se deben correr las siguientes líneas de código en el terminal:

```bash
mkdir build
cd build
cmake ..
make
```

Todos los ejecutables quedán guardados en la carpeta `./bin` en la carpeta
raíz del proyecto `hw1`. El ejecutable `main` contiene los resultados
del caso de uso de los objetos creados en el código fuente, y `TestHW1` ejecuta
y reporta los resultados de los test unitarios implementados en los archivos de
 _testing_ del  subdirectorio `test`.

```bash
./bin/main
./bin/TestHW1
```


### Estructura del proyecto

La estructura del proyecto contiene una carpeta `src` con el código fuente
donde se definen las clases y métodos (i.e. `.hpp`), existe un `CMakeLists.txt` para compilar el proyecto siguiendo las instrucciones de arriba. Los ejecutables se crean en el directorio `bin` a nivel de la carpeta del proyecto. Finamlmente, se utiliza un submodulo
para implementar los test unitarios con la librería externa `googletest`, la 
cual se almacena en la carpeta `vendor`.

```
/src/
   |_point.hpp
   |_vector.hpp
   |_polygon.hpp
/bin/
   |_main
   |_TestHW1
/test/
   |_CMakeLists.txt
   |_test_point.cpp
   |_test_vector.cpp
   |_test_polygon.cpp
/vendor/
   |_googletest
CMakeLists.txt
main.cpp
README.md
```

### Submodulos y librería `googletest`

En este proyecto utilicé la librería externa de testing [googletest](https://github.com/google/googletest) como submodulo de este proyect. Esto quiere
decir que en la raíz de este repositorio, hay un archivo `.gitmodule` que contiene la información del submodulo a utilizar. Importante revisar
esta pregunta sobre como inicializar un repositorio que contiene submodulos, la idea es que cuando uno clone este repo, se descargen las dependencias
a estas librerías de manera automática. 

```bash
git clone project
git submodule init
```

Sin embargo, para que esto ocurra se debe (i) clonar el proyecto
como submodulo (e.g. `git submodule add https://github.com/chaconinc/DbConnector`) y (ii) realizar un commit del submodulo.
```
