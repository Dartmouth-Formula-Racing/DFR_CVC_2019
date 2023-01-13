/* Testing/test_main.cpp */
#include "gtest/gtest.h"
#include "TrivialClass.hpp"

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}

TEST(BasicTest, PassingTest)
{
	TrivialClass t;
	EXPECT_TRUE(t.returnsTrue());
}

TEST(BasicTest, FailingTest)
{
	TrivialClass t;
	EXPECT_TRUE(t.returnsFalse());
}
