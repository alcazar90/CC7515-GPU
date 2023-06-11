## Assignment 2: The Game of Life 👾

Más acerca <a href="https://conwaylife.com/book/" target="_blank">Conway's Game of Life</a>.

<center>
<img src="https://global.discourse-cdn.com/mcneel/uploads/default/original/3X/9/9/9903c2258a7822736a2fcb9628e40bb63f8d0b28.gif">
</center>

### Las reglas del juego

1. Una célula nace de un espacio muerto si tiene 3 vecinos vivos.
2. Una célula sobrevive en la próxima generación si tiene 2 o 3 vecinos. En caso contrario, o muere de subpoblación o sobrepoblación respectivamente.

### Clonar el repositorio e iniciar submodulo de testing.

```sh
git clone https://github.com/alcazar90/CC7515-GPU
git submodule init
git submodule update
cd hw/hw2
```

### Para ejecutar la tarea 2 se debe ejecutar el siguiente comando:
```bash
mkdir build
cd build
cmake ..
make
```
### Esto creará tres ejecutables en la carpeta `bin`. Para ejecutar cada uno de ellos se debe ejecutar el siguiente comando:
```bash
./bin/<nombre_ejecutable>
``` 

### Al inicio de cada programa se le pedirá al usuario que ingrese los parámetros de la simulación. Estos son:
- Número de iteraciones.
- Número de filas.
- Número de columnas.
- Visualización de la simulación (Sí=1 o No=0).
- Cargar un archivo de configuración de la grilla o generarla aleatoriamente.
- - Si se carga un archivo, se debe ingresar el nombre del archivo: './config/board02.txt'
- Número de threads.
- Seleccionar la versión del kernel (Con ifs=1 o Sin ifs=0).
- Número de bloques.

### Ejemplo de ejecución:

```bash
~/CC7515-GPU/hw/hw2/build$ ../bin/gof_cuda 
Please enter the number of iterations: 5
Now we will be set the initial grid configuration...
Please enter the number of rows: 32
Please enter the number of columns: 32
Do you want to visualize the game? (true=1 or false=0): 1
Specify the name of the file to initialize the grid. If you want to initialize randomly, write 'RANDOM': ../config/board02.txt
Please enter the number of threads: 32
Select the version with 'ifs'(1) or without 'ifs'(0): 0
Please enter the number of blocks: 32
Welcome to the game of life. We will play 5 generations...and the universe will be a grid of size 32x32
```

