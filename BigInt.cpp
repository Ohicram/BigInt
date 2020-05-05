#include "BigInt.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>
BigInt::BigInt()
{
	_digits.push_back(0);
	_isPositive = true;
}

BigInt::BigInt(long long value)
{
	_isPositive = value >= 0;
	while (value > 0) {
		_digits.push_back(value % 10);
		value /= 10;
	}
}

BigInt::BigInt(std::string valueStr)
{
	//@todo: handle different representation such as scientific notation
	_isPositive = true;
	for (auto itStr = valueStr.rbegin(); itStr != valueStr.rend(); ++itStr) {
		if (*itStr >= '0' && *itStr <= '9') {
			_digits.push_back(*itStr - '0');
		}
		else if (*itStr == '-' && itStr == --(valueStr.rend())) {
			_isPositive = false;
		}
		else
		{
			throw std::invalid_argument("Cannot convert the string to BigInt");
		}
	}
}

BigInt BigInt::operator+(const BigInt& other)
{
	//@todo: handle sign difference
	BigInt res;
	res._digits.clear();
	res._digits.reserve(std::max(_digits.size(), other._digits.size() + 1));
	uint8_t sum_remainder = 0;
	for (size_t i = 0; i < std::max(_digits.size(), other._digits.size()); ++i) {
		uint8_t tot = 0;
		if (i < _digits.size() && i < other._digits.size()) {
			tot = _digits[i] + other._digits[i] + sum_remainder;
		}
		else if (i < _digits.size()) {
			tot = _digits[i] + sum_remainder;
		}
		else {
			tot = other._digits[i] + sum_remainder;
		}
		res._digits.push_back(tot % 10);
		sum_remainder = tot / 10;
	}
	if (sum_remainder > 0) {
		res._digits.push_back(sum_remainder);
	}
	return res;
}

std::ostream& operator<<(std::ostream& os, BigInt const& number)
{
	std::ostringstream ostr;
	for (auto itStr = number._digits.rbegin(); itStr != number._digits.rend(); ++itStr) {
		// Not working!     os << *itStr;
		ostr << static_cast<char>(*itStr + '0');
	}
	return os << ostr.str();
}
