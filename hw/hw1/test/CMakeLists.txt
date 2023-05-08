cmake_minimum_required(VERSION 3.16.3)

add_executable(
	TestHW1
	test_point.cpp
	test_vector.cpp
	test_polygon.cpp
)

# la librería de google se llama gtest_main
# project es la libraría que incluye el proyecto principal
# con sus ejecutables. Esta se definio como un INTERFACE
target_link_libraries(
	TestHW1
	gtest_main
	project
)

# con include() cargamos los metodos de googletest
include(GoogleTest)
gtest_discover_tests(TestHW1)
