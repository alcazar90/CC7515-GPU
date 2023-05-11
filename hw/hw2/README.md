## The Game of Life

Las reglas del juego:

1. Una célula nace de un espacio muerto si tiene 3 vecinos vivos.
2. Una célula sobrevive en la próxima generación si tiene 2 o 3 vecinos.

Objetivo de la tarea:

* Realizar una implementación serial en CPU
  * Implementar clase `grid`
  * Usar `googletest` para crear unittest de la clase `grid`
  * Crear clase `board` basado en `grid` con métodos para inicializar de manera aleatoria, guardar configuraciones en archivos txt o inicializar una configuración a partir de un archivo txt
  * Crear main con inicialización del juego sin GUI, sino utilizando el método `grid::toString2D()`:
* Implmenetar el algoritmo en CUDA
* Implementar el algoritmo en OpenCL
* Medir y comparar desempeño de las implementaciones bajo varios tamaños de grillas
