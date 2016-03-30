#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <algorithm>
//#include <papi.h> // <-- Uncomment to use PAPI

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
	sprintf_s(executionTimeString, "Time: %3.3f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
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
	sprintf_s(executionTimeString, "Time: %3.3f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
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

		SYSTEMTIME beginTime = clock();
		int row = 0, col = 0, auxRow = 0;
		double tempSum = 0.0;

		omp_set_num_threads(threads);

#pragma omp parallel for //private(col) private(tempSum)private(auxRow) 
		for (row = 0; row < dimension; row++)
		{
#pragma omp parallel for private(tempSum) private(auxRow)
			for (col = 0; col < dimension; col++)
			{
				tempSum = 0;
				for (auxRow = 0; auxRow < dimension; auxRow++)
					tempSum += matrixA[row * dimension + auxRow] * matrixB[auxRow * dimension + col];

				matrixC[row * dimension + col] = tempSum;
			}
		}

		SYSTEMTIME endTime = clock();
		char executionTimeString[100];
		cout << endl << "Threads: " << threads << endl;
		cout << "Dimensions: " << dimension << "x" << dimension << endl;
		sprintf_s(executionTimeString, "Time: %3.3f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
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
	int i = 0, j = 0;
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


		SYSTEMTIME beginTime = clock();
		double tempSum = 0.0;

		omp_set_num_threads(threads);

#pragma omp parallel for private(j)
		for (i = 0; i < dimension; i++)
		{
			for (j = 0; j < dimension; j++)
			{
#pragma omp parallel for
				for (int k = 0; k < dimension; k++)
				{
					matrixC[i * dimension + k] += matrixA[i * dimension + j] * matrixB[j * dimension + k];
				}
			}
		}

		SYSTEMTIME endTime = clock();
		char executionTimeString[100];
		cout << endl << "Threads: " << threads << endl;
		cout << "Dimensions: " << dimension << "x" << dimension << endl;
		sprintf_s(executionTimeString, "Time: %3.3f seconds\n", (double)(endTime - beginTime) / CLOCKS_PER_SEC);
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

	// PAPI: Cdimensionall init handlers here

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
			// PAPI: Start counting events here

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

			// PAPI: Stop and reset event counter here
		}
	} while (selectedOption >= 1 && selectedOption <= 4);

	// PAPI: Destroy event handlers here

	return 0;
}
