all:
	g++ -Wall -fopenmp sieve.cpp primes_utilities.cpp primes_calculation.cpp -o sieve

distributed:
	mpiCC.openmpi distributed_sieve.cpp primes_utilities.cpp primes_calculation.cpp -o distributed_sieve
