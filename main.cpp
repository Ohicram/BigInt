#include "BigInt.h"

#include <iostream>

int main() {
	BigInt bl(1);
	BigInt bs{"99999999999999999999999999999999999999999999999999999999"};
	std::cout << bl << " + " << bs << " = " << bl + bs << std::endl;
	return 0;
}