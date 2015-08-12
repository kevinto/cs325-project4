/**************************************************************
 * *  Filename: tspalgo.c
 * *  Coded by: Group 2
 * *  Purpose - This file implements the algorithm for the TSP
 * *			problem. We decided to implement an appoximation
 * *			algorithm.
 * *
 * ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "filefunctions.h"
#include <math.h>

struct edge {
	int i_id;
	int j_id;
	double weight;
};

// Function Definitions
void executeAlgorithm(int **cityArray, int numberOfCities, int numberOfElements, char *inputFileName);
void find_distance(struct city *cities, int num_of_cities, struct edge *city_edge, int num_of_edges);
double find_MST(int num_of_cities, struct edge *city_edge, int **mst_adj_matrix, int num_of_edges);
int cmp_weights(const void *a, const void *b);
void PrintAdjMatrix(int **mst_adj_matrix, int num_of_cities);
void setArrToNegOne(int *array, int arraySize);
void DFS(int sourceNode, int *visited, int **adjMatrix, int num_of_cities, int *dfsPath, int pathSize);
void addToPath(int *dfsPath, int pathSize, int sourceNode);
int calculatePathLength(int *dfsPath, int num_of_edges, int **mst_adj_matrix, int num_of_cities, struct edge *edgeList);
int getWeightFromEdgeList(struct edge *edgeList, int num_of_edges, int startingNode, int endingNode);

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

		executeAlgorithm(cityArray, numberOfLines, numberOfElements, inputFileName);

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
 * *  cityArray - The array containing all the cities.
 * *  numberOfCities - The number of the cities in the array above.
 * *  numberOfElements - DEPRECEATED. Needs to be removed in next refactoring
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
void executeAlgorithm(int **cityArray, int numberOfCities, int numberOfElements, char *inputFileName)
{
	int i;

	// Init dynamic array to hold the values from the test file. The format
	// of the dynamic array is an array of struct city. Each struct in the
	// array contains:
	// id = city identifier (this is an integer)
	// x = x coordinate (integer)
	// y = y coordinate (integer)
	struct city *cities = (struct city *)malloc(numberOfCities * sizeof(struct city));
	// Put test file values into the array
	for (i = 0; i < numberOfCities; i++)
	{
		cities[i].id = cityArray[i][0];
		cities[i].x = cityArray[i][1];
		cities[i].y = cityArray[i][2];
	}
	//debug purposes only
	// displayCityArray(cities, numberOfCities);

	// Create a dynamic array to hold the results, numberOfCities+1. The first line
	// will be the the path length and the following lines will be the path traversal
	// sequence
	int resultArraySize = numberOfCities + 1;
	int *resultArray = (int *)malloc(resultArraySize * sizeof(int));
	resetArrays(resultArray, resultArraySize);

	int num_of_cities = numberOfCities;
	int num_of_edges = num_of_cities * (num_of_cities) / 2;

	// Struct that stores 2 cities and the weight of their edge
	struct edge *city_edge;
	city_edge = malloc(sizeof(struct edge) * num_of_edges);

	// Init weighted adjacency matix
	int **mst_adj_matrix = malloc(sizeof(int *) * num_of_cities);
	for (i = 0; i < num_of_cities; i++) {
		mst_adj_matrix[i] = (int *)malloc(num_of_cities * sizeof(int));
	}

	//Find distance between each city & store in array of city structs 
	find_distance(cities, num_of_cities, city_edge, num_of_edges);

	// Find MST by using Prim's algorithm and store MST in adj matrix
	double total_weight = find_MST(num_of_cities, city_edge, mst_adj_matrix, num_of_edges);
	// printf("MST weight = %.0f\n", total_weight); // For debugging
	// PrintAdjMatrix(mst_adj_matrix, num_of_cities); // For debugging

	// Init the construct that keeps track of the DFS path
	int *dfsPath = (int *)malloc(num_of_edges * sizeof(int));
	setArrToNegOne(dfsPath, num_of_edges);

	// Init visited array for DFS to track the next node to traverse to.
	int *dfsVisited = (int *)malloc(num_of_cities * sizeof(int));
	for (int i = 0; i < num_of_cities; ++i)
	{
		dfsVisited[i] = 0;
	}

	// Run DFS, keeping track of the path and the total length
	DFS(0, dfsVisited, mst_adj_matrix, num_of_cities, dfsPath, num_of_edges);
	addToPath(dfsPath, num_of_edges, 0); // Add the last city to travel. This city is the first city in the path 

	// Write out the approximate optimal path length to the result array
	int finalTspPathLength = calculatePathLength(dfsPath, num_of_edges, mst_adj_matrix, num_of_cities, city_edge);
	pushIntResult(finalTspPathLength, resultArray, resultArraySize);
	// printf("final path length: %d\n", finalTspPathLength);

	// Put approximate "optimal" path into result array
	for (i = 0; i < num_of_cities; ++i)
	{
		// printf("visited: %d\n", dfsVisited[i]);
		pushIntResult(dfsPath[i], resultArray, resultArraySize);
	}
	// For debugging
	// displayIntArray(resultArray, resultArraySize);

	// Output the result array to the result file
	outputResultToFile(resultArray, resultArraySize, inputFileName);

	// Free all dynamic data structures
	for (i = 0; i < num_of_cities; i++) {
		free(mst_adj_matrix[i]);
	}
	free(mst_adj_matrix);
	free(dfsVisited);
	free(dfsPath);
	free(city_edge);
	free(resultArray);
}

// Calculate the length of the given path
int calculatePathLength(int *dfsPath, int num_of_edges,  int **mst_adj_matrix, int num_of_cities, struct edge *edgeList)
{
	int i, startingNode, endingNode;
	int numberOfNodesInPath = 0;
	int currentTotalEdgeWeight = 0;
	int currentEdgeWeight;

	// Find number of nodes in dfs path
	for (i = 0; i < num_of_edges; ++i)
	{
		if (dfsPath[i] != -1)
		{
			numberOfNodesInPath++;
		}
		else
		{
			break;
		}
	}

	// Find the sum of all the nodes in the path
	for (i = 0; i < numberOfNodesInPath; ++i)
	{
		if (i == numberOfNodesInPath - 2)
		{
			// End of the path, exit.
			// We are exiting at the second to list node because
			// we are adding the distance by edges.
			break;
		}
		else
		{
			// Get the start and end nodes of the edge
			startingNode = dfsPath[i];
			endingNode = dfsPath[i + 1];
		}

		if (mst_adj_matrix[startingNode][endingNode] == -2)
		{
			// Take care of the case where two cities have the same coordinates. 
			// Here two cities are on the same coordinates, so their weight will
			// be 0
			currentEdgeWeight = 0;
		}
		else
		{
			// Get the weight of the current edge
			currentEdgeWeight = mst_adj_matrix[startingNode][endingNode];
		}

		// Only find in edge list if the edge is not the MST and if the two nodes in 
		// question are not on the same coordinates
		if (currentEdgeWeight == 0 && !(mst_adj_matrix[startingNode][endingNode] == -2))
		{
			// Find weight in the edge list
			currentEdgeWeight = getWeightFromEdgeList(edgeList, num_of_edges, startingNode, endingNode);
		}
		currentTotalEdgeWeight += currentEdgeWeight;
	}

	// Find the edge for the second to last node in the path to the last node in the path
	currentEdgeWeight = getWeightFromEdgeList(edgeList, num_of_edges, dfsPath[numberOfNodesInPath - 2], dfsPath[numberOfNodesInPath - 1]);

	currentTotalEdgeWeight += currentEdgeWeight;

	// For debugging
	// for (i = 0; i < num_of_edges; i++) {
	// 	printf("Edge between %d and %d, W: %.0f\n", edgeList[i].i_id, edgeList[i].j_id, edgeList[i].weight);
	// }

	return currentTotalEdgeWeight;
}

// Gets the weight from the edge list containing a list of all the edges
int getWeightFromEdgeList(struct edge *edgeList, int num_of_edges, int startingNode, int endingNode)
{
	int i;
	for (i = 0; i < num_of_edges; i++)
	{
		if (edgeList[i].i_id == startingNode && edgeList[i].j_id == endingNode)
		{
			return round(edgeList[i].weight);
		}

		if (edgeList[i].j_id == startingNode && edgeList[i].i_id == endingNode)
		{
			return round(edgeList[i].weight);
		}
	}

	return 0;
}

// Perform a depth first search. Reasoning behind using DFS to solve TSP is given in the associated report
void DFS(int sourceNode, int *visited, int **adjMatrix, int num_of_cities, int *dfsPath, int pathSize)
{
	int j, i;
	visited[sourceNode] = 1;
	addToPath(dfsPath, pathSize, sourceNode);
	
	for (j = 0; j < num_of_cities; j++) {
		if (sourceNode >= num_of_cities || j >= num_of_cities)
		{
			// Protective code to prevent out of bounds access
			return;
		}

		// Here the -2 means that the two nodes are on the same coordinates
		if (!visited[j] && (adjMatrix[sourceNode][j] > 0 || adjMatrix[sourceNode][j] == -2)) {
			// Traverse to an adjacent node
			DFS(j, visited, adjMatrix, num_of_cities, dfsPath, pathSize);
		}
	}
}

// Adds a node to the an array tracking the traversal sequence
void addToPath(int *dfsPath, int pathSize, int sourceNode)
{
	int i;
	for (i = 0; i < pathSize; ++i)
	{
		if (dfsPath[i] == -1)
		{
			dfsPath[i] = sourceNode;
			break;
		}
	}
}

// Sets all elements in an array to -1
void setArrToNegOne(int *array, int arraySize) {
	int i;
	for (i = 0; i < arraySize; ++i)
	{
		array[i] = -1;
	}
}

// For debugging. Prints the adjacency matrix
void PrintAdjMatrix(int **mst_adj_matrix, int num_of_cities)
{
	int i, j;

	for (i = 0; i < num_of_cities; i++)
	{
		for (j = 0; j < num_of_cities; j++)
		{
			printf("%d ", mst_adj_matrix[i][j]);
		}
		printf("\n");
	}
}

//Find MST using Prim's Algorithm
double find_MST(int num_of_cities, struct edge *city_edge, int **mst_adj_matrix, int num_of_edges) {
	// NOTE: Before this method was called another method was invoked which populated city_edge. 
	// 		 city_edge was also sorted by weight in increasing order. This always needs be done
	//		 before this current method is invoked. This is to always maintain that the next edge
	//		 to be processed is the edge with the least cost.

	int *comps, i, j, k, cj, l;
	int total_distance = 0;

	//Init the array that tracks the number of cities to be processed
	comps = malloc(sizeof(int) * num_of_cities);
	for (i = 0; i < num_of_cities; i++) {
		comps[i] = i;
	}

	// Loops through all edges
	for (k = 0; k < num_of_edges; k++) {
		i = city_edge[k].i_id;
		j = city_edge[k].j_id;

		// If we havent processed the current edge yet
		if (comps[i] != comps[j]) {
			total_distance += city_edge[k].weight;

			// Add the edges to the adjacency matrix
			mst_adj_matrix[city_edge[k].i_id][city_edge[k].j_id] = round(city_edge[k].weight);
			mst_adj_matrix[city_edge[k].j_id][city_edge[k].i_id] = round(city_edge[k].weight);

			if (city_edge[k].weight == 0)
			{
				// Here -2 means the cities have the same coordinates
				mst_adj_matrix[city_edge[k].i_id][city_edge[k].j_id] = -2;
				mst_adj_matrix[city_edge[k].j_id][city_edge[k].i_id] = -2;
			}

			cj = comps[j];

			// Removes the already added edge from the comps array, so we do not
			// process it again.
			for (l = 0; l < num_of_cities; l++) {
				if (comps[l] == cj)
					comps[l] = comps[i];
			}
		}
	}

	// Print out the edges of the mst
	// for (i = 0; i < num_of_edges; i++) {
	//     printf("MST Edge between %d and %d, W: %.0f\n", mst_edge[i].i_id, mst_edge[i].j_id, mst_edge[i].weight);
	// }

	free(comps);
	// free(city_edge); // Freed in main
	return total_distance;
}

//qsort weight comparison function - required for qsort, which is part of the stdlib.h
int cmp_weights(const void *a, const void *b) {
	double w1, w2;

	w1 = ((struct edge*)a)->weight;
	w2 = ((struct edge*)b)->weight;

	if (w1 < w2)
		return -1;
	else if (w1 == w2)
		return 0;
	else
		return 1;
}

//Find distance between each city and store in struct
void find_distance(struct city *cities, int num_of_cities, struct edge *city_edge, int num_of_edges) {
//  struct edge city_edge[num_of_edges];
	int i, j, h = 0;
	double weight;
	for (i = 0; i < num_of_cities; i++) {
		for (j = i + 1; j < num_of_cities; j++) {
			weight = sqrt( pow((cities[i].x - cities[j].x), 2) + pow((cities[i].y - cities[j].y), 2) );
			city_edge[h].i_id = cities[i].id;
			city_edge[h].j_id = cities[j].id;
			city_edge[h].weight = weight;

			// TEST weights were stored correctly
			// printf("Edge between %d and %d, W: %.0f\n", city_edge[h].i_id, city_edge[h].j_id, city_edge[h].weight);

			h++;
		}
	}

	//qsort the edges by weight. Note that cmp_weights is a function
	qsort(city_edge, num_of_edges, sizeof(struct edge), cmp_weights);

	// TEST SORT WORKED
	// for (i = 0; i < num_of_edges; i++) {
	// 	printf("Edge between %d and %d, W: %.0f\n", city_edge[i].i_id, city_edge[i].j_id, city_edge[i].weight);
	// }
}