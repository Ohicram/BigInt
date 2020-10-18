#include "pch.h"
#include "BigInt.h"

TEST(Constructors, EmptyConstructors) {
	EXPECT_NO_THROW(BigInt bi);
}

TEST(Constructors, BigIntLongLongConstructors) {
	EXPECT_NO_THROW(BigInt bi(0));
	EXPECT_NO_THROW(BigInt bi(123));
	EXPECT_NO_THROW(BigInt bi(-1));
	EXPECT_NO_THROW(BigInt bi(-0));
	EXPECT_NO_THROW(BigInt bi(-123456789));
}

TEST(Constructors, BigIntStringConstructors) {
	EXPECT_ANY_THROW(BigInt bi("pippO"));
	EXPECT_ANY_THROW(BigInt bi("--123"));
	EXPECT_ANY_THROW(BigInt bi("123-"));
	EXPECT_ANY_THROW(BigInt bi("+123"));
	EXPECT_NO_THROW(BigInt bi("123"));
	EXPECT_NO_THROW(BigInt bi("-123"));
	EXPECT_NO_THROW(BigInt bi("0"));
	EXPECT_NO_THROW(BigInt bi("-0"));
}

TEST(ComparisonOperators, Equality) {
	const BigInt void_val;
	EXPECT_TRUE(void_val == void_val);
	const BigInt long_string{ "123456789102030405060708090100" };
	EXPECT_TRUE(long_string == long_string);
	const BigInt long_value(123456789);
	EXPECT_TRUE(long_value == long_value);
	const BigInt negative_value(-123456);
	const BigInt negative_string("-123456");
	EXPECT_TRUE(negative_value == negative_string);
	const BigInt positive_value(123456);
	const BigInt positive_string("123456");
	EXPECT_TRUE(positive_string == positive_value);

	EXPECT_TRUE(BigInt("-1") != BigInt("1"));
	EXPECT_TRUE(BigInt(-1) != BigInt(1));
	
	EXPECT_TRUE(BigInt(0) == BigInt(-0));
	EXPECT_TRUE(BigInt("0") == BigInt("-0"));
}

TEST(ComparisonOperators, LessThan) {
	EXPECT_TRUE(BigInt("-10") < BigInt("-9"));
	EXPECT_TRUE(BigInt("-9") < BigInt("-3"));
	EXPECT_TRUE(BigInt("-10") < BigInt("10"));
	EXPECT_TRUE(BigInt("0") < BigInt("10"));
	EXPECT_FALSE(BigInt("0") < BigInt("0"));
	EXPECT_TRUE(BigInt("5") < BigInt("8"));
	EXPECT_TRUE(BigInt("5") < BigInt("10"));
}

TEST(ComparisonOperators, GreaterThan) {
	EXPECT_FALSE(BigInt("-10") > BigInt("-9"));
	EXPECT_TRUE(BigInt("-1") > BigInt("-3"));
	EXPECT_TRUE(BigInt("10") > BigInt("-10"));
	EXPECT_TRUE(BigInt("0") > BigInt("-10"));
	EXPECT_FALSE(BigInt("0") > BigInt("0"));
	EXPECT_TRUE(BigInt("5") > BigInt("3"));
	EXPECT_TRUE(BigInt("10") > BigInt("9"));
}

TEST(ComparisonOperators, LessThanEqual) {
	EXPECT_TRUE(BigInt("-10") <= BigInt("-9"));
	EXPECT_TRUE(BigInt("-9") <= BigInt("-3"));
	EXPECT_TRUE(BigInt("-10") <= BigInt("10"));
	EXPECT_TRUE(BigInt("0") <= BigInt("10"));
	EXPECT_TRUE(BigInt("0") <= BigInt("0"));
	EXPECT_TRUE(BigInt("5") <= BigInt("8"));
	EXPECT_TRUE(BigInt("5") <= BigInt("10"));
}