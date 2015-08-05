#ifndef _FILEFUNCTIONS_H_
#define _FILEFUNCTIONS_H_

#include <stdio.h>

// Constants
#define MAX_INT_ELEMENTS 50
#define MAX_INPUT_LINE_SIZE 1000

void displayFile();
int numberOfLinesInFile(char *fileName);
void initializeIntArray(int *intArray, int numberofElements);
void displayIntArray(int *intArray, int numberOfElements);
void display2DIntArray(int **intArray, int rows, int columns);
void fillIntArray(char *inputFileName, int inputLineNumber, int *inputArray, int numberOfElements);
int getNumberOfElementsInLine(char *inputFileName, int inputLineNumber);
void RemoveNewLineAndAddNullTerm(char *stringValue);
int countElementsInString(char *stringValue);
void getLineFromFile(char *inputFileName, int inputLineNumber, char *stringValue);
void outputResultToFile(int *resultArray, int resultArraySize, char *inputFileName);
void testfunction();
int getChangeAmount(char *inputFileName, int inputLineNumber);
int fileExists(char *fileName);
void pushIntResult(int value, int *arrayToPushTo, int arraySize);
void resetArrays(int *array, int arrLen);

#endif