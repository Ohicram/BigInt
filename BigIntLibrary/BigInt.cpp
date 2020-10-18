#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "include\BigInt.h"

#pragma region constructors
BigInt::BigInt() : _sign(Sign::positive), _digits(1, 0)
{
	
}

BigInt::BigInt(long long num) : _sign(num >= 0 ? Sign::positive : Sign::negative)
{
	// Remove the sign to easily push the digits without weird conversions and log10 operation
	num = std::abs(num);
	// Reserve space for the digits and set to 0 their 
	const int n = num == 0 ? 1 : 1 + static_cast<int>(std::log10(num));
	_digits.reserve(n);
	do
	{
		_digits.push_back(num % 10);
		num /= 10;
	} while (num > 0);
}

BigInt::BigInt(const std::string& s) : _sign((s[0] == '-' && s[1] != '0') ? Sign::negative : Sign::positive)
{
	_digits.reserve(s.length() + 1);
	// Push back the string in reverse order
	for(auto it = s.rbegin(); it != s.rend(); ++it)
	{
		if((it == s.rend()-1) && *it == '-')
			continue;	// is eventually the last loop...
		if (*it >= '0' && *it <= '9')
			_digits.push_back(*it - '0');
		else
			throw std::exception("Invalid input format string for BigInt. Only digits [0-9] are allowed.");
	}
}
#pragma endregion



std::ostream& BigInt::operator<<(std::ostream& out) const
{
	out << static_cast<std::string>(*this);
	return out;
}

#pragma region conversions
/*
 * This function convert a BigInt to std::string
 */
BigInt::operator std::string() const
{
	std::stringstream s("");
	if (is_negative())
		s << "-";
	for(int k = 0; k < num_digits(); ++k)
	{
		s << static_cast<char>('0' + get_digit(k));
	}
	return s.str();
}
#pragma endregion


bool BigInt::operator==(const BigInt& rhs) const
{
	return (this->_sign == rhs._sign && this->_digits == rhs._digits);
}

bool BigInt::operator!=(const BigInt& rhs) const
{
	return !(*this == rhs);
}

bool BigInt::operator<(const BigInt& rhs) const
{
	if(is_negative())
	{
		if (rhs.is_positive())
			return true;
		if (num_digits() < rhs.num_digits())
			return false;
		else if (num_digits() == rhs.num_digits())
			return _digits > rhs._digits;
		else
			return true;
	}
	else
	{
		if (rhs.is_negative())
			return false;
		if (num_digits() < rhs.num_digits())
			return true;
		else if (num_digits() == rhs.num_digits())
			return _digits < rhs._digits;
		else
			return false;
	}
}

bool BigInt::operator>(const BigInt& rhs) const
{
	if (is_negative())
	{
		if (rhs.is_positive())
			return false;
		if (num_digits() < rhs.num_digits())
			return true;
		else if (num_digits() == rhs.num_digits())
			return _digits < rhs._digits;
		else
			return false;
	}
	else
	{
		if (rhs.is_negative())
			return true;
		if (num_digits() < rhs.num_digits())
			return false;
		else if (num_digits() == rhs.num_digits())
			return _digits > rhs._digits;
		else
			return true;
	}
}

bool BigInt::operator<=(const BigInt& rhs) const
{
	return !(*this > rhs);
}


bool BigInt::operator>=(const BigInt& rhs) const
{
	return !(*this < rhs);
}
