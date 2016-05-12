#include <iostream>
#include <math.h>
#include <omp.h>
#include <iomanip>
#include <chrono>
#include <vector>

using namespace std;

#define NOW chrono::high_resolution_clock::now()
#define NOW_TO_TIME(time) chrono::duration_cast<std::chrono::microseconds>(time).count()

typedef chrono::time_point<chrono::steady_clock> TIME_VAR;
typedef vector<bool> PRIMES_ARRAY;
typedef time_t SYSTEMTIME;
typedef unsigned long long INDEX_VAR;

void printTimeSpent(TIME_VAR start, TIME_VAR end) {
	auto timeSpent = (end - start);

	cout << "Time spent: " << setprecision(6) << fixed << NOW_TO_TIME(timeSpent) / 1000.0 << " ms" << endl;
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

void calculatePrimesFor(INDEX_VAR limit, int rank, int nProcesses, INDEX_VAR currentNumber, PRIMES_ARRAY & primes) {

	INDEX_VAR arrayLength = limit + 1;
	float calculations = (float)(limit / currentNumber) - 1;
	INDEX_VAR step = (INDEX_VAR)ceil(calculations / nProcesses);
	INDEX_VAR min = 2 + rank * step;
	INDEX_VAR max = (INDEX_VAR)fmin(min + step, (limit / currentNumber) + 1);

	for (INDEX_VAR i = min; i < max; i++) {
		INDEX_VAR index = i * currentNumber;
		primes[i * currentNumber] = false;
	}
}

int sequentialSieve(int n) {

	auto arrayLength = n + 1;
	PRIMES_ARRAY primes(arrayLength);

	for (int i = 0; i < arrayLength; i++)
		primes[i] = true;

	TIME_VAR start = NOW;
	for (int i = 2; i < arrayLength; i++) {

		if (!primes[i])
			continue;

		calculatePrimesFor(n, 0, 1, i, primes);
	}
	TIME_VAR end = NOW;

	printArray(primes, arrayLength);
	printTimeSpent(start, end);

	primes.clear();

	return 0;
}

int parallelSieve(int calcLimit) {
	bool* unmarked = new bool[calcLimit + 1];

	for (int i = 0; i <= calcLimit; i++)
		unmarked[i] = true;

	omp_set_dynamic(0);

	int numThreads = 4;
	int sqrtCalcLimit = (int)sqrt(calcLimit);

	double begin = omp_get_wtime();

	omp_set_num_threads(numThreads);

	for (int i = 2; i <= sqrtCalcLimit; i++) {

		if (!unmarked[i])
			continue;

		int max = calcLimit / i;
		int threadStep = max / numThreads;
		int r = max % numThreads;

#pragma omp parallel num_threads(numThreads)
		{
			int thread = omp_get_thread_num();
			int min = fmax(threadStep * thread + 1, 2);
			int max = threadStep * (thread + 1);
			if (thread == numThreads - 1)
				max += r;

			for (int j = min; j <= max; j++) {
				unmarked[i * j] = false;
			}
		}
	}

	double end = omp_get_wtime();

	// Final Output
	cout << "Output: ";
	int count = 0;
	for (int i = calcLimit; count < 10; i--)
	{
		if (unmarked[i])
		{
			count++;
			cout << i << " ";
		}
	}
	cout << endl << endl;

	cout << "Time spent: " << setprecision(6) << fixed << (end - begin) << " seconds" << endl;

	delete(unmarked);

	return 0;
}

int main() {
	INDEX_VAR algorithm = 0, n = 0;

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
			parallelSieve(n);
			break;

		default:
			break;
		}
	} while (algorithm > 0 && algorithm < 3);

	return 0;
}
