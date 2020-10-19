#pragma once
#include <cstdint>

#pragma region forward-declarations
class ostream;
class istream;
class vector;
class string;
#pragma endregion

#define BIGINT_BASE 10

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
	BigInt& operator=(const BigInt&) = default;
#pragma endregion

#pragma region input/output
	//ostream& Print(ostream& os);
	friend std::ostream& operator<<(std::ostream& out, const BigInt& big);
	//istream& operator>>(istream& in, BigInt& big);
#pragma endregion

#pragma region arithmetic
private:
	const BigInt& operator*=(unsigned int num);
	friend BigInt operator*(const BigInt&  big, unsigned int num);
	friend BigInt operator*(unsigned int num, const BigInt& big);
public:
	const BigInt& operator++();
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

