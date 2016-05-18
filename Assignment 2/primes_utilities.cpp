#include "primes_utilities.h"

using namespace std;

void printArray(const bool *primes, INDEX_VAR size) {
	INDEX_VAR count = 10;

	cout << "Array: ";
	for (INDEX_VAR i = size - 1; i >= 2 && count > 0; i--) {
		if (primes[i]) {
			cout << i << " ";
			count--;
		}
	}
	cout << endl << endl;
}

void runSieve(INDEX_VAR k, INDEX_VAR low, INDEX_VAR high, bool *primes) {
	INDEX_VAR startValue;
	INDEX_VAR kSqr = pow(k, 2);

	if(kSqr >= low) {
		startValue = kSqr;
	} else {
		INDEX_VAR r = (low % k);
		
		if(r == 0) {
			startValue = low;
		}
		else {
			startValue = low + (k - r);
		}
	}
	
	
	for(INDEX_VAR i = startValue; i <= high; i += k) {
		primes[i - low] = false;
	}
}

bool *createBoolArray(INDEX_VAR size) {
	bool *array = new bool[size];
	
	for (INDEX_VAR i = 0; i < size; i++)
		array[i] = true;
	
	return array;
}
