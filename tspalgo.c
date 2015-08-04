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

void executeAlgorithm(int *inputArray, int numberOfElements, int changeAmount, char *inputFileName);

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

		int i;
		int numberOfElements = 0;
		int changeAmount = 0;
		int lineContainingArray = 0;
		int lineContainingChangeAmount = 0;
		char *inputFileName = argv[1];

		int numberOfLines = numberOfLinesInFile(inputFileName);
		int numberOfProblemsToProcess = numberOfLines / 2;

		// Run the algorithm problem in the input file. A problem
		//  consists of the array of denominations and the amount
		//  of change.
		for (i = 0; i < numberOfProblemsToProcess; i++)
		{
			lineContainingArray = i + i;
			numberOfElements = getNumberOfElementsInLine(inputFileName, lineContainingArray);

			int *inputArray = malloc(numberOfElements * sizeof(int));

			// Fill the input array with the numbers from line i in the file
			fillIntArray(inputFileName, lineContainingArray, inputArray, numberOfElements);

			lineContainingChangeAmount = (2 * i) + 1;
			changeAmount = getChangeAmount(inputFileName, lineContainingChangeAmount);

			executeAlgorithm(inputArray, numberOfElements, changeAmount, inputFileName);

			// Cleanup dynamically allocated strings
			free(inputArray);
		}
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
 * *  Executes the algorithm
 * *
 * ***************************************************************/
void executeAlgorithm(int *inputArray, int numberOfElements, int changeAmount, char *inputFileName)
{
	// Greedy algorithm pseudocode
	// For each element in the inputArray starting from the last element to the first
	// while the current element <= changeAmount
	// subtract the current element value from the changeAmount
	// increment the current denomination in the change denomination array
	// increment the minNumberOfCoins

	// Create and initialize the array to hold denomination amounts
	int i;
	int *resultChangeArray = malloc(numberOfElements * sizeof(int));
	for (i = 0; i < numberOfElements; i++)
	{
		resultChangeArray[i] = 0;
	}

	int minNumberOfCoins = 0;
	for (i = numberOfElements - 1; i >= 0; i--)
	{
		while (inputArray[i] <= changeAmount)
		{
			changeAmount -= inputArray[i];
			resultChangeArray[i]++;
			minNumberOfCoins++;

			if (changeAmount <= 0)
			{
				break;
			}
		}

		if (changeAmount <= 0)
		{
			break;
		}
	}

	// // Output the result to results file
	outputResultToFile(resultChangeArray, numberOfElements, minNumberOfCoins, inputFileName);

	free(resultChangeArray);
}
