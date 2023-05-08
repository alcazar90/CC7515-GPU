#include <gtest/gtest.h>
#include "../src/point.hpp"
#include "../src/vector.hpp"

TEST(TestVector, TestInitVectorDefault) {
	Vector<int> v1;
	Vector<float> v2;
}

TEST(TestVector, TestInitVectorFromCoords) {
	Vector<int> v1(1, 8);
	Vector<float> v2(2.5, 3.3);
}

TEST(TestVector, TestInitVectorFromPoint) {
	Point<int> p1(0, 0);
	Point<float> p2(2.5, 3.8);
	Vector v1(p1);
	Vector v2(p2);
}

TEST(TestVector, TestVectorGetters) {
	Vector<int> v1(3, -1);
	
	EXPECT_EQ(v1.getX(), 3);
	EXPECT_EQ(v1.getY(), -1);
}

TEST(TestVector, TestVectorToString) {
	Vector<int> v1(3, -1);
	EXPECT_EQ(v1.toString(), "(3, -1)");
}

TEST(TestVector, TestVectorDotProduct) {
	Vector<int> v1(2, 4);
	Vector<int> v2(3, 5);
	
	EXPECT_FLOAT_EQ(v1.dot(v2), 26.0);
}

TEST(TestVector, TestVectorMagnitude) {
	Vector<int> v1(3, 4);
	Vector<float> v2(1.0, 2.5);

	EXPECT_FLOAT_EQ(v1.magnitude(), 5.0);
	EXPECT_FLOAT_EQ(v2.magnitude(), 2.6925824);

}

TEST(TestVector, TestVectorAdditionIntToInt) {
	Vector<int> v1(3, 4);
	Vector<int> v2(2, 1);
	EXPECT_EQ((v1 + v2).toString(), "(5, 5)");
	EXPECT_EQ((v1 - v2).toString(), "(1, 3)");
}

TEST(TestVector, TestVectorAdditionFloatToFloat) {
	Vector<float> v1(3.2, 4.0);
	Vector<float> v2(2.5, 1.8);
	EXPECT_FLOAT_EQ((v1 + v2).getX(), 5.7);
	EXPECT_FLOAT_EQ((v1 + v2).getY(), 5.8);
	EXPECT_FLOAT_EQ((v1 - v2).getX(), 0.7);
	EXPECT_FLOAT_EQ((v1 - v2).getY(), 2.2);
}

TEST(TestVector, TestVectorComparison) {
	Vector<int> v1(1, 2);
	Vector<int> v2(1, 2);
	Vector<int> v3(99,99);
	EXPECT_EQ(v1 == v2, true);
	EXPECT_EQ(v1 == v3, false);
	EXPECT_EQ(v1 != v3, true);
	EXPECT_EQ(v1 != v2, false);
}

TEST(TestVector, TestFloatVectorScale) {
	Vector<float> v1(1, 2);
	EXPECT_EQ((v1 * 2).getX(), 2);
	EXPECT_EQ((v1 * 2).getY(), 4);
	EXPECT_EQ((-2 * v1).getX(), -2);
	EXPECT_EQ((-2 * v1).getY(), -4);
	EXPECT_EQ((v1 * 0.5).getX(), 0.5);
	EXPECT_EQ((v1 * 0.5).getY(), 1.0);
}


TEST(TestVector, TestIntVectorScale) {
	Vector<int> v1(1, 2);
	EXPECT_EQ((v1 * 2).getX(), 2);
	EXPECT_EQ((v1 * 2).getY(), 4);
	EXPECT_EQ((-2 * v1).getX(), -2);
	EXPECT_EQ((-2 * v1).getY(), -4);
	// deberia transformarse de int a float cuando se escala por un número
	// no entero? supuesto: no incorporaar mezcla de tipos de variables.
	//EXPECT_EQ((v1 * 0.5).getX(), 0.5);
	//EXPECT_EQ((v1 * 0.5).getY(), 1.0);
}

TEST(TestVector, TestVectorAssignation) {
	Vector<int> v1(99,99);
	Vector<int> v2;
	v2 = v1;
	EXPECT_EQ(v2.getX(), 99);
	EXPECT_EQ(v2.getY(), 99);
}

