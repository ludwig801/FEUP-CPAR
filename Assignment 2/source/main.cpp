#include <iostream>
#include <math.h>

using namespace std;

int sequential_sieve(int calcLimit) {
	bool* unmarked = new bool[calcLimit + 1];

	for (int i = 0; i <= calcLimit; i++)
		unmarked[i] = true;

	for (int i = 2; i <= calcLimit; i++) {
		if (!unmarked[i])
			continue;
		for (int j = 2; i * j <= calcLimit; j++) {
			unmarked[i * j] = false;
		}
	}

	// Final Output
	cout << "Output: ";
	for (int i = 2; i <= calcLimit; i++)
		if (unmarked[i])
			cout << i << " ";
	cout << endl << endl;

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
			sequential_sieve(n);
			break;

		case 2:
			break;

		case 3:
			break;

		default:
			break;
		}
	} while (algorithm > 0 && algorithm < 4);

	return 0;
}
