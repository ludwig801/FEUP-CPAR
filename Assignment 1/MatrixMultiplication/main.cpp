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

void OnMult(int dimension)
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
			for (int auxCol = 0; auxCol < dimension; auxCol++)
				tempSum += matrixA[row * dimension + auxCol] * matrixB[auxCol * dimension + col];

			matrixC[row * dimension + col] = tempSum;
		}
	}

	SYSTEMTIME endTime = clock();
	char executionTimeString[100];
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


void OnMultLine(int m_ar)
{


}

int main(int argc, char *argv[])
{
	int matrixDimension, matrixMaxDimension, matrixIncrement;
	int selectedOption;

	// PAPI: Cdimensionall init handlers here

	selectedOption = 1;
	do {
		cout << endl << "1. Multiplication" << endl;
		cout << "2. Line Multiplication" << endl;
		cout << "Selection?: ";
		cin >> selectedOption;
		if (selectedOption == 0)
			break;
		cout << "Starting dimension?: ";
		cin >> matrixDimension;
		cout << "Ending dimension?: ";
		cin >> matrixMaxDimension;
		cout << "Increment?: ";
		cin >> matrixIncrement;

		// PAPI: Start counting events here

		switch (selectedOption)
		{
		case 1:
			OnMult(matrixDimension);
			break;
		case 2:
			OnMultLine(matrixDimension);

			break;
		}

		// PAPI: Stop and reset event counter here
	} while (selectedOption != 0);

	// PAPI: Destroy event handlers here

	return 0;
}
