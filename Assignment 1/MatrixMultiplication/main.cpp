#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#ifdef __gnu_linux__
	#include <papi.h>
#endif

using namespace std;

#define SYSTEMTIME clock_t

void LineColMultiplication(int dimension)
{
	double *matrixA, *matrixB, *matrixC;

	matrixA = (double *)malloc((dimension * dimension) * sizeof(double));
	matrixB = (double *)malloc((dimension * dimension) * sizeof(double));
	matrixC = (double *)malloc((dimension * dimension) * sizeof(double));

	for (int row = 0; row < dimension; row++)
		for (int col = 0; col < dimension; col++)
			matrixA[row * dimension + col] = (double)1.0;

	for (int row = 0; row < dimension; row++)
		for (int col = 0; col < dimension; col++)
			matrixB[row * dimension + col] = (double)(row + 1);

	SYSTEMTIME beginTime = clock();
	double tempSum;

	for (int row = 0; row < dimension; row++)
	{
		for (int col = 0; col < dimension; col++)
		{
			tempSum = 0;
			for (int auxRow = 0; auxRow < dimension; auxRow++)
				tempSum += matrixA[row * dimension + auxRow] * matrixB[auxRow * dimension + col];

			matrixC[row * dimension + col] = tempSum;
		}
	}

	SYSTEMTIME endTime = clock();
	char executionTimeString[100];
	cout << endl << "Dimensions: " << dimension << "x" << dimension << endl;
	sprintf(executionTimeString, "Time: %3.3f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
	cout << executionTimeString;

	cout << "Result matrix: " << endl;
	for (int row = 0; row < 1; row++)
	{
		for (int col = 0; col < min(10, dimension); col++)
			cout << matrixC[col] << " ";
	}
	cout << endl;

	// Free allocated memory for the matrices
	free(matrixA);
	free(matrixB);
	free(matrixC);
}

void LineLineMultiplication(int dimension)
{
	double *matrixA, *matrixB, *matrixC;

	matrixA = (double *)malloc((dimension * dimension) * sizeof(double));
	matrixB = (double *)malloc((dimension * dimension) * sizeof(double));
	matrixC = (double *)malloc((dimension * dimension) * sizeof(double));

	for (int row = 0; row < dimension; row++)
		for (int col = 0; col < dimension; col++)
		{
			matrixA[row * dimension + col] = (double)1.0;
			matrixB[row * dimension + col] = (double)(row + 1);
			matrixC[row * dimension + col] = (double)0;
		}


	SYSTEMTIME beginTime = clock();
	double tempSum = 0.0;

	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			for (int k = 0; k < dimension; k++)
			{
				matrixC[i * dimension + k] += matrixA[i * dimension + j] * matrixB[j * dimension + k];
			}
		}
	}

	SYSTEMTIME endTime = clock();
	char executionTimeString[100];
	cout << endl << "Dimensions: " << dimension << "x" << dimension << endl;
	sprintf(executionTimeString, "Time: %3.3f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
	cout << executionTimeString;

	cout << "Result matrix: " << endl;
	for (int row = 0; row < 1; row++)
	{
		for (int col = 0; col < min(10, dimension); col++)
			cout << matrixC[col] << " ";
	}
	cout << endl;

	// Free allocated memory for the matrices
	free(matrixA);
	free(matrixB);
	free(matrixC);
}

void LineColMultiplicationParallel(int dimension)
{
	double *matrixA, *matrixB, *matrixC;

	matrixA = (double *)malloc((dimension * dimension) * sizeof(double));
	matrixB = (double *)malloc((dimension * dimension) * sizeof(double));
	matrixC = (double *)malloc((dimension * dimension) * sizeof(double));

	for (int threads = 1; threads <= 4; threads++)
	{
		for (int row = 0; row < dimension; row++)
			for (int col = 0; col < dimension; col++)
			{
				matrixA[row * dimension + col] = (double)1.0;
				matrixB[row * dimension + col] = (double)(row + 1);
				matrixC[row * dimension + col] = (double)0.0;
			}

		double beginTime = omp_get_wtime();
		int row = 0, col = 0, auxRow = 0;
		double tempSum = 0.0;

		omp_set_num_threads(threads);

#pragma omp parallel for private(col) private(tempSum) private(auxRow)
		for (row = 0; row < dimension; row++)
		{
			for (col = 0; col < dimension; col++)
			{
				tempSum = 0;
				for (auxRow = 0; auxRow < dimension; auxRow++)
					tempSum += matrixA[row * dimension + auxRow] * matrixB[auxRow * dimension + col];

				matrixC[row * dimension + col] = tempSum;
			}
		}

		double endTime = omp_get_wtime();
		char executionTimeString[100];
		cout << endl << "Threads: " << threads << endl;
		cout << "Dimensions: " << dimension << "x" << dimension << endl;
		sprintf(executionTimeString, "Time: %3.3f seconds\n", (double)(endTime - beginTime));
		cout << executionTimeString;

		cout << "Result matrix: " << endl;
		for (int row = 0; row < 1; row++)
		{
			for (int col = 0; col < min(10, dimension); col++)
				cout << matrixC[col] << " ";
		}
		cout << endl;
	}

	// Free allocated memory for the matrices
	free(matrixA);
	free(matrixB);
	free(matrixC);
}

