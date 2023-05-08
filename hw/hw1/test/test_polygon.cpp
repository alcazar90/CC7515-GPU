#include <gtest/gtest.h>
#include "../src/point.hpp"
#include "../src/vector.hpp"
#include "../src/polygon.hpp"


TEST(TestPolygon, TestInitPolygonDefault) {

	Polygon<int> pol0;

	Polygon<int> pol1(
		{Point<int>(),
		 Point<int>(1,1),
		 Point<int>(2,2),
		 Point<int>(3,3)
		}
	);

	Polygon<float> pol2(
		{Point<float>(),
		 Point<float>(1.4,1.2),
		 Point<float>(2.2,2.7),
		 Point<float>(3.5,4.1)
		}
	);

}

TEST(TestPolygon, TestPolygonAddPoint) {
	Polygon<int> pol1(
		{Point<int>(),
		 Point<int>(1,1),
		 Point<int>(2,2),
		 Point<int>(3,3)
		}
	);
	pol1.addPoint(Point<int>(4,4));
	EXPECT_EQ(pol1[3].toString(), "(3, 3)");
}

TEST(TestPolygon, TestPolygonGetNumPoints) {
	Polygon<int> pol0;
	Polygon<int> pol1(
		{Point<int>(),
		 Point<int>(1,1),
		 Point<int>(2,2),
		 Point<int>(3,3)
		}
	);
	EXPECT_EQ(pol0.getNumPoints(), 0);
	EXPECT_EQ(pol1.getNumPoints(), 4);
	pol1.addPoint(Point<int>(4,4));
	EXPECT_EQ(pol1.getNumPoints(), 5);
}

TEST(TestPolygon, TestPolygonSubsetOp) {
	Point<int> p2(2,2);
	Polygon<int> pol1(
		{Point<int>(),
		 Point<int>(1,1),
		 p2,
		 Point<int>(3,3)}
	);
	EXPECT_EQ(pol1[2] == p2, true);
	ASSERT_THROW(pol1[-6], std::out_of_range);
	ASSERT_THROW(pol1[6], std::out_of_range);
}

TEST(TestPolygon, TestPolygonIsCounterclockwise) {
	Polygon<int> pol1(
		{Point<int>(),
		 Point<int>(1,1),
		 Point<int>(2,2),
		 Point<int>(3,3)}
	);

	Polygon<int> pol2({
		 Point<int>(3,-3),
		 Point<int>(-3,-3),
		 Point<int>(-1,10)}
	);
	EXPECT_EQ(pol1.isCounterclockwise(), false);
	EXPECT_EQ(pol2.isCounterclockwise(), false);
}


