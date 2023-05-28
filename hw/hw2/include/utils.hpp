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
bool PRETTYPRINT;
string filename;

void initGameConfig() {
    // Inicializador - Configuracion de parámetros del juego
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
}