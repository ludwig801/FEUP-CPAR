#include <iostream>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

using namespace std;

#define SYSTEMTIME clock_t

int distributedSieve(int limit, int rank, int nProcesses) {
	
	MPI_Status status;

	int size = limit + 1;
	bool *primes = new bool[size];
	
	for(int i = 0; i < size; i++) {
		primes[i] = true;
	}

	if(rank == 0) {
		bool *iPrimes = new bool[size];
		int sqrtLimit = (int)sqrt(limit);
		
		for(int i = 0; i < size; i++) {
			iPrimes[i] = true;
		}
		
		int finish = sqrtLimit;

		for(int n = 2; n <= finish; n++) {
			int response = 1;
			
			if(n == finish) {
				response = 0;
				
				for(int i = 1; i < nProcesses; i++) {
					MPI_Send(&response, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				}
			}
			else {
				for(int i = 1; i < nProcesses; i++) {
					MPI_Send(&response, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				}
			}
				
			if(!primes[n])
			{
				cout << n << " was marked as being NON-prime" << endl;
				continue;
			}

			int max = limit / n;
			int nStep = max / nProcesses;
			int r = max % nProcesses;

			for(int i = 1; i < nProcesses; i++) {
				int offset = nStep * i + 1;
				int procMax = nStep * (i + 1);
				
				MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&offset, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&procMax, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
			
			for(int i = 2; i <= nStep; i++) {
				primes[i * n] = false;
			}
			
			if(r != 0) {
				for(int i = limit - r; i < limit; i++) {
					primes[i * n] = false;
				}		
			}

			for(int i = 1; i < nProcesses; i++) {
				MPI_Recv(&iPrimes[0], size, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
				
				for(int j = 2; j < size; j++) {
					primes[j] = primes[j] && iPrimes[j];
				}
			}
		}
		
		delete(iPrimes);
		
		// Final Output
		cout << "Output: ";
		int count = 0;
		for (int i = 2; i <= limit; i++)
		{
			if (primes[i])
			{
				count++;
				cout << i << " ";
			}
		}
		cout << endl << endl;
	}
	else {
		int response;
		
		do {
			int currentNumber;
			int offset, max;
			
			MPI_Recv(&response, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			
			MPI_Recv(&currentNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			MPI_Recv(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			MPI_Recv(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			
			offset = fmax(offset, 2);
			
			for(int i = offset; i <= max; i++) {
				primes[i * currentNumber] = false;
			}

			MPI_Send(&primes[0], size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

		} while(response != 0);
	}
	
	delete(primes);
	
    return 0; 
}

int main(int argc, char **argv) {

	MPI_Init(&argc, &argv);

	MPI_Status status;
	int rank, size;
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int limit;
	
	if(rank == 0) {
		cout << "Limit: ";
		cin >> limit;
		
		for(int i = 1; i < size; i++) {
			MPI_Send(&limit, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&limit, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}
	
	distributedSieve(limit, rank, size);

	MPI_Finalize();

	return 0;
}
