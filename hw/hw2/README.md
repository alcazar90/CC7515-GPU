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
* Implementar el algoritmo en CUDA
* Implementar el algoritmo en OpenCL
* Medir y comparar desempeño de las implementaciones bajo varios tamaños de grillas

### TODOs

1. Agregar método que entrega el conjunto de coordenadas `(x,y)` con celdas con 1s
2. Agregar método que entregue el conjunto de coordenadas `(x, y)` con celdas con 0s
3. Modificar método `grid.populateRandom(percentage)` para que escoga aleatoriamente del conjunto de 0s, en vez de samplear hasta que encuentre una celda disponible para modificar
4. ~~Implementar juego de la vida de forma secuencial, usar para verificar pocos pasos y el método `grid.toString2D`~~
5. Implementar GUI para poder visualizar mejor la grilla y tener mayor control del programa
6. Implementar código para extraer estadísticas del estado de un tablero en cada configuración (e.g. número celdas vivas, muertos, cantidad de clusters?)


### Referencias

¿Cómo iterar a través de la fila del `grid`?

```cpp
int main() {
    // inicializar grid de 10x5
    grid = Grid<int>(10, 5);

    // operador [] sobre escrito en grid para que entregue la ith fila
    std::vector<int> row = grid[1];

    // luego podemos iterar sobre el vector
    std::cout << "Todas las celdas de la fila 2:";
    for (auto i : row) {
        std::cout << ' ' << i;
    }
    std::cout << '\n';
}
```

Para acceder a información de celdas especificas usar `grid.get(x, y)`:

```cpp
int main() {
    // inicializar grid de 10x5
    grid = Grid<int>(10, 5);

    // acceder a la celda 2, 2
    std::cout << "grid.get(1, 1): " << grid.get(1, 1) << std::endl;

    // esto arroja la excepcción std::out_of_range("Invalid Index")
    std::cout << "grid.get(100, 100): " << grid.get(100, 100) << std::endl;

}
```

