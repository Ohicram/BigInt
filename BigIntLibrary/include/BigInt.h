#pragma once

#include <vector>
#include <string>
/*
 *	BigInt is implemented through a vector of byte ordered in Little Endian
 *  each element of the vector is a digit of the BigInt.
 */
class BigInt
{
private:
	bool _isPositive;
	std::vector<uint8_t> _digits;
public:
	//Constructors
	BigInt();
	BigInt(long long);
	BigInt(std::string);
	//@todo: implements rule of five
	//	Bigint(const Bigint& b);
	//Compare
	//bool operator<(const BigInt&) const;
	//bool operator>(const BigInt&) const;
	//bool operator<=(const BigInt&) const;
	//bool operator>=(const BigInt&) const;
	bool operator==(const BigInt&) const;
	bool operator!=(const BigInt&) const;
	//Adding
	BigInt operator+ (const BigInt& other);
	//	BigInt operator+(BigInt const&) const;
	/*	Bigint& operator+=(Bigint const&);
		Bigint operator+(long long const&) const;
		Bigint& operator+=(long long);

		//Subtraction
		Bigint operator-(Bigint const&) const;
		Bigint& operator-=(Bigint const&);

		//Multiplication
		Bigint operator*(Bigint const&);
		Bigint& operator*=(Bigint const&);
		Bigint operator*(long long const&);
		Bigint& operator*=(int const&);



		//Allocation
		Bigint operator=(const long long&);

		//Access
		int operator[](int const&);
	*/
	//Input&Output
//	friend std::istream& operator>>(std::istream&, Bigint&);
	friend std::ostream& operator<<(std::ostream&, BigInt const&);
	/*
		//Helpers
		void clear();
		Bigint& abs();

		//Power
		Bigint& pow(int const&);

		//Trivia
		int digits() const;
		int trailing_zeros() const;
	private:
		int segment_length(int) const;
		Bigint pow(int const&, std::map<int, Bigint>&);
		int compare(Bigint const&) const; //0 a == b, -1 a < b, 1 a > b
	};

	Bigint abs(Bigint);
	std::string to_string(Bigint const&);
	Bigint factorial(int);*/

};

