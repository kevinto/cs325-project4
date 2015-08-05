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
	// display2DIntArray(cityArray, numberOfCities, 3);

	// Create and initialize the array to hold denomination amounts
	// int i;
	// int *resultChangeArray = malloc(numberOfElements * sizeof(int));
	// for (i = 0; i < numberOfElements; i++)
	// {
	// 	resultChangeArray[i] = 0;
	// }

	// int minNumberOfCoins = 0;
	// for (i = numberOfElements - 1; i >= 0; i--)
	// {
	// 	while (inputArray[i] <= changeAmount)
	// 	{
	// 		changeAmount -= inputArray[i];
	// 		resultChangeArray[i]++;
	// 		minNumberOfCoins++;

	// 		if (changeAmount <= 0)
	// 		{
	// 			break;
	// 		}
	// 	}

	// 	if (changeAmount <= 0)
	// 	{
	// 		break;
	// 	}
	// }

	// // // Output the result to results file
	// outputResultToFile(resultChangeArray, numberOfElements, minNumberOfCoins, inputFileName);

	// free(resultChangeArray);
}
