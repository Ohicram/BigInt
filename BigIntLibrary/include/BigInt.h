#pragma once
#include <cstdint>

#pragma region forward-declarations
class ostream;
class istream;
class vector;
class string;
#pragma endregion

enum class Sign
{
	positive,
	negative
};

class BigInt
{
private:
	Sign _sign;
	std::vector<uint8_t> _digits;
public:
#pragma region constructors
	BigInt();
	BigInt(long long num);
	BigInt(const std::string& s);
#pragma endregion

#pragma region input/output
	//ostream& Print(ostream& os);
	std::ostream& operator<<(std::ostream& out) const;
	//istream& operator>>(istream& in, BigInt& big);
#pragma endregion

#pragma region arithmetic
	//const BigInt& operator-=(const BigInt& rhs);
	//const BigInt& operator+=(const BigInt& rhs);
	//const BigInt& operator*=(const BigInt& rhs);
#pragma endregion

#pragma region comparison
	bool operator==(const BigInt& rhs) const;
	bool operator!=(const BigInt& rhs) const;
	bool operator<(const BigInt& rhs) const;
	bool operator>(const BigInt& rhs) const;
	bool operator<=(const BigInt& rhs) const;
	bool operator>=(const BigInt& rhs) const;
#pragma endregion

#pragma region conversions
	operator std::string() const;
#pragma endregion

private:
#pragma region getters/setters
	// These functions are intended to be modified in case of future refactoring
	// All these functions are defined here to be inline
	size_t num_digits() const
	{
		return _digits.size();
	}
	int get_digit(int k) const
	{
		return _digits[k];
	}
	void change_digit(int k, int value)
	{
		_digits[k] = value;
	}
	void add_digit(int value)
	{
		_digits.push_back(value);
	}
	bool is_positive() const
	{
		return _sign == Sign::positive;
	}
	bool is_negative() const
	{
		return _sign == Sign::negative;
	}
#pragma endregion 
};