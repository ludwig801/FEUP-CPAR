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
	
			//cout << "i: " << i << " | step: " << threadStep << " | r: " << r;
			//cout << endl;
	
	#pragma omp parallel num_threads(numThreads)
			{
				int thread = omp_get_thread_num();
				int min = fmax(threadStep * thread + 1, 2);
				int max = threadStep * (thread + 1);
				if (thread == numThreads - 1)
					max += r;
	
				//cout << "> thread: " << thread << " | window: " << min << "-" << max;
				//cout << endl;

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
