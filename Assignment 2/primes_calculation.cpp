#include "primes_calculation.h"

int calculatePrimesFor(INDEX_VAR n, int rank, int nProcesses,
	INDEX_VAR currentNumber, PRIMES_ARRAY & primes) {

	float calculations = (n / currentNumber) - 1;
	INDEX_VAR step = (INDEX_VAR) ceil(calculations / nProcesses);
	
	INDEX_VAR min = 2 + rank * step;
	INDEX_VAR max = fmin(min + step, (n / currentNumber) + 1);

	for (INDEX_VAR i = min; i < max; i++) {
		primes[i * currentNumber] = false;
	}
	
	return 0;
}
