first_sieve:
	g++ -Wall -g -fopenmp -o2 sieve.cpp primes_utilities.cpp -o sieve

distributed:
	mpiCC.openmpi -Wall -g  -o2 -m64 distributed_sieve.cpp primes_utilities.cpp -o distributed_sieve
