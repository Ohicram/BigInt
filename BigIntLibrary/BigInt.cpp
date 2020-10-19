#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "include\BigInt.h"

#pragma region constructors
BigInt::BigInt() : m_sign(Sign::positive), m_digits(1, 0)
{
	
}

BigInt::BigInt(long long num) : m_sign(num >= 0 ? Sign::positive : Sign::negative)
{
	// Remove the sign to easily push the digits without weird conversions and log10 operation
	num = std::abs(num);
	// Reserve space for the digits and set to 0 their 
	const int n = num == 0 ? 1 : 1 + static_cast<int>(std::log10(num));
	m_digits.reserve(n);
	do
	{
		m_digits.push_back(num % 10);
		num /= 10;
	} while (num > 0);
}

BigInt::BigInt(const std::string& s) : m_sign((s[0] == '-' && s[1] != '0') ? Sign::negative : Sign::positive)
{
	m_digits.reserve(s.length() + 1);
	// Push back the string in reverse order
	for(auto it = s.rbegin(); it != s.rend(); ++it)
	{
		if((it == s.rend()-1) && *it == '-')
			continue;	// is eventually the last loop...
		if (*it >= '0' && *it <= '9')
			m_digits.push_back(*it - '0');
		else
			throw std::exception("Invalid input format string for BigInt. Only digits [0-9] are allowed.");
	}
}
#pragma endregion



