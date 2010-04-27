#include "include.h"
#include <gtest/gtest.h>
#ifdef _DEBUG
	#pragma comment(lib,"gtestd.lib")
#endif // _DEBUG

TEST(TestMonthDay, y2009)
{
	EXPECT_EQ(31, GetMaxDayfromMonth(2009,1));
	EXPECT_EQ(28, GetMaxDayfromMonth(2009,2));
 	EXPECT_EQ(31, GetMaxDayfromMonth(2009,3));
	EXPECT_EQ(30, GetMaxDayfromMonth(2009,4));
	EXPECT_EQ(31, GetMaxDayfromMonth(2009,5));
	EXPECT_EQ(30, GetMaxDayfromMonth(2009,6));
	EXPECT_EQ(31, GetMaxDayfromMonth(2009,7));
	EXPECT_EQ(31, GetMaxDayfromMonth(2009,8));
	EXPECT_EQ(30, GetMaxDayfromMonth(2009,9));
	EXPECT_EQ(31, GetMaxDayfromMonth(2009,10));
	EXPECT_EQ(30, GetMaxDayfromMonth(2009,11));
	EXPECT_EQ(31, GetMaxDayfromMonth(2009,12));
}
TEST(TestMonthDay, y2008)
{
	EXPECT_EQ(31, GetMaxDayfromMonth(2008,1));
	EXPECT_EQ(29, GetMaxDayfromMonth(2008,2));
	EXPECT_EQ(31, GetMaxDayfromMonth(2008,3));
	EXPECT_EQ(30, GetMaxDayfromMonth(2008,4));
	EXPECT_EQ(31, GetMaxDayfromMonth(2008,5));
	EXPECT_EQ(30, GetMaxDayfromMonth(2008,6));
	EXPECT_EQ(31, GetMaxDayfromMonth(2008,7));
	EXPECT_EQ(31, GetMaxDayfromMonth(2008,8));
	EXPECT_EQ(30, GetMaxDayfromMonth(2008,9));
	EXPECT_EQ(31, GetMaxDayfromMonth(2008,10));
	EXPECT_EQ(30, GetMaxDayfromMonth(2008,11));
	EXPECT_EQ(31, GetMaxDayfromMonth(2008,12));
}

// TEST(TestMonthDay, yn)
// {
// 	int i=1900,
// 	EXPECT_EQ(31, GetMaxDayfromMonth(2008,12));
// }
int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}