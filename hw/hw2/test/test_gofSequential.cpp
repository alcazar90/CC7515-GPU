/*
 * File: test_gofSequential.cpp
 * -----------------------------
 * Este archivo contiene los test unitarios para la implementación secuencial
 * del juego de la vida. Este se base en la estructura de dato grid.h.
 * 
 * En particular, se inicializan distintas configuraciones de tableros 
 * que se encuentran en la raíz del proyecto, en la carpeta config.
 * 
 * El objetivo es verificar que la evolución del programa sea correcto
 * para distintas configuraciones de tableros, en determinadas generaciones.
 * 
 * TODO:
 * - ¿Cómo se puede llamar un programa ejecutable con los argumentos por flags
 *  desde un test unitario?
 * 
 * - ¿Cómo se puede organizar el cmakefile para que se compile primero 
 *    el programa gof_sequential.cpp y luego se pueda llamar por este programa
 *    para probar distintas configuraciones?
 * 
 * - ¿Esto se puede realizar con la librería gmock?
*/
#include <gtest/gtest.h>
#include "../include/grid.h"
#include "../src/grid.cpp"

using namespace std;

class TestGofSequential : public ::testing::Test {
    protected:
    void SetUp() override {
        Grid<int> board01;
        board01.initializeFromFile("../config/board01.txt");
    }

    // void TearDown() override {}
    Grid<int> board01;
    };

TEST_F(TestGofSequential, TestInitializeFromFile) {
    std::cout << "board01.toString2D(): " << board01.toString2D() << std::endl;
}