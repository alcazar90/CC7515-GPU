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

1. Normalizar interfaz de `std::cin` para que script benchmark ejecute los programas.
2. Agregar compilación de la implementación en CUDA a CMakeLists.txt, por ahora se compila de la siguiente forma. Ideal que el output quedé `./bin/gof_cuda` junto a los demás.
    ```bash
    nvcc gof_cuda.cu -o gof_cuda
    ```
3. Actualizar en `gof_sequential.cpp` el método `CountNeighbors` para que ocupe operaciones de modulo, considerando vecinos las diagonales y el tablero ciclico.
4. Agregar en `./test/` algo relacionado o un README con validación funcional de las implementaciones de _Game of Life_, generar un GIF por cada output o crear test set según ciertas configuraciones de tableros y estados de prueba que se encuentran en `./config/boardXX.txt`.
5. Crear gráficos y tablas con información del _benchmark_
6. Agregar interfaz para visualizar el juego usando `GLFW`
7. Implementar en Kokkos (?)

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

