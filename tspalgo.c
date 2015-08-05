/**************************************************************
 * *  Filename: tspalgo.c
 * *  Coded by: Group 2
 * *  Purpose - This file implements the algorithm for the TSP
 * *			problem.
 * *
 * ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "filefunctions.h"

void executeAlgorithm(int **cityArray, int numberOfCities, char *inputFileName);

// Program entry point
int main(int argc, char *argv[])
{
	if ( argc != 2 )
	{
		// A file name was not entered as a program argument
		printf( "usage: %s filename\n", argv[0] );
		return 1;
	}
	else
	{
		// Check if file exists
		if (fileExists(argv[1]) == 0)
		{
			printf("Error: File does not exist\n");
			return 1;
		}

		int i, j;
		int changeAmount = 0;
		int lineContainingArray = 0;
		int lineContainingChangeAmount = 0;
		char *inputFileName = argv[1];
		int numberOfLines = numberOfLinesInFile(inputFileName);
		int numberOfElements = getNumberOfElementsInLine(inputFileName, 0);

		// Init dynamic array to hold the values from the test file. The format
		// of the dynamic array is cityArray[row][column]. A row contains coordinates
		// for one city. There are three columns:
		// first column = city identifier (this is an integer)
		// second column = x coordinate (integer)
		// third column = y coordinate (integer)
		int **cityArray = (int **)malloc(numberOfLines * sizeof(int *));
		for (i = 0; i < numberOfLines; i++)
		{
			cityArray[i] = (int *)malloc(numberOfElements * sizeof(int));
		}

		// Put test file values into the array
		for (i = 0; i <  numberOfLines; i++)
		{
			fillIntArray(inputFileName, i, cityArray[i], numberOfElements);
		}

		// For debug only
		// display2DIntArray(cityArray, numberOfLines, numberOfElements);

		executeAlgorithm(cityArray, numberOfLines, inputFileName);

		// Free dynamic arrays
		for (i = 0; i < numberOfLines; i++)
		{
			free(cityArray[i]);
		}
		free(cityArray);
	}

	return 0;
}

/**************************************************************
 * * Entry:
 * *  inputArray - The input array containing the coin denominations.
 * *  numberOfElements - The number of elements in the array.
 * *  changeAmount - The change amount.
 * *  inputFileName - The input file name.
 * *
 * * Exit:
 * *  n/a
 * *
 * * Purpose:
 * *  Executes the algorithm. This method assumes each row in cityArray
 * *  has three columns.
 * *
 * ***************************************************************/
void executeAlgorithm(int **cityArray, int numberOfCities, char *inputFileName)
{
	// Create a dynamic array to hold the results, numberOfCities+1
	int resultArraySize = numberOfCities + 1;
	int *resultArray = (int *)malloc(resultArraySize * sizeof(int));
	resetArrays(resultArray, resultArraySize);

	// TODO: Write algorithm using the push mechanism to keep track of the cities. 
	// Note: The optimal TSP path should be the first line of the output file, 
	//       so we might need to create an intermediate dynamic array to hold the 
	//    	 optimum path.
	int testint1 = 1;
	pushIntResult(testint1, resultArray, resultArraySize);

	// For debugging
	// displayIntArray(resultArray, resultArraySize);

	// Create a method to output the resulting array to a file
	outputResultToFile(resultArray, resultArraySize, inputFileName);

	free(resultArray);
}
