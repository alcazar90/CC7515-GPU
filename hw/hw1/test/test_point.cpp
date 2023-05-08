#include <gtest/gtest.h>
#include "../src/point.hpp"

// test para point.hpp
TEST(TestPoint, TestPointGetters) {
	Point p1(1.5, 3.7);
	EXPECT_EQ(p1.getX(), 1.5);
	EXPECT_EQ(p1.getY(), 3.7);
}

TEST(TestPoint, TestPointToString) {
	Point<int> p1(1, 2);
	EXPECT_EQ(p1.toString(), "(1, 2)");
}

TEST(TestPoint, TestPointSumTwoPoints) {
	Point p1(2, 3);
	Point p2(3, 2);
	EXPECT_EQ((p1 + p2).toString(), "(5, 5)"); 
}

TEST(TestPoint, TestPointComparison) {
	Point p1(2, 2);
	Point p2(2, 2);
	Point<int> p3;
	EXPECT_EQ(p1 == p2, true);
	EXPECT_EQ(p1 == p3, false);
}

TEST(TestPoint, TestPointAssignation) {
	Point p1(1, 2);
	Point p2(3, 4);
	p1 = p2;
	EXPECT_EQ(p1.toString(), p2.toString());
}

TEST(TestPoint, TestPointDistance) {
	Point p1(0, 0);
	Point p2(3, 4);

	EXPECT_EQ(p1.distance(p2), 5.0);
}

TEST(TestPoint, TestPointSubsetOp) {
	Point p1(1, 2);

	EXPECT_EQ(p1[0], p1.getX());
	EXPECT_EQ(p1[1], p1.getY());
}


/*
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
*/
