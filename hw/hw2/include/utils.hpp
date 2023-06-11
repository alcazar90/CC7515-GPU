/*
 * File: utils.hpp
 * ---------------
 * 
*/
#include <iostream>
#include <string>

using namespace std;

int NITER;
int NROWS;
int NCOLS;
int THREADS;
int BLOCKS;
bool type_kernel;
bool PRETTYPRINT;
string filename;

void initGameConfig(int argc, char* argv[]) {
    // Inicializador - Configuracion de parámetros del juego
    if (argc == 6) {
        NITER = std::stoi(argv[1]);
        NROWS = std::stoi(argv[2]);
        NCOLS = std::stoi(argv[3]);
        PRETTYPRINT = (std::stoi(argv[4]) != 0);
        filename = argv[5];
        THREADS = std::stoi(argv[6]);
        type_kernel = std::stoi(argv[7]);
    }else{
        std::cout << "Please enter the number of iterations: ";
        std::cin >> NITER;
        std::cout << "Now we will be set the initial grid configuration..." << std::endl;
        std::cout << "Please enter the number of rows: ";
        std::cin >> NROWS;
        std::cout << "Please enter the number of columns: ";
        std::cin >> NCOLS;
        std::cout << "Do you want to visualize the game? (true=1 or false=0): ";
        std::cin >> PRETTYPRINT;
        std::cout << "Specify the name of the file to initialize the grid. If you want to initialize randomly, write 'RANDOM': ";
        std::cin >> filename;
        std::cout << "Please enter the number of threads: ";
        std::cin >> THREADS;
        std::cout << "Select the version with 'ifs'(1) or without 'ifs'(0): ";
        std::cin >> type_kernel;
        std::cout << "Please enter the number of blocks: ";
        std::cin >> BLOCKS;
    }

}