#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
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
	if(num < BIGINT_BASE)
	{
		m_digits = std::vector<digit_t>(1, num);
		return;
	}
	if(num == BIGINT_BASE)
	{
		m_digits = { 0, 1 };
		return;
	}
	// Reserve space for the digits and set to 0 their 
	//const int n = num == 0 ? 1 : 1 + static_cast<int>(std::log10(num));
	//m_digits.reserve(n);
	BigInt temp = 0;
	BigInt base = 1;
	do
	{
		//m_digits.push_back(num % 10);
		temp += base*(num % 10);
		num /= 10;
		base *= 10;
	} while (num > 0);
	temp.m_sign = m_sign;
	std::swap(*this, temp);
}

BigInt::BigInt(const std::string& s) : m_sign((s[0] == '-' && s[1] != '0') ? Sign::negative : Sign::positive), m_digits(1, 0)
{
	//m_digits.reserve(s.length() + 1);
	// Push back the string in reverse order
	BigInt temp = 0;
	BigInt base = 1;
	for(auto it = s.rbegin(); it != s.rend(); ++it)
	{
		if((it == s.rend()-1) && *it == '-')
			continue;	// is eventually the last loop...
		if (*it >= '0' && *it <= '9')
		{
			temp += base * (*it - '0');
			base *= 10;
			//m_digits.push_back(*it - '0');
		}
		else
			throw std::exception("Invalid input format string for BigInt. Only digits [0-9] are allowed.");
	}
	temp.m_sign = m_sign;
	std::swap(*this, temp);
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
	//if (is_negative())
	//	s << "-";
	/*for(int k = num_digits()-1; k >= 0; --k)
	{
		s << static_cast<char>('0' + get_digit(k));
	}*/
	BigInt temp(*this);
	temp.m_sign = Sign::positive;
	do
	{
		//m_digits.push_back(num % 10);
		BigInt remind = temp % 10;
		s << static_cast<char>('0' + remind.get_digit(0));
		temp /= 10;
	} while (temp > 0);
	if (is_negative())
		s << "-";
	const std::string reversed_string = s.str();
	// Revert the string for human-reading
	return std::string(reversed_string.rbegin(), reversed_string.rend());
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

BigInt BigInt::operator-()
{
	BigInt result{ *this };
	result.m_sign = result.m_sign == Sign::positive ? Sign::negative : Sign::positive;
	return result;
}

const BigInt& BigInt::operator++()
{
	*this += BigInt(1);
	return *this;
}

const BigInt& BigInt::operator--()
{
	*this -= BigInt(1);
	return *this;
}

BigInt BigInt::operator++(int)
{
	BigInt temp{ *this };
	operator++();
	return temp;
}

BigInt BigInt::operator--(int)
{
	BigInt temp{ *this };
	operator--();
	return temp;
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
	// If the result is zero force it to be positive
	if (num_digits() == 1 && get_digit(0) == 0)
		m_sign = Sign::positive;
	*this = std::move(counter);
	return *this;
}

BigInt operator/(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result /= rhs;
	return result;
}

const BigInt& BigInt::operator%=(const BigInt& rhs)
{
	if (rhs == 0)
	{
		throw std::runtime_error("Math error: Attempted to divide by Zero\n");
	}

	// Compute the sign
	const Sign result_sign = m_sign != rhs.m_sign ? Sign::negative : Sign::positive;

	// Simplify the computation striping out the sign
	m_sign = Sign::positive;
	BigInt rhsTemp(rhs);
	rhsTemp.m_sign = Sign::positive;
	// Reduce itself until...
	while (*this >= rhsTemp)
	{
		*this -= rhsTemp;
	}
	// Restore the correct sign
	m_sign = result_sign;
	// If the result is zero force it to be positive
	if (num_digits() == 1 && get_digit(0) == 0)
		m_sign = Sign::positive;
	return *this;
}

BigInt operator%(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result %= rhs;
	return result;
}

BigInt pow(const BigInt& base, const BigInt& exponent)
{
	if (exponent < 0)
	{
		throw std::exception("Negative exponents are not supported for BigInt types.");
	}
	if (exponent == 0)
	{
		return 1;
	}
	// Continue with the "normal" cases
	BigInt result{ base };
	for (BigInt i = 0; i < exponent; ++i)
	{
		result *= base;
	}
	return result;
}

BigInt pow(const BigInt& base, int exponent)
{
	if (exponent < 0)
	{
		throw std::exception("Negative exponents are not supported for BigInt types.");
	}
	if (exponent == 0)
	{
		return 1;
	}
	// Continue with the "normal" cases
	BigInt result{ base };
	for (BigInt i = 1; i < exponent; ++i)
	{
		result *= base;
	}
	return result;
}

#pragma endregion 

#pragma region bitwise-operators

void BigInt::perform_bitwise(const BigInt& rhs, std::function<uint8_t(uint8_t, uint8_t)> bw_operator)
{
	const auto size_r = rhs.m_digits.size();
	const auto size_l = m_digits.size();
	for (int i = 0; i < std::max(size_r, size_l); ++i)
	{
		const auto digit_r = i < size_r ? rhs.get_digit(i) : 0;
		const auto digit_l = i < size_l ? get_digit(i) : 0;
		if (i < size_l)
		{
			m_digits[i] = bw_operator(digit_l, digit_r);
		}
		else
		{
			m_digits.push_back(bw_operator(digit_l, digit_r));
		}
	}
	remove_leading_zeros();
}

const BigInt& BigInt::operator&=(const BigInt& rhs)
{
	/*const auto size_r = rhs.m_digits.size();
	const auto size_l = m_digits.size();
	for(int i = 0; i < std::max(size_r, size_l); ++i)
	{
		const auto digit_r = i < size_r ? rhs.get_digit(i) : 0;
		const auto digit_l = i < size_l ? get_digit(i) : 0;
		if(i < size_l)
		{
			m_digits[i] = digit_l & digit_r;
		}
		else
		{
			m_digits.push_back(digit_l & digit_r);
		}
	}
	remove_leading_zeros();
	return *this;*/
	perform_bitwise(rhs, std::bit_and<uint8_t>());
	return *this;
}

BigInt operator&(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result &= rhs;
	return result;
}

const BigInt& BigInt::operator|=(const BigInt& rhs)
{
	perform_bitwise(rhs, std::bit_or<uint8_t>());
	return *this;
}

BigInt operator|(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result |= rhs;
	return result;
}

BigInt operator^(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result(lhs);
	result ^= rhs;
	return result;
}

const BigInt& BigInt::operator^=(const BigInt& rhs)
{
	perform_bitwise(rhs, std::bit_xor<uint8_t>());
	return *this;
}

BigInt& BigInt::operator<<=(std::size_t pos)
{
	size_t extra = 0;
	// Define the mask for the bit in the current digit
	const size_t digit_mask = extra | ~digit_t{ 0 };
	// Shift the maximum and re-iterate
	while (pos > 0)
	{
		size_t shiftpos = 0;
		if(pos >= sizeof(size_t)*8)
		{
			shiftpos = (sizeof(size_t) * 8)/2;
			pos -= (sizeof(size_t) * 8)/2;
		}
		else
		{
			shiftpos = pos;
			pos = 0;
		}
		for (int i = 0; i < m_digits.size(); ++i)
		{
			size_t d = get_digit(i);
			size_t d_shifted = d << shiftpos;
			// The shifted bits should contains the bits shifted from the previous digit
			size_t new_d = (d_shifted | extra) & digit_mask;
			m_digits[i] = static_cast<digit_t>(new_d);
			// The bits that are shifted on the space for the next digits, are moved back
			// in the space of current digit, for the next iteration
			extra = d_shifted;
			extra >>= (8 * sizeof(digit_t));
		}
		// If the shifted bit exceeds the current capacity, add then at the end
		while (extra > 0)
		{
			m_digits.push_back(static_cast<digit_t>(extra & digit_mask));
			extra >>= (8 * sizeof(digit_t));
		}
	}
	return *this;
}

BigInt BigInt::operator<<(std::size_t pos) const
{
	BigInt result(*this);
	result <<= pos;
	return result;
}

BigInt& BigInt::operator>>=(std::size_t pos)
{
	size_t extra = 0;
	// Define the mask for the bit in the next digit
	const size_t extra_mask = extra | ~digit_t{0};
	// Define the mask for the bit in the current digit
	const size_t digit_mask = ~extra_mask;
	for (int i = m_digits.size()-1; i >= 0; --i)
	{
		size_t d = get_digit(i);
		size_t d_shifted = d << pos;
		// The shifted bits should contains the bits shifted from the previous digit
		size_t new_d = (d_shifted | extra) & digit_mask;
		m_digits[i] = static_cast<digit_t>(new_d);
		// The bits that are shifted on the space for the next digit, are moved back
		// in the space of current digit, for the next iteration
		extra = d_shifted & extra_mask;
		extra >>= (8 * sizeof(digit_t));
	}
	// If the shifted bit exceeds the current capacity, add then at the end
	if (extra > 0)
	{
		m_digits.push_back(static_cast<digit_t>(extra));
	}
	return *this;
}

BigInt BigInt::operator>>(std::size_t pos) const
{
	BigInt result(*this);
	result <<= pos;
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
