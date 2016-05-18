#ifndef _PRIMES_UTILITIES_H_
#define _PRIMES_UTILITIES_H_

#include <math.h>
#include "types.h"

#define BLOCK_LOW(i, n, p) ((i) * (n) / (p))
#define BLOCK_HIGH(i, n, p) (BLOCK_LOW((i) + 1, n, p) - 1)
#define BLOCK_SIZE(i, n, p) (BLOCK_LOW((i) + 1, n, p) - BLOCK_LOW(i, n, p))
#define BLOCK_OWNER(index, n, p) ((((p) * (index) + 1) - 1) / (n))

void printArray(const bool *primes, INDEX_VAR length);

void runSieve(INDEX_VAR k, INDEX_VAR low, INDEX_VAR high, bool *primes);

bool *createBoolArray(INDEX_VAR size);

#endif
