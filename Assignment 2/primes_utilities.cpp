#include "primes_utilities.h"

using namespace std;

void printTimeSpent(TIME_VAR start, TIME_VAR end) {

	cout << "Time spent: " << setprecision(6) << fixed << ((end.tv_nsec - start.tv_nsec) / 1000000.0) << "ms" << endl;
}

void printTimeSpent(double start, double end) {
	
	double timeSpent = (end - start);

	cout << "Time spent: " << setprecision(6) << fixed << timeSpent * 1000.0 << "ms" << endl;
}

void printArray(const PRIMES_ARRAY & primes, INDEX_VAR length) {
	INDEX_VAR count = 10;

	cout << "Array: ";
	for (INDEX_VAR i = length - 1; i >= 2 && count > 0; i--) {
		if (primes[i]) {
			cout << i << " ";
			count--;
		}
	}
	cout << endl << endl;
}
