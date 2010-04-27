#include <gtest/gtest.h>
#include "../fefuntion/stringEx.h"

// ::testing::AssertionResult Assertstring_replace(const char* m_expr, const char* n_expr, int m, int n) { 
// 	if (MutuallyPrime(m, n)) 
// 		return ::testing::AssertionSuccess(); 
// 	::testing::Message msg; 
// 	msg << m_expr << " and " << n_expr << " (" << m << " and " << n 
// 		<< ") are not mutually prime, " << "as they have a common divisor " 
// 		<< SmallestPrimeCommonDivisor(m, n); 
// 	return ::testing::AssertionFailure(msg); 
// }

TEST(stringTest, �ַ����滻)
{
	std::string testStr="abcabcabcabc";
	std::string resultStr="aaaa";
	string_replace(testStr,"abc","a");
	EXPECT_EQ(resultStr, testStr);
}
TEST(stringTest, �ַ����滻2)
{
	std::string testStr="abcabcabcabc";
	std::string resultStr="aaaa";
	string_replace(testStr,"abc","a");
	EXPECT_EQ(resultStr, testStr);
}


TEST(stringSplit, �ַ����ָ�)
{
	std::string testStr="cccccccccbccccccccbccccccccccbcccccccccccbb";
	std::string resultStr="b";
	std::vector<std::string> bb=string_split(testStr,"b","");
	EXPECT_EQ(6,bb.size());

}

TEST(IPCoverNumberToString, ����תIP��ַ)
{
	EXPECT_EQ(IPCoverNumberToString(3232235757),"192.168.0.237");
}