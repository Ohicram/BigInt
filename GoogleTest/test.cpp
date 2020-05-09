#include "pch.h"
#include "BigInt.h"
TEST(TestCaseName, TestName) {
	BigInt voidVal;
	EXPECT_TRUE(voidVal == voidVal);
	BigInt longString{ "123456789102030405060708090100" };
	EXPECT_TRUE(longString == longString);
	BigInt longValue(123456789);
	EXPECT_TRUE(longValue == longValue);
	BigInt negativeValue(-123456);
	BigInt positiveValue("123456");
	EXPECT_TRUE(!(negativeValue == positiveValue));
	BigInt positiveString("123456");
	EXPECT_TRUE(positiveString == positiveValue);
}