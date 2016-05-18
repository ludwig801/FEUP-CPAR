#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
#include "primes_utilities.h"
#include "distributed_sieve.h"

using namespace std;

INDEX_VAR convertToIndex(INDEX_VAR n)
{
	return (n - 3) >> 1;
}

INDEX_VAR convertIndexToNumber(INDEX_VAR i)
{
	return 2 * i + 3;
}

void distributedSieve(int power) {
	
	double time = 0;
	int rank, nProcesses;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	
	INDEX_VAR n = pow(2, power);
	INDEX_VAR blockLow = BLOCK_LOW(rank, n - 1, nProcesses) + 2;
	INDEX_VAR blockHigh = BLOCK_HIGH(rank, n - 1, nProcesses) + 2;
	INDEX_VAR blockSize = BLOCK_SIZE(rank, n - 1, nProcesses);
	
	bool *primes = new bool[blockSize];
	for(INDEX_VAR i = 0; i < blockSize; i++) {
		primes[i] = true;
	}
	
	MPI_Barrier(MPI_COMM_WORLD); // Synchronize processes

	if(rank == 0) {
		time = -MPI_Wtime();
	}
	
	INDEX_VAR k = 2;
	INDEX_VAR kSqr = pow(k, 2);
	INDEX_VAR startValue;
	
	while(kSqr <= n) {
		// calculate the start block value to each process
		if (kSqr < blockLow) {
			INDEX_VAR r = (blockLow % k);
			
			if(r == 0) {
				startValue = blockLow;
			}
			else {
				startValue = blockLow + (k - r);
			}
					
		} else {
			startValue = kSqr;
		}
		
		for(INDEX_VAR i = startValue; i <= blockHigh; i += k) {
			primes[i - blockLow] = false;
		}
		
		if(rank == 0) {
			do {
				k++;
			} while(!primes[k - blockLow] && pow(k, 2) < blockHigh);
		}
		
		// Send the next prime to other processes
		MPI_Bcast(&k, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
		
		kSqr = pow(k, 2);
	}
	
	INDEX_VAR primeCount = 0;
	for (INDEX_VAR i = 0; i < blockSize; i++) {
		if (primes[i]) {
			primeCount++;
		}
	}

	INDEX_VAR totalPrimeCount = primeCount;

	if (nProcesses > 1) {
		MPI_Reduce(&primeCount, &totalPrimeCount, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	
	if(rank == 0) {
		time += MPI_Wtime();
		cout << "Primes: " << totalPrimeCount << endl;
		cout << "Time spent: " << (time * 1000.0) << "ms\n";
	}
	
	free(primes);
}

int main(int argc, char **argv) {
	
	INDEX_VAR power = (INDEX_VAR)atoll(argv[1]);
	
	MPI_Init(&argc, &argv);

	distributedSieve(power);
	
	MPI_Finalize();

	return 0;
}
