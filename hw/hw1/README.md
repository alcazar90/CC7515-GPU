## Assignment 1

Este proyecto incluye las implementaciones de las clases punto, vector,
y poligono de la tarea 1 del curso CC7515 - Computación en GPU.

Para compilar el proyecto se deben correr las siguientes líneas de código en el terminal:

```bash
mkdir build
cd build
cmake ..
make
```

Todos los ejecutables quedán guardados en la carpeta `./bin` del proyecto:

```bash
./bin/main
./bin/PointTests
```

La estructura del proyecto contiene una carpeta `src` con el código fuente
donde se definen las clases y métodos (i.e. `.hpp`), existe un `CMakeLists.txt` para compilar el proyecto siguiendo las instrucciones de arriba. El ejecutable se crea en el directorio `bin` a nivel de la carpeta del
proyecto. Finamlmente, los archivos de prueba se encuentran en... 

```
/src/
   |_point.hpp
   |_vector.hpp
   |_polygon.hpp
/bin/
   |_ejecutables
/test/
   |_CMakeLists.txt
   |_test_point.cpp
/vendor/
   |_googletest
CMakeLists.txt
main.cpp
README.md
```
