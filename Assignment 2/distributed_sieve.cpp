#include <iostream>
#include <stdlib.h>
#include "mpi.h"
#include "primes_utilities.h"

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
	INDEX_VAR blockLow = 2 + BLOCK_LOW(rank, n - 1, nProcesses); // compensate for [0,1], which we don't want to consider anyway
	INDEX_VAR blockHigh = 2 + BLOCK_HIGH(rank, n - 1, nProcesses); // compensate for [0,1], which we don't want to consider anyway
	INDEX_VAR blockSize = BLOCK_SIZE(rank, n - 1, nProcesses);
	
	bool *primes = createBoolArray(blockSize);
	
	MPI_Barrier(MPI_COMM_WORLD); // Synchronize processes

	if(rank == 0) {
		time = -MPI_Wtime();
	}
	
	INDEX_VAR k = 2;
	while(pow(k, 2) <= n) {
		runSieve(k, blockLow, blockHigh, primes);
		
		if(rank == 0) {
			do {
				k++;
			} while(!primes[k - blockLow] && pow(k, 2) < blockHigh);
		}
	
		MPI_Bcast(&k, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
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
