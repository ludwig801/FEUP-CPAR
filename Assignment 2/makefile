all:
	g++ -Wall -g -fopenmp sieve.cpp primes_utilities.cpp primes_calculation.cpp -o sieve

distributed:
	mpiCC.openmpi -o2 -m64 distributed_sieve.cpp primes_utilities.cpp -o distributed_sieve
