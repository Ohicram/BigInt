#pragma once
#include <cstdint>
#include <functional>

#pragma region forward-declarations
class ostream;
class istream;
class vector;
class string;
#pragma endregion

#define BIGINT_BASE 256

enum class Sign
{
	positive,
	negative
};

class BigInt
{
private:
	typedef uint8_t digit_t;
	Sign m_sign;
	std::vector<digit_t> m_digits;
public:
#pragma region constructors
	BigInt();
	BigInt(long long num);
	BigInt(const std::string& s);
#pragma endregion

#pragma region input/output
	//ostream& Print(ostream& os);
	friend std::ostream& operator<<(std::ostream& out, const BigInt& big);
	//istream& operator>>(istream& in, BigInt& big);
#pragma endregion

#pragma region arithmetic
private:
	// This operations are not exposed to the final user, because their
	// functionality is restricted to single digit operation.
	const BigInt& operator*=(unsigned int num);
	friend BigInt operator*(const BigInt&  big, unsigned int num);
	friend BigInt operator*(unsigned int num, const BigInt& big);

	friend void iterative_subtraction_division(const BigInt& lhs, const BigInt& rhs, BigInt& out_quotient, BigInt& out_reminder);
public:
	BigInt operator-() const;
	BigInt operator++(int);
	BigInt operator--(int);
	// Returning const reference avoids complex and error prone syntax (++++x) (x+=y++)
	const BigInt& operator++();
	const BigInt& operator--();
	const BigInt& operator+=(const BigInt& rhs);
	friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
	const BigInt& operator-=(const BigInt& rhs);
	friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
	const BigInt& operator*=(const BigInt& rhs);
	friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
	const BigInt& operator/=(const BigInt& rhs);
	friend BigInt operator/(const BigInt& lhs, const BigInt& rhs);
	const BigInt& operator%=(const BigInt& rhs);
	friend BigInt operator%(const BigInt& lhs, const BigInt& rhs);

	friend BigInt pow(const BigInt& base, const BigInt& exponent);
	friend BigInt pow(const BigInt& base, int exponent);
#pragma endregion

#pragma region bitwise-operators
private:
	void perform_bitwise(const BigInt& rhs, std::function<uint8_t(uint8_t, uint8_t)>);
public:
	const BigInt& operator&=(const BigInt& rhs);
	friend BigInt operator&(const BigInt& lhs, const BigInt& rhs);
	const BigInt& operator|=(const BigInt& rhs);
	friend BigInt operator|(const BigInt& lhs, const BigInt& rhs);
	const BigInt& operator^=(const BigInt& rhs);
	friend BigInt operator^(const BigInt& lhs, const BigInt& rhs);

	BigInt& operator<<=(std::size_t pos);
	BigInt operator<<(std::size_t pos) const;
	BigInt& operator>>=(std::size_t pos);
	BigInt operator>>(std::size_t pos) const;
#pragma endregion 

#pragma region comparison
friend bool operator==(const BigInt& lhs, const BigInt& rhs);
friend bool operator!=(const BigInt& lhs, const BigInt& rhs);
friend bool operator<(const BigInt& lhs, const BigInt& rhs);
friend bool operator>(const BigInt& lhs, const BigInt& rhs);
friend bool operator<=(const BigInt& lhs, const BigInt& rhs);
friend bool operator>=(const BigInt& lhs, const BigInt& rhs);
#pragma endregion
	
#pragma region conversions
	operator std::string() const;
#pragma endregion

private:
	const BigInt& remove_leading_zeros();
#pragma region getters/setters
	// These functions are intended to be modified in case of future refactoring
	// All these functions are defined here to be inline
	size_t num_digits() const
	{
		return m_digits.size();
	}
	unsigned int get_digit(int k) const
	{
		return k < num_digits() ? m_digits[k] : 0;
	}
	void change_digit(int k, int value)
	{
		m_digits[k] = value;
	}
	void add_digit(int value)
	{
		m_digits.push_back(value);
	}
	bool is_positive() const
	{
		return m_sign == Sign::positive;
	}
	bool is_negative() const
	{
		return m_sign == Sign::negative;
	}
#pragma endregion 
};