std::ostream& operator<<(std::ostream& out, const BigInt& big)
{
	out << static_cast<std::string>(big);
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

#pragma region operators
const BigInt& BigInt::operator*=(unsigned int num)
{
	// This function perform the multiplication by a single digit (no sign check)
	assert(num <= BIGINT_BASE);

	// Multiplication by zero
	if (num == 0)
	{
		*this = BigInt(0);
		return *this;
	}
	// Multiplication by one
	if(num == 1)
	{
		return *this;
	}
	// General case
	int carry = 0;
	int product = 0;
	for(int i = 0; i < num_digits(); ++i)
	{
		product = num * get_digit(i) + carry;
		carry = product / BIGINT_BASE;
		change_digit(i, product % BIGINT_BASE);
	}
	while(carry > 0)
	{
		add_digit(carry % BIGINT_BASE);
		carry /= BIGINT_BASE;
	}
	return *this;
}

const BigInt& BigInt::operator*=(const BigInt& rhs)
{
	// Compute the sign of the result
	m_sign = m_sign != rhs.m_sign ? Sign::negative : Sign::positive;
	// Copy itself to avoid aliasing
	BigInt self(*this);
	BigInt sum(0);

	for(int i = 0; i < rhs.num_digits(); ++i)
	{
		sum += self * static_cast<unsigned int>(rhs.get_digit(i));
		self*=static_cast<unsigned int>(BIGINT_BASE);
	}
	*this = sum;
	return *this;
}

BigInt operator*(unsigned int num, const BigInt& big)
{
	BigInt result(big);
	result *= num;
	return result;
}

BigInt operator*(const BigInt& big, unsigned int num)
{
	BigInt result(big);
	result *= num;
	return result;
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result *= rhs;
	return result;
}

const BigInt& BigInt::operator++()
{
	*this += BigInt(1);
	return *this;
}

const BigInt& BigInt::operator+=(const BigInt& rhs)
{
	// If the operands do not share the same sign, subtract them
	//		A   +   B
	//	If (-A) + (+B) => (-A) - (-B)
	//  Or (+A) + (-B) => (+A) - (+B)
	if(this->m_sign != rhs.m_sign)
	{
		BigInt temp(rhs);
		temp.m_sign = temp.is_positive() ? Sign::negative : Sign::positive;
		return *this-=temp;
	}
	// The addition algorithm
	// Get a pointer to smaller and larger BigInt operand
	const auto max_n = std::max(this->num_digits(), rhs.num_digits());
	// Perform the operation
	int carry = 0;
	int sum = 0;
		for (auto i = 0; i < max_n; ++i)
		{
			sum = get_digit(i) + rhs.get_digit(i) + carry;
			carry = sum / BIGINT_BASE;
			if (i < num_digits())
				change_digit(i, sum % BIGINT_BASE);
			else
				add_digit(sum % BIGINT_BASE);
		}
		if (carry > 0)
		{
			add_digit(1);
		}
		return *this;
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result += rhs;
	return result;
}

const BigInt& BigInt::operator-=(const BigInt& rhs)
{
	// If they have different sign transform it in an addition by multiplying the rhs by -1
	//		A   -   B
	//	If (-A) - (+B) => (-A) + (-B)
	//  Or (+A) - (-B) => (+A) + (+B)
	if(this->m_sign != rhs.m_sign)
	{
		BigInt temp(rhs);
		temp.m_sign = temp.is_positive() ? Sign::negative : Sign::positive;
		return *this += temp;
	}
	// Handle the subtraction algorithm
	// "swap" the operands to set the greater on the left side
	if(is_positive() && (*this) < rhs || is_negative() && (*this) > rhs)
	{
		// switch operand order
		*this = rhs - *this;
		// switch sign
		m_sign = is_positive() ? Sign::negative : Sign::positive;
		return *this;
	}
	// Now we are in the case that *this is greater than rhs and we can subtract from it
	int borrow = 0;
	int diff = 0;
	for(int i = 0; i < num_digits(); ++i)
	{
		diff = get_digit(i) - rhs.get_digit(i) - borrow;
		if(diff < 0)
		{
			diff += BIGINT_BASE;
			borrow = 1;
		}
		else
		{
			borrow = 0;
		}
		change_digit(i, diff);
	}
	remove_leading_zeros();
	return *this;
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result -= rhs;
	return result;
}

const BigInt& BigInt::operator/=(const BigInt& rhs)
{
	if(rhs == 0)
	{
		throw std::runtime_error("Math error: Attempted to divide by Zero\n");
	}
	
	// Compute the sign
	const Sign result_sign = m_sign != rhs.m_sign ? Sign::negative : Sign::positive;	

	// Simplify the computation striping out the sign
	m_sign = Sign::positive;
	BigInt rhsTemp(rhs);
	rhsTemp.m_sign = Sign::positive;
	// Count how many times rhsTemp is contained in *this
	BigInt counter(0);
	while(*this >= rhsTemp)
	{
		*this -= rhsTemp;
		counter += 1;
	}
	// Restore the correct sign
	counter.m_sign = result_sign;

	*this = std::move(counter);
	return *this;
}

BigInt operator/(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result /= rhs;
	return result;
}

#pragma endregion 

#pragma region comparisons

bool operator==(const BigInt& lhs, const BigInt& rhs)
{
	return (lhs.m_sign == rhs.m_sign && lhs.m_digits == rhs.m_digits);
}

bool operator!=(const BigInt& lhs, const BigInt& rhs)
{
	return !(lhs == rhs);
}

bool operator<(const BigInt& lhs, const BigInt& rhs)
{
	if (lhs.is_negative())
	{
		if (rhs.is_positive())
			return true;
		if (lhs.num_digits() < rhs.num_digits())
			return false;
		else if (lhs.num_digits() == rhs.num_digits())
			return lhs.m_digits > rhs.m_digits;
		else
			return true;
	}
	else
	{
		if (rhs.is_negative())
			return false;
		if (lhs.num_digits() < rhs.num_digits())
			return true;
		else if (lhs.num_digits() == rhs.num_digits())
			return lhs.m_digits < rhs.m_digits;
		else
			return false;
	}
}

bool operator>(const BigInt& lhs, const BigInt& rhs)
{
	if (lhs.is_negative())
	{
		if (rhs.is_positive())
			return false;
		if (lhs.num_digits() < rhs.num_digits())
			return true;
		else if (lhs.num_digits() == rhs.num_digits())
			return lhs.m_digits < rhs.m_digits;
		else
			return false;
	}
	else
	{
		if (rhs.is_negative())
			return true;
		if (lhs.num_digits() < rhs.num_digits())
			return false;
		else if (lhs.num_digits() == rhs.num_digits())
			return lhs.m_digits > rhs.m_digits;
		else
			return true;
	}
}

bool operator<=(const BigInt& lhs, const BigInt& rhs)
{
	return !(lhs > rhs);
}

bool operator>=(const BigInt& lhs, const BigInt& rhs)
{
	return !(lhs < rhs);
}
#pragma endregion

const BigInt& BigInt::remove_leading_zeros()
{
	int elements_to_remove = 0;
	for (int i = static_cast<int>(num_digits()) - 1; i > 0 && get_digit(i) == 0; --i)
		elements_to_remove++;
	m_digits.erase(m_digits.end()-elements_to_remove, m_digits.end());

	// If the result is zero force it to be positive
	if (num_digits() == 1 && get_digit(0) == 0)
		m_sign = Sign::positive;
	
	return *this;
}
