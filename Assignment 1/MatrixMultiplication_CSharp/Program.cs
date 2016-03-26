using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MatrixMultiplication_CSharp
{
    class Program
    {
        static void LineColMultiplication(int dimension)
        {
            double[] matrixA, matrixB, matrixC;

            matrixA = new double[dimension * dimension];
            matrixB = new double[dimension * dimension];
            matrixC = new double[dimension * dimension];

            for (int row = 0; row < dimension; row++)
                for (int col = 0; col < dimension; col++)
                {
                    matrixA[row * dimension + col] = 1.0;
                    matrixB[row * dimension + col] = (double)(row + 1);
                    matrixC[row * dimension + col] = 0.0;
                }

            var beginTime = DateTime.Now;
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

            var endTime = DateTime.Now;
            Console.WriteLine(string.Concat("Dimensions: ", dimension, "x", dimension));
            Console.WriteLine("Time: {0:N3}", (endTime - beginTime).TotalMilliseconds);
            Console.WriteLine("Result matrix: ");
            for (int row = 0; row < 1; row++)
                for (int col = 0; col < Math.Min(10, dimension); col++)
                    Console.Write(string.Concat(matrixC[col], " "));
            Console.WriteLine();
        }

        static void LineLineMultiplication(int dimension)
        {
            double[] matrixA, matrixB, matrixC;

            matrixA = new double[dimension * dimension];
            matrixB = new double[dimension * dimension];
            matrixC = new double[dimension * dimension];

            for (int row = 0; row < dimension; row++)
                for (int col = 0; col < dimension; col++)
                {
                    matrixA[row * dimension + col] = (double)1.0;
                    matrixB[row * dimension + col] = (double)(row + 1);
                    matrixC[row * dimension + col] = (double)0;
                }

            var beginTime = DateTime.Now;

            for (int row = 0; row < dimension; row++)
            {
                for (int col = 0; col < dimension; col++)
                {
                    for (int auxCol = 0; auxCol < dimension; auxCol++)
                    {
                        matrixC[row * dimension + auxCol] += matrixA[row * dimension + col] * matrixB[col * dimension + auxCol];
                    }
                }
            }

            var endTime = DateTime.Now;
            Console.WriteLine(string.Concat("Dimensions: ", dimension, "x", dimension));
            Console.WriteLine("Time: {0:N3}", (endTime - beginTime).TotalMilliseconds);
            Console.WriteLine("Result matrix: ");
            for (int row = 0; row < 1; row++)
                for (int col = 0; col < Math.Min(10, dimension); col++)
                    Console.Write(string.Concat(matrixC[col], " "));
            Console.WriteLine();
        }

        static void main(string[] args)
        {
            int matrixDimension, matrixMaxDimension, matrixIncrement;
            int selectedOption;

            // PAPI: Cdimensionall init handlers here

            selectedOption = 1;
            do
            {
                Console.WriteLine();
                Console.WriteLine("1. Multiplication");
                Console.WriteLine("2. Line Multiplication");
                Console.WriteLine("3. Multiplication (Parallel)");
                Console.WriteLine("4. Line Multiplication (Parallel)");
                Console.Write("Selection ?: ");
                selectedOption = int.Parse(Console.ReadLine());
                if (selectedOption < 1 || selectedOption > 4)
                    break;
                Console.Write("Starting dimension ?: ");
                matrixDimension = int.Parse(Console.ReadLine());
                Console.Write("Ending dimension ?: ");
                matrixMaxDimension = int.Parse(Console.ReadLine());
                Console.Write("Increment ?: ");
                matrixIncrement = int.Parse(Console.ReadLine());

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
                    }

                    // PAPI: Stop and reset event counter here
                }
            } while (selectedOption >= 1 && selectedOption <= 4);

            // PAPI: Destroy event handlers here
        }
    }
}
