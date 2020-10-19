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

TEST(Operators, PositiveAdditions) {
	BigInt x = 2;
	x += x;
	EXPECT_EQ(x, 4);
	EXPECT_EQ((BigInt(0) + BigInt(1)), 1);
	EXPECT_EQ((BigInt(3) + BigInt(8)), 11);
	EXPECT_EQ((BigInt(999) + BigInt(1)), 1000);
}

TEST(Operators, PositiveSubtractions) {
	BigInt x = 351;
	x -= x;
	EXPECT_EQ(x, 0);
	EXPECT_EQ((BigInt(1) - BigInt(1)) , 0);
	EXPECT_EQ((BigInt(10) - BigInt(1)) , 9);
	EXPECT_EQ((BigInt(5) - BigInt(8)), -3);
	EXPECT_EQ((BigInt(999) - BigInt(1000)), -1);
}

TEST(Operators, NegativeAdditions) {
	EXPECT_EQ((BigInt(0) + BigInt(-1)), -1);
	EXPECT_EQ((BigInt(-3) + BigInt(8)), 5);
	EXPECT_EQ((BigInt(-999) + BigInt(1)), -998);
}

TEST(Operators, NegativeSubtractions) {
	EXPECT_EQ((BigInt(-1) - BigInt(-1)), 0);
	EXPECT_EQ((BigInt(-10) - BigInt(1)), -11);
	EXPECT_EQ((BigInt(-5) - BigInt(8)), -13);
	EXPECT_EQ((BigInt(999) - BigInt(-1000)), 1999);
}

TEST(Operators, Multiplication) {
	EXPECT_EQ((BigInt(999) * BigInt(1)), 999);
	EXPECT_EQ((BigInt(0) * BigInt(123456)), 0);
	EXPECT_EQ((BigInt(6) * BigInt(6)), 36);
	EXPECT_EQ((BigInt(12) * BigInt(12)), 144);
	EXPECT_EQ((BigInt(12) * BigInt(10)), 120);
	EXPECT_EQ((BigInt(-9) * BigInt(5)), -45);
}