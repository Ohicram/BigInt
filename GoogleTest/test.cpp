#include "pch.h"

#include "BigInt.h"
#include <string>

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
	EXPECT_TRUE(BigInt(-10) < BigInt(-9));
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
	EXPECT_EQ((BigInt(99) + BigInt(1)), 100);
	EXPECT_EQ((BigInt(3) + BigInt(8)), 11);
	EXPECT_EQ((BigInt(999) + BigInt(1)), 1000);
	BigInt y = 0;
	for(int i = 0; i < 123; ++i)
	{
		y += 1;
	}
	EXPECT_EQ(y, 123);
}

TEST(Operators, MinusSign) {
	BigInt x = -8;
	EXPECT_EQ(-x, 8);
	EXPECT_EQ(-x-8, 0);
	BigInt y = 8;
	EXPECT_EQ(-y, -8);
	EXPECT_EQ(-y+8, 0);
}

TEST(Operators, IncrementDecrementOperators) {
	BigInt x = 0;
	EXPECT_EQ(x++, 0);
	EXPECT_EQ(++x, 2);
	EXPECT_EQ(--x, 1);
	EXPECT_EQ(x--, 1);
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

TEST(Operators, Division) {
	EXPECT_EQ((BigInt(100) / BigInt(5)), 20);
	EXPECT_EQ((BigInt(100) / BigInt(100)), 1);
	EXPECT_EQ((BigInt(100) / BigInt(1)), 100);
	EXPECT_EQ((BigInt(81) / BigInt(9)), 9);

	EXPECT_EQ((BigInt(256) / BigInt(-4)), -64);
	EXPECT_EQ((BigInt(-3) / BigInt(2)), -1);
	EXPECT_EQ((BigInt(-89) / BigInt(-9)), 9);
	EXPECT_EQ((BigInt(0) / BigInt(129)), 0);
	
	EXPECT_ANY_THROW((BigInt(89) / BigInt(0)));
}

TEST(Operators, Modulo) {
	EXPECT_EQ((BigInt(100) % BigInt(97)), 3);
	EXPECT_EQ((BigInt(100) % BigInt(100)), 0);
	EXPECT_EQ((BigInt(1023) % BigInt(2)), 1);
	EXPECT_EQ((BigInt(123) % BigInt(100)), 23);

	EXPECT_EQ((BigInt(256) % BigInt(-4)), 0);
	EXPECT_EQ((BigInt(-3) % BigInt(2)), -1);
	EXPECT_EQ((BigInt(-89) % BigInt(-9)), 8);
	EXPECT_EQ((BigInt(0) % BigInt(129)), 0);

	EXPECT_ANY_THROW((BigInt(89) % BigInt(0)));
}

TEST(Math, Power) {
	EXPECT_EQ(pow(BigInt(2), 2), 4);
	EXPECT_EQ(pow(BigInt(2), 8), 256);
	EXPECT_EQ(pow(BigInt(2), 0), 1);
	EXPECT_EQ(pow(BigInt(1), 8), 1);
	EXPECT_EQ(pow(BigInt(0), 8), 0);
	EXPECT_EQ(pow(BigInt(0), BigInt(2)), 0);
	EXPECT_EQ(pow(BigInt(3), BigInt(3)), 81);
	EXPECT_EQ(pow(BigInt(3), BigInt(0)), 1);
	EXPECT_ANY_THROW(pow(BigInt(2), -1));
}

TEST(Conversions, ToString) {
	BigInt x = 129;
	std::string s_x = x;
	EXPECT_STREQ(s_x.c_str(), "129");
	BigInt y = -9129;
	std::string s_y = y;
	EXPECT_STREQ(s_y.c_str(), "-9129");
}

TEST(Bitwise, And)
{
	EXPECT_EQ(BigInt(123) & BigInt(122), 122);
	EXPECT_EQ(BigInt(74129) & BigInt(1), BigInt(1));
	EXPECT_EQ(BigInt(1) & BigInt(74129), BigInt(1));
	EXPECT_EQ(BigInt("123456789") & BigInt("123456789"), BigInt("123456789"));
}