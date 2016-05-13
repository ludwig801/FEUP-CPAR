#ifndef PRIMES_CALCULATION_H
#define PRIMES_CALCULATION_H

#include "types.h"
#include <iomanip>
#include <iostream>
#include <math.h>

int calculatePrimesFor(INDEX_VAR n, int rank, int nProcesses,
	INDEX_VAR currentNumber, PRIMES_ARRAY & primes);

#endif
