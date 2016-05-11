#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"

using namespace std;

#define SYSTEMTIME clock_t

#define MY_TYPE unsigned long long

void printBoolArray(bool *array, int length) {
	cout << "Array: ";
	int count = 0;
	for (int i = length - 1; count < 10 && i >= 2; i--) {
		if(array[i]) {
			cout << i << " ";
			count++;
		}
	}
	cout << endl;
}

int calculatePrimesFor(int limit, int rank, int nProcesses, int currentNumber, bool *primes) {

	int arrayLength = limit + 1;
	float calculations = (limit / currentNumber) - 1;
	int step = (int) ceil(calculations / nProcesses);
	
	int min = 2 + rank * step;
	int max = min + step;

	for (int i = min; i < max; i++) {
		int index = i * currentNumber;
		
		if(index < arrayLength) {
			primes[index] = false;
		}
	}
	
	return 0;
}

int distributedSieve(int limit, int rank, int nProcesses) {
	MPI_Status status;

	int currentNumber = 2;
	int nLimit = (int)sqrt(limit);
	int size = limit + 1;
	bool isNextPrime = true;
	bool *primes = new bool[size];

	for (int i = 0; i < size; i++) {
		primes[i] = true;
	}

	// Parent process
	if (rank == 0) {
		bool isOthersPrime = false;
		int notPrime = -1;
		
		//cout << "nLimit -----> " << nLimit << endl;

		do {
			
			isNextPrime = primes[currentNumber];
			// Receive if the current number is prime for all or not
			for (int i = 1; i < nProcesses; i++) {
				MPI_Recv(&isOthersPrime, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);

				isNextPrime = isNextPrime && isOthersPrime;
			}
			
			//cout << "Decision: " << currentNumber << " --> " << (isNextPrime == true ? "true" : "false") << endl;
			
			// Send decision
			for (int i = 1; i < nProcesses; i++) {
				MPI_Send(&isNextPrime, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
			}
			
			if(isNextPrime)
			{
				calculatePrimesFor(limit, rank, nProcesses, currentNumber, primes);
			}
			
			currentNumber++;
			
		} while (currentNumber <= nLimit);
		
		//cout << "Receiving all primes..." << endl;
		
		// Receive full range of "others" primes
		bool *otherPrimes = new bool[size];
		for (int i = 1; i < nProcesses; i++) {
			
			MPI_Recv(&otherPrimes[0], size, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
			
			//cout << "Received all primes from " << i << endl;

			for (int j = 0; j < size; j++) {
				primes[j] = primes[j] && otherPrimes[j];
			}
		}
		
		//cout << "All primes received." << endl;

		// Print final results
		printBoolArray(primes, size);
		
		delete(otherPrimes);
	}
	// Children processes
	else {
		do {
			
			//cout << "[" << rank << "] will send " << currentNumber << " --> " << (primes[currentNumber] == true ? "true" : "false") << endl;
			
			// Send "my" value of currentNumber
			MPI_Send(&primes[currentNumber], 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			
			// Receive parent decision
			MPI_Recv(&isNextPrime, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			
			//cout << "[" << rank << "] received decision for " << currentNumber << " --> " << (isNextPrime == true ? "true" : "false") << endl;

			if(isNextPrime)
			{
				calculatePrimesFor(limit, rank, nProcesses, currentNumber, primes);
			}
			
			currentNumber++;
			
		} while (currentNumber <= nLimit);
		
		//cout << "Process " << rank << " will send all primes..." << endl;

		// Here we synchronize all primes calculated
		MPI_Send(&primes[0], size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		
		//cout << "Process "<< rank << " sent primes..." << endl;
	}
	
	//cout << "Process " << rank << " terminated." << endl;

	delete(primes);

	return 0;
}

int main(int argc, char **argv) {

	MPI_Init(&argc, &argv);

	MPI_Status status;
	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(argc < 2)
	{
		if(rank == 0)
		{
			cout << "Required limit as argument" << endl;
		}
		
		return -1;
	}

	int limit = pow(2, atoi(argv[1]));
	
	distributedSieve(limit, rank, size);

	MPI_Finalize();

	return 0;
}
