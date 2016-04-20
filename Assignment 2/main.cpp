#include <iostream>
#include <math.h>

int main() {
	int n;

	std::cout << "n=";
	std::cin >> n;

	int calcLimit = (n);
	bool unmarked[calcLimit+1];
	
	for(int i = 0; i <= calcLimit; i++)
		unmarked[i] = true;
	
	for(int i = 2; i <= calcLimit; i++) {
		if(!unmarked[i])
			continue;
		for(int j = 2; i * j <= calcLimit; j++) {
			unmarked[i * j] = false;
		}
	}
	
	//final output
	std::cout << "Primes: ";
	for(int i = 2; i <= calcLimit; i++)
		if(unmarked[i])
			std::cout << i << " ";
	std::cout << std::endl;

	return 0;
}
