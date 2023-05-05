cmake_minimum_required(VERSION 3.16.3)

project(hw1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# For setting a directory to the executable. I am setting it to the bin folder
# in the root directory (TODO: what's the best practice for this?)
# configurar un directorio (i.e. ./bin) para almacenar los ejecutables del proyecto
# ¿cuál es la mejor práctica para esto?
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# configurar testing con librería googletest
set(GTEST_LIB googletest)

add_subdirectory(./vendor/${GTEST_LIB})
enable_testing()
add_subdirectory(test)

# Nota: esta fue la forma que encontré para vincular los archivos .hpp
# al /test/CMakeLists.txt
# Fuente: https://stackoverflow.com/questions/60604249/how-to-make-a-header-only-library-with-cmake
add_library(project INTERFACE)
target_include_directories(project INTERFACE .)

# agregar en SRC_FILES los archivos a compilar
set(SRC_FILES
    src/point.hpp
    src/vector.hpp
    src/polygon.hpp
    main.cpp
)

# vinculamos los archivos SRC_FILES a un ejecutable
add_executable(main ${SRC_FILES})
