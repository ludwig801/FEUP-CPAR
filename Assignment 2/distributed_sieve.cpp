#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
#include "primes_calculation.h"
#include "primes_utilities.h"

using namespace std;

int distributedSieve(INDEX_VAR n, int rank, int nProcesses) {
	MPI_Status status;

	INDEX_VAR arrayLength = n + 1;
	PRIMES_ARRAY primes(arrayLength);
	INDEX_VAR nLimit = (INDEX_VAR)sqrt(n);
	INDEX_VAR currentNumber = 2;
	bool isNextPrime = true;
	
	for (INDEX_VAR i = 0; i < arrayLength; i++)
		primes[i] = true;

	// Parent process
	if (rank == 0) {
		TIME_VAR start, end;
		bool isOthersPrime = false;
		
		clock_gettime(CLOCK_REALTIME, &start);
		do {
			// Check if next is prime
			isNextPrime = primes[currentNumber];
			for (int i = 1; i < nProcesses; i++) {
				MPI_Recv(&isOthersPrime, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);

				isNextPrime = isNextPrime && isOthersPrime;
			}

			// Send Decision
			for (int i = 1; i < nProcesses; i++) {
				MPI_Send(&isNextPrime, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
			}
			
			// Do my part
			if(isNextPrime)
				calculatePrimesFor(n, rank, nProcesses, currentNumber, primes);
			
			currentNumber++;
		} while (currentNumber <= nLimit);
		
		cout << "Process " << rank << " finished Computation. Will now join data.\n";

		PRIMES_ARRAY otherPrimes(arrayLength);
		for (int i = 1; i < nProcesses; i++) {

			PRIMES_ARRAY::iterator iter = otherPrimes.begin();
			MPI_Recv(&iter, (arrayLength / 8), MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
			
			for (INDEX_VAR j = 0; j < arrayLength; j++) {
				
				if(i == 1)
				{
					cout << "Other primes [" << j << "]: " << otherPrimes[j] << endl;
				}
				
				primes[j] = primes[j] && otherPrimes[j];
			}
		}

		clock_gettime(CLOCK_REALTIME, &end);
		
		otherPrimes.clear();

		printArray(primes, arrayLength);
		printTimeSpent(start, end);
	}
	// Children processes
	else {
		do {
			// Send my value for the current number
			bool isNextPrime = primes[currentNumber];
			MPI_Send(&isNextPrime, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			
			// Receive decision
			MPI_Recv(&isNextPrime, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

			// Do my part
			if(isNextPrime)
				calculatePrimesFor(n, rank, nProcesses, currentNumber, primes);
			
			currentNumber++;
		} while (currentNumber <= nLimit);

		cout << "Process " << rank << " finished Computation. Will now join data.\n";

		PRIMES_ARRAY::iterator iter = primes.begin();
		MPI_Send(&iter, (arrayLength / 8), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	
	primes.clear();

	return 0;
}

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(argc < 2)
	{
		if(rank == 0)
			cout << "Required limit as argument" << endl;
		
		return -1;
	}

	INDEX_VAR limit = (INDEX_VAR)pow(2, atoi(argv[1]));

	distributedSieve(limit, rank, size);

	MPI_Finalize();

	return 0;
}