void LineLineMultiplicationParallel(int dimension)
{
	int i = 0, j = 0, k = 0;
	double *matrixA, *matrixB, *matrixC;

	matrixA = (double *)malloc((dimension * dimension) * sizeof(double));
	matrixB = (double *)malloc((dimension * dimension) * sizeof(double));
	matrixC = (double *)malloc((dimension * dimension) * sizeof(double));

	for (int threads = 1; threads <= 4; threads++)
	{
		for (i = 0; i < dimension; i++)
			for (int col = 0; col < dimension; col++)
			{
				matrixA[i * dimension + col] = (double)1.0;
				matrixB[i * dimension + col] = (double)(i + 1);
				matrixC[i * dimension + col] = (double)0;
			}


		double beginTime = omp_get_wtime();
		double tempSum = 0.0;

		omp_set_num_threads(threads);

#pragma omp parallel for private(j) private(k)
		for (i = 0; i < dimension; i++)
		{
			for (j = 0; j < dimension; j++)
			{
				for (k = 0; k < dimension; k++)
				{
					matrixC[i * dimension + k] += matrixA[i * dimension + j] * matrixB[j * dimension + k];
				}
			}
		}

		double endTime = omp_get_wtime();
		char executionTimeString[100];
		cout << endl << "Threads: " << threads << endl;
		cout << "Dimensions: " << dimension << "x" << dimension << endl;
		sprintf(executionTimeString, "Time: %3.3f seconds\n", (double)(endTime - beginTime));
		cout << executionTimeString;

		cout << "Result matrix: " << endl;
		for (int row = 0; row < 1; row++)
		{
			for (int col = 0; col < min(10, dimension); col++)
				cout << matrixC[col] << " ";
		}
		cout << endl;
	}

	// Free allocated memory for the matrices
	free(matrixA);
	free(matrixB);
	free(matrixC);
}

int main(int argc, char *argv[])
{
	int matrixDimension = 0, matrixMaxDimension = 0, matrixIncrement = 0;
	int selectedOption;

#ifdef __gnu_linux__
	int EventSet = PAPI_NULL;
	long long papiValues[2];
	int ret;

	ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		std::cout << "FAILURE initializing PAPI library" << endl;

	ret = PAPI_create_eventset(&EventSet);
		if (ret != PAPI_OK) cout << "ERROR: create eventset" << endl;


	ret = PAPI_add_event(EventSet,PAPI_L1_DCM );
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L1_DCM" << endl;


	ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L2_DCM" << endl;
#endif

	selectedOption = 1;
	do {
		cout << endl << "1. Multiplication" << endl;
		cout << "2. Line Multiplication" << endl;
		cout << "3. Multiplication (Parallel)" << endl;
		cout << "4. Line Multiplication (Parallel)" << endl;
		cout << "Selection?: ";
		cin >> selectedOption;
		if (selectedOption < 1 || selectedOption > 4)
			break;
		cout << "Starting dimension?: ";
		cin >> matrixDimension;
		cout << "Ending dimension?: ";
		cin >> matrixMaxDimension;
		if (matrixDimension < matrixMaxDimension)
		{
			cout << "Increment?: ";
			cin >> matrixIncrement;
		}

		if (matrixIncrement <= 0)
			matrixIncrement = 1000;

		for (; matrixDimension <= matrixMaxDimension; matrixDimension += matrixIncrement)
		{
#ifdef __gnu_linux__
			ret = PAPI_start(EventSet);
			if (ret != PAPI_OK) cout << "ERROR: Start PAPI" << endl;
#endif

			switch (selectedOption)
			{
			case 1:
				LineColMultiplication(matrixDimension);
				break;
			case 2:
				LineLineMultiplication(matrixDimension);
				break;
			case 3:
				LineColMultiplicationParallel(matrixDimension);
				break;
			case 4:
				LineLineMultiplicationParallel(matrixDimension);
				break;
			}

#ifdef __gnu_linux__
			ret = PAPI_stop(EventSet, papiValues);
  			if (ret != PAPI_OK) cout << "ERROR: Stop PAPI" << endl;
  			printf("L1 DCM: %lld \n",papiValues[0]);
  			printf("L2 DCM: %lld \n",papiValues[1]);
#endif
		}
	} while (selectedOption >= 1 && selectedOption <= 4);

#ifdef __gnu_linux__
	ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
	if ( ret != PAPI_OK )
		std::cout << "FAILURE removing event" << endl; 

	ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
	if ( ret != PAPI_OK )
		std::cout << "FAILURE removing event" << endl; 

	ret = PAPI_destroy_eventset( &EventSet );
	if ( ret != PAPI_OK )
		std::cout << "FAILURE destroying event set" << endl;
#endif

	return 0;
}
