// felibtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#pragma comment(lib,"gtestd.lib")


TEST(CMemMapFile, y2008)
{
	//EXPECT_EQ(31, GetMaxDayfromMonth(2008,1));
}
int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	return 0;
}

