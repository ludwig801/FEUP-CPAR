all:
	g++ -Wall -g -fopenmp sieve.cpp primes_utilities.cpp primes_calculation.cpp -o sieve

distributed:
	mpiCC.openmpi -Wall -g distributed_sieve.cpp primes_utilities.cpp primes_calculation.cpp -o distributed_sieve
