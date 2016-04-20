#include <iostream>
#include <math.h>
#include <omp.h>
#include <iomanip>

using namespace std;

#define SYSTEMTIME clock_t

int sequentialSieve(int calcLimit) {
	bool* unmarked = new bool[calcLimit + 1];

	for (int i = 0; i <= calcLimit; i++)
		unmarked[i] = true;

	SYSTEMTIME begin = clock();

	for (int i = 2; i <= calcLimit; i++) {
		if (!unmarked[i])
			continue;

		for (int j = 2; i * j <= calcLimit; j++) {
			unmarked[i * j] = false;
		}
	}

	SYSTEMTIME end = clock();

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

	cout << "Time spent: " << setprecision(6) << fixed << (double)(end - begin) / CLOCKS_PER_SEC << " seconds" << endl;

	delete(unmarked);

	return 0;
}

int parallelSieve(int calcLimit) {
	bool* unmarked = new bool[calcLimit + 1];

	for (int i = 0; i <= calcLimit; i++)
		unmarked[i] = true;

	int numThreads = omp_get_num_threads();
	int step = calcLimit / numThreads;

	double begin = omp_get_wtime();

#pragma omp parallel for private(j)
	for (int i = (omp_get_thread_num() * step) + 1; i <= omp_get_thread_num() * (step + 1); i++) {
		if (i < 2)
			continue;
		
		if (!unmarked[i])
			continue;

		for (int j = 2; i * j <= calcLimit; j++) {
			unmarked[i * j] = false;
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
	int algorithm = 0, n = 0;

	do {

		cout << "Algorithm:" << endl;
		cout << " 1. Sequential" << endl;
		cout << " 2. Parallel" << endl;
		cout << " 3. Distributed Parallel" << endl;
		cout << "? ";
		cin >> algorithm;
		if (algorithm < 1 || algorithm > 3)
			break;

		cout << "Limit: ";
		cin >> n;

		switch (algorithm) {

		case 1:
			sequentialSieve(n);
			break;

		case 2:
			parallelSieve(n);
			break;

		case 3:
			break;

		default:
			break;
		}
	} while (algorithm > 0 && algorithm < 4);

	return 0;
}
