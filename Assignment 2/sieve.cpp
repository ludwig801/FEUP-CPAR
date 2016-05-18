#include <iostream>
#include <omp.h>
#include <iomanip>
#include <time.h>
#include "primes_utilities.h"

using namespace std;

void sequentialSieve(INDEX_VAR power) {

	INDEX_VAR n = pow(2, power);
	INDEX_VAR size = n + 1;
	INDEX_VAR nLimit = (INDEX_VAR)sqrt(n);
	TIME_VAR start, end;
	bool *primes = createBoolArray(size);

	clock_gettime(CLOCK_REALTIME, &start);
	for (INDEX_VAR i = 2; i <= nLimit; i++) {

		if (!primes[i])
			continue;

		runSieve(i, 0, size, primes);
	}
	clock_gettime(CLOCK_REALTIME, &end);

	INDEX_VAR primeCount = 0;
	for(INDEX_VAR i = size - 1; i >= 2; i--) {
		if(primes[i]) {
			primeCount++;
			if(primeCount < 10) {
				cout << i << " ";
			}
		}
	}
	cout << endl;
	cout << "Primes: " << primeCount << endl;
	
	double deltaTime = (end.tv_sec - start.tv_sec) * 1000.0 + ((end.tv_nsec - start.tv_nsec) / 1000000.0);
	
	cout << "Time spent: " << setprecision(6) << fixed << deltaTime << "ms" << endl;
	
	free(primes);
}

void parallelSieve(INDEX_VAR power, INDEX_VAR numThreads) {

	INDEX_VAR n = pow(2, power);
	INDEX_VAR nLimit = (INDEX_VAR)sqrt(n);
	INDEX_VAR size = n + 1;
	double start, end;
	bool *primes = createBoolArray(size);

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
			int tid = omp_get_thread_num();
			// +2 --> compensate for [0,1], which we don't want to consider anyway
			INDEX_VAR blockLow = 2 + BLOCK_LOW(tid, n - 1, numThreads);
			INDEX_VAR blockHigh = 2 + BLOCK_HIGH(tid, n - 1, numThreads);

			runSieve(i, blockLow, blockHigh, &(primes[blockLow]));
		}
	}
	end = omp_get_wtime();
	
	INDEX_VAR primeCount = 0;
	for(INDEX_VAR i = size - 1; i >= 2; i--) {
		if(primes[i]) {
			primeCount++;
			if(primeCount < 10) {
				cout << i << " ";
			}
		}
	}
	cout << endl;
	cout << "Primes: " << primeCount << endl;

	cout << "Time spent: " << setprecision(6) << fixed << (double)((end - start) * 1000.0) << "ms" << endl;
	
	free(primes);
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
