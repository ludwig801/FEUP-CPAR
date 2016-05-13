#include <iostream>
#include <omp.h>
#include <iomanip>
#include <time.h>
#include "primes_calculation.h"
#include "primes_utilities.h"

using namespace std;


int sequentialSieve(INDEX_VAR n) {

	INDEX_VAR arrayLength = n + 1;
	PRIMES_ARRAY primes(arrayLength);
	INDEX_VAR nLimit = (INDEX_VAR)sqrt(n);
	TIME_VAR start, end;

	for (INDEX_VAR i = 0; i < arrayLength; i++)
		primes[i] = true;

	clock_gettime(CLOCK_REALTIME, &start);
	for (INDEX_VAR i = 2; i <= nLimit; i++) {

		if (!primes[i])
			continue;

		calculatePrimesFor(n, 0, 1, i, primes);
	}
	clock_gettime(CLOCK_REALTIME, &end);
	
	printArray(primes, arrayLength);
	printTimeSpent(start, end);

	primes.clear();

	return 0;
}

int parallelSieve(INDEX_VAR n, INDEX_VAR numThreads) {

	INDEX_VAR arrayLength = n + 1;
	PRIMES_ARRAY primes(arrayLength);
	INDEX_VAR nLimit = (INDEX_VAR)sqrt(n);
	double start, end;
	
	for (INDEX_VAR i = 0; i < arrayLength; i++)
		primes[i] = true;

	omp_set_dynamic(0);
	if(omp_get_dynamic())
	{
		cout << "Warning: Dinamic allocation of processors is still active!\n";
	}
	
	omp_set_num_threads(numThreads);

	start = omp_get_wtime();
	for (INDEX_VAR i = 2; i <= nLimit; i++) {

		if (!primes[i])
			continue;

#pragma omp parallel num_threads(numThreads)
		{
			calculatePrimesFor(n, omp_get_thread_num(), omp_get_num_threads(), i, primes);
		}
	}
	end = omp_get_wtime();

	printArray(primes, arrayLength);
	printTimeSpent(start, end);

	primes.clear();

	return 0;
}

int main() {
	INDEX_VAR algorithm = 0, n = 0;
	INDEX_VAR nThreads = 0;

	do {
		cout << "Algorithm:" << endl;
		cout << " 1. Sequential" << endl;
		cout << " 2. Parallel" << endl;
		cout << "? ";
		cin >> algorithm;
		if (algorithm < 1 || algorithm > 2)
			break;

		cout << "Limit: pow(2,n)" << endl;
		cout << "n? ";
		cin >> n;

		n = pow(2, n);

		switch (algorithm) {

		case 1:
			sequentialSieve(n);
			break;

		case 2:
			cout << "threads? ";
			cin >> nThreads;
			parallelSieve(n, nThreads);
			break;

		default:
			break;
		}
		
	} while (algorithm > 0 && algorithm < 3);

	return 0;
}
