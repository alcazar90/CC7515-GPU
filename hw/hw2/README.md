## Assignment 2: The Game of Life 👾

Más acerca <a href="https://conwaylife.com/book/" target="_blank">Conway's Game of Life</a>.

<center>
<img src="https://global.discourse-cdn.com/mcneel/uploads/default/original/3X/9/9/9903c2258a7822736a2fcb9628e40bb63f8d0b28.gif">
</center>

### Las reglas del juego

1. Una célula nace de un espacio muerto si tiene 3 vecinos vivos.
2. Una célula sobrevive en la próxima generación si tiene 2 o 3 vecinos.

### Objetivo de la tarea

* Realizar una implementación serial en CPU
  * Implementar clase `grid`
  * Usar `googletest` para crear unittest de la clase `grid`
  * Crear clase `board` basado en `grid` con métodos para inicializar de manera aleatoria, guardar configuraciones en archivos txt o inicializar una configuración a partir de un archivo txt
  * Crear main con inicialización del juego sin GUI, sino utilizando el método `grid::toString2D()`:
* Implmenetar el algoritmo en CUDA
* Implementar el algoritmo en OpenCL
* Medir y comparar desempeño de las implementaciones bajo varios tamaños de grillas

### Referencias

`grid` clase página 210 e interface página 957.
