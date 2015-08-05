
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "filefunctions.h"

/**************************************************************
 * * Entry:
 * *  resultChangeArray - The array containing the amount of each denomination
 * *  numberOfElements - The result array size
 * *  minNumberOfCoins - The minimum number of coins
 * *  inputFileName - The input file name
 * *
 * * Exit:
 * *  n/a
 * *
 * * Purpose:
 * *  Outputs the results to [inputFileName]change.txt
 * *
 * ***************************************************************/
void outputResultToFile(int *resultChangeArray, int numberOfElements, int minNumberOfCoins, char *inputFileName)
{
	// char inputFileNameNoExt[MAX_INPUT_LINE_SIZE];
	// strncpy(inputFileNameNoExt, inputFileName, MAX_INPUT_LINE_SIZE - 1);

	// Remove the extension from the input file name
	// if (strstr(inputfilename, ".txt") != null) {
	// 	int inputfilenamelen = strlen(inputfilenamenoext);
	// 	inputfilenamenoext[inputfilenamelen - 4] = 0;
	// }

	// Create the output file name
	char outputFileName[MAX_INPUT_LINE_SIZE - 1];
	strncpy(outputFileName, inputFileName, MAX_INPUT_LINE_SIZE - 1);
	strncat(outputFileName, ".tour", MAX_INPUT_LINE_SIZE - 1);

	FILE *outputFile = fopen(outputFileName, "a");

	// Output the array containing the change amount of each denomination
	fprintf(outputFile, "%c", '[');
	int i;
	for (i = 0; i < numberOfElements; ++i)
	{
		// Do not write comma if first element
		if (i != 0)
		{
			fprintf(outputFile, "%s", ", ");
		}
		fprintf(outputFile, "%d", resultChangeArray[i]);
	}
	fprintf(outputFile, "%c", ']');
	fprintf(outputFile, "%c", '\n');

	// Output the minimum number of coins needed
	fprintf(outputFile, "%d", minNumberOfCoins);
	fprintf(outputFile, "%c", '\n');

	fclose(outputFile);
}

void fillIntArray(char *inputFileName, int inputLineNumber, int *inputArray, int numberOfElements)
{
	// printf("you are on line: %d\n", inputLineNumber);
	initializeIntArray(inputArray, numberOfElements);

	char stringValue[MAX_INPUT_LINE_SIZE];
	getLineFromFile(inputFileName, inputLineNumber, stringValue);
	// printf("the line is: %s\n", stringValue);

	// Establish string and get the first token
	int currentNumber;
	int currentInputArrayIdx = 0;
	char *token = strtok(stringValue, " ");
	while ( token != 0 )
	{
		sscanf(token, "%d", &currentNumber);

		if (currentInputArrayIdx >= numberOfElements)
		{
			// printf("inputLineNumber: %d\n", inputLineNumber);
			// printf("Error (fillIntArray): Accessing out of bounds index in array\n");
			break;
		}

		inputArray[currentInputArrayIdx++] = currentNumber;
		// printf("currentInputArrayIdx: %d\n", currentInputArrayIdx);

		/* Get next token: */
		token = strtok(0, " ");
	}
}

int getChangeAmount(char *inputFileName, int inputLineNumber)
{
	int changeAmount = 0;
	FILE *fp;
	fp = fopen(inputFileName, "r");

	if (fp == 0)
	{
		perror("Failed to open file");
	}
	else
	{
		int lineNumber = 0;
		char line[MAX_INPUT_LINE_SIZE];

		// Read the line
		while (fgets(line, sizeof(line), fp) != 0)
		{
			// Break the loop if we are at the line we want
			if (lineNumber == inputLineNumber)
			{
				break;
			}
			lineNumber++;
		}

		RemoveNewLineAndAddNullTerm(line);
		// printf("%s\n", line);

		sscanf(line, "%d", &changeAmount);
		fclose(fp);
	}

	return changeAmount;
}

int getNumberOfElementsInLine(char *inputFileName, int inputLineNumber)
{
	int numberOfElements = 0;
	FILE *fp;
	fp = fopen(inputFileName, "r");

	if (fp == 0)
	{
		perror("Failed to open file");
	}
	else
	{
		int lineNumber = 0;
		char line[MAX_INPUT_LINE_SIZE]; /* or other suitable maximum line size */

		while (fgets(line, sizeof(line), fp) != 0) /* read a line */
		{
			// fputs(line, stdout); /* write the line */
			// printf("%s\n", line);

			// Break the loop if we are at the line we want
			if (lineNumber == inputLineNumber)
			{
				break;
			}
			lineNumber++;
		}

		RemoveNewLineAndAddNullTerm(line);
		// printf("%s\n", line);

		numberOfElements = countElementsInString(line);
		fclose(fp);
	}

	return numberOfElements;
}

