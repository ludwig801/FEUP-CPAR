import java.util.Scanner;
import java.lang.System;

public class Test {
	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		int matrixDimension = 0, matrixMaxDimension = 0, matrixIncrement = 0;
		int selectedOption = 1;

		do {
			System.out.println("1. Multiplication");
			System.out.println("2. Line Multiplication");
			System.out.println("3. Multiplication (Parallel)");
			System.out.println("4. Line Multiplication (Parallel)");
			System.out.print("Selection?: ");
			selectedOption = scanner.nextInt();
			if (selectedOption < 1 || selectedOption > 4)
				break;
			System.out.print("Starting dimension?: ");
			matrixDimension = scanner.nextInt();
			System.out.print("Ending dimension?: ");
			matrixMaxDimension = scanner.nextInt();
			if (matrixDimension < matrixMaxDimension)
			{
				System.out.print("Increment?: ");
				matrixIncrement = scanner.nextInt();
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
						//LineLineMultiplication(matrixDimension);
						break;
					case 3:
						//LineColMultiplicationParallel(matrixDimension);
						break;
					case 4:
						//LineLineMultiplicationParallel(matrixDimension);
						break;
				}

			// PAPI: Stop and reset event counter here
			}
		} while (selectedOption >= 1 && selectedOption <= 4);
	}

	private static void LineColMultiplication(int dimension) {
		double[] matrixA = new double[dimension * dimension];
		double[] matrixB = new double[dimension * dimension];
		double[] matrixC = new double[dimension * dimension];

		for (int row = 0; row < dimension; row++)
			for (int col = 0; col < dimension; col++)
				matrixA[row * dimension + col] = 1.d;

		for (int row = 0; row < dimension; row++)
			for (int col = 0; col < dimension; col++)
				matrixB[row * dimension + col] = row + 1.d;

		long beginTime = System.currentTimeMillis();

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

		System.out.println("Dimensions: " + dimension + "x" + dimension);
		System.out.println("Time: " + (System.currentTimeMillis() - beginTime) / 1000.f + " seconds");

		System.out.print("Result matrix: ");
		for (int row = 0; row < 1; row++)
		{
			for (int col = 0; col < Math.min(10, dimension); col++)
				System.out.print(matrixC[col] + " ");
		}
		System.out.println("\n");
	}
}