void getLineFromFile(char *inputFileName, int inputLineNumber, char *stringValue)
{
	int numberOfElements = 0;
	FILE *fp;
	fp = fopen(inputFileName, "r");

	if (fp == 0)
	{
		perror("Failed to open file");
	}
	else
	{
		int lineNumber = 0;
		char line[MAX_INPUT_LINE_SIZE]; /* or other suitable maximum line size */

		while (fgets(line, sizeof(line), fp) != 0) /* read a line */
		{
			// fputs(line, stdout); /* write the line */
			// printf("%s\n", line);

			// Break the loop if we are at the line we want
			if (lineNumber == inputLineNumber)
			{
				break;
			}
			lineNumber++;
		}

		RemoveNewLineAndAddNullTerm(line);
		strncpy(stringValue, line, MAX_INPUT_LINE_SIZE);
		// printf("%s\n", line);

		fclose(fp);
	}
}

int countElementsInString(char *stringValue)
{
	int numberOfSpaces = 0;
	int i;

	int stringLen = strlen(stringValue);
	for (i = 0; i < stringLen; i++)
	{
		if (stringValue[i] == 0)
		{
			break;
		}

		if (stringValue[i] == ' ')
		{
			numberOfSpaces++;
		}
	}

	if (numberOfSpaces == 0)
	{
		return 1;
	}
	else
	{
		return numberOfSpaces + 1;
	}
}

// Sets all elements in the array to zero
void initializeIntArray(int *intArray, int numberofElements)
{
	int i, j;
	for (i = 0; i < numberofElements; i++)
	{
		intArray[i] = 0;
	}
}

int numberOfLinesInFile(char *fileName)
{
	int ch = 0;
	int numberOfLines = 0;

	FILE *fp;
	fp = fopen(fileName, "r");

	if (fp == 0)
	{
		printf("Error: file not found\n");
	}

	// Count each newline character as a line
	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == '\n')
		{
			numberOfLines++;
		}
	}

	// This takes care of two cases:
	//	1. The last line of the file doesnt end with a new character
	//	2. There is only one line in the file and this lines does not
	//	   contain a newline character
	// if ((ch != '\n' && numberOfLines != 0) || (ch != 0 && numberOfLines == 0 ))
	// if ((ch != '\n' && numberOfLines != 0))
	// {
	// 	numberOfLines++;
	// }

	fclose(fp);
	return numberOfLines;
}

void displayFile()
{
	FILE *fp;
	char *fileName = "MSS_Problems.txt";
	fp = fopen(fileName, "r");

	if (fp == 0)
	{
		perror("Failed to open file: MSS_Problems.txt");
	}
	else
	{
		char line[5000]; /* or other suitable maximum line size */

		while (fgets(line, sizeof(line), fp) != 0) /* read a line */
		{
			fputs(line, stdout); /* write the line */
			printf("end line\n");
		}

		fclose(fp);
	}

	numberOfLinesInFile(fileName);
}

// For debug purposes
void displayIntArray(int *intArray, int numberOfElements)
{
	int i;
	int arrayTracker = 0;
	for (i = 0; i < numberOfElements; i++)
	{
		printf("%d ", intArray[i]);
		arrayTracker++;
	}

	printf("\n");
	printf("displayIntArray total elements: %d\n", arrayTracker);
}

/**************************************************************
 * * Entry:
 * *  stringValue - the string you want to transform
 * *
 * * Exit:
 * *  n/a
 * *
 * * Purpose:
 * *  Removes the new line character from the string and adds a null
 * *  terminator in its place.
 * *
 * ***************************************************************/
void RemoveNewLineAndAddNullTerm(char *stringValue)
{
	size_t ln = strlen(stringValue) - 1;
	if (stringValue[ln] == '\n')
	{
		stringValue[ln] = '\0';
	}
}

int fileExists(char *fileName)
{
	FILE *fp;
	fp = fopen(fileName, "r");

	if (fp == 0)
	{
		return 0;
	}
	fclose(fp);

	return 1;
}

void testfunction()
{
	printf("hello from ff\n");
}

void display2DIntArray(int **intArray, int rows, int columns)
{
	int i, j;
	for (i = 0; i <  rows; i++)
	{
		for (j = 0; j < columns; j++)
		{
			printf("%d ", intArray[i][j]);
		}
		printf("\n");
	}
}
