//cs325, Projec 4, Group 2 (Kevin To, Matthew Walz, Frank Eslami)
//Source code for main program

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//HARD-KEYED DATA UNTIL MATT'S FUNCTION PROVIDES DATA


//CREATE DUMMY DATA UNTIL MATT'S PROVIDES DATA
struct city {
	int id;
	int x;
	int y;
};

struct edge {
	int i_id;
	int j_id;
	double weight;
};

//FUNCTION DECLARATIONS
void find_distance(struct city cities[], int num_of_cities, struct edge *city_edge, int num_of_edges);
double find_MST(int num_of_cities, struct edge *city_edge, int **mst_adj_matrix, int num_of_edges);
int cmp_weights(const void *a, const void *b);
void PrintAdjMatrix(int **mst_adj_matrix, int num_of_cities);
void setArrToNegOne(int *array, int arraySize);
void DFS(int sourceNode, int *visited, int **adjMatrix, int num_of_cities, int *dfsPath, int pathSize);
void addToPath(int *dfsPath, int pathSize, int sourceNode);
int calculatePathLength(int *dfsPath, int num_of_edges, int **mst_adj_matrix, int num_of_cities, struct edge *edgeList);
int getWeightFromEdgeList(struct edge *edgeList, int num_of_edges, int startingNode, int endingNode);

int main() {
	int num_of_cities = 4;
	int num_of_edges = num_of_cities * (num_of_cities) / 2;

	//INITIALIZE DUMMY DATA UNTIL MATT'S PROVIDES DATA
	struct city cities[num_of_cities];
	cities[0].id = 0;
	cities[0].x = 200;
	cities[0].y = 800;

	cities[1].id = 1;
	cities[1].x = 3600;
	cities[1].y = 2300;

	cities[2].id = 2;
	cities[2].x = 3100;
	cities[2].y = 3300;

	cities[3].id = 3;
	cities[3].x = 4700;
	cities[3].y = 5750;


	//Struct that stores 2 cities and the weight of their edge
	struct edge *city_edge;
	city_edge = malloc(sizeof(struct edge) * num_of_edges);

	//Weighted adjacency Matix
	int i;
	int **mst_adj_matrix = malloc(sizeof(int *) * num_of_cities);
	for (i = 0; i < num_of_cities; i++) {
		mst_adj_matrix[i] = (int *)malloc(num_of_cities * sizeof(int));
	}

	//Find distance between each city & store in struct
	find_distance(cities, num_of_cities, city_edge, num_of_edges);

	//Find MST by using Prim's algorithm
	double total_weight = find_MST(num_of_cities, city_edge, mst_adj_matrix, num_of_edges);
	printf("MST weight = %.0f\n", total_weight);
	PrintAdjMatrix(mst_adj_matrix, num_of_cities); // For debugging

	//Init the construct that keeps track of the DFS path
	int *dfsPath = (int *)malloc(num_of_edges * sizeof(int));
	setArrToNegOne(dfsPath, num_of_edges);

	//Run DFS, keeping track of the path and the total length
	int *dfsVisited = (int *)malloc(num_of_cities * sizeof(int));
	DFS(0, dfsVisited, mst_adj_matrix, num_of_cities, dfsPath, num_of_edges);
	addToPath(dfsPath, num_of_edges, 0); // Add the last city to travel to
	for (i = 0; i < num_of_edges; i++) {
		printf("path: %d\n", dfsPath[i]);
	}

	// Write out the approximate optimal path length
	int finalTspPathLength = calculatePathLength(dfsPath, num_of_edges, mst_adj_matrix, num_of_cities, city_edge);
	printf("final path length: %d\n", finalTspPathLength);

	// Free all dynamic data structures
	for (i = 0; i < num_of_cities; i++) {
		free(mst_adj_matrix[i]);
	}
	free(mst_adj_matrix);
	free(dfsVisited);
	free(dfsPath);
	free(city_edge);

	return 0;
}

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

	for (i = 0; i < numberOfNodesInPath; ++i)
	{
		if (i == numberOfNodesInPath - 2)
		{
			// End of the path, exit
			break;
		}
		else
		{
			startingNode = dfsPath[i];
			endingNode = dfsPath[i + 1];
		}

		currentEdgeWeight = mst_adj_matrix[startingNode][endingNode];

		if (currentEdgeWeight == 0)
		{
			// Need to find it in the edge list
			currentEdgeWeight = getWeightFromEdgeList(edgeList, num_of_edges, startingNode, endingNode);
		}
		currentTotalEdgeWeight += currentEdgeWeight;

		printf("curr w: %d\n", currentEdgeWeight);
	}

	// Find the edge for the second to last node in the path to the last node in the path
	currentEdgeWeight = getWeightFromEdgeList(edgeList, num_of_edges, dfsPath[numberOfNodesInPath - 2], dfsPath[numberOfNodesInPath - 1]);
	printf("curr w: %d\n", currentEdgeWeight);

	currentTotalEdgeWeight += currentEdgeWeight;

	// for (i = 0; i < num_of_edges; i++) {
	// 	printf("Edge between %d and %d, W: %.0f\n", edgeList[i].i_id, edgeList[i].j_id, edgeList[i].weight);
	// }

	return currentTotalEdgeWeight;
}

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

void DFS(int sourceNode, int *visited, int **adjMatrix, int num_of_cities, int *dfsPath, int pathSize)
{
	int j;
	visited[sourceNode] = 1;
	addToPath(dfsPath, pathSize, sourceNode);
	// printf("path: %d\n", sourceNode);

	for (j = 0; j < num_of_cities; j++) {
		if (sourceNode >= num_of_cities || j >= num_of_cities)
		{
			// Protective code to prevent out of bounds access
			return;
		}

		if (!visited[j] && adjMatrix[sourceNode][j] > 0) {
			DFS(j, visited, adjMatrix, num_of_cities, dfsPath, pathSize);
		}
	}
}

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

void setArrToNegOne(int *array, int arraySize) {
	int i;
	for (i = 0; i < arraySize; ++i)
	{
		array[i] = -1;
	}
}

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
	int *comps, i, j, k, cj, l;
	int total_distance = 0;

	//Init the array that tracks the number of cities to be processed
	comps = malloc(sizeof(int) * num_of_cities);
	for (i = 0; i < num_of_cities; i++) {
		comps[i] = i;
	}

	for (k = 0; k < num_of_edges; k++) {
		i = city_edge[k].i_id;
		j = city_edge[k].j_id;

		if (comps[i] != comps[j]) {
			total_distance += city_edge[k].weight;

			// Add the edges to the adjacency matrix
			mst_adj_matrix[city_edge[k].i_id][city_edge[k].j_id] = round(city_edge[k].weight);
			mst_adj_matrix[city_edge[k].j_id][city_edge[k].i_id] = round(city_edge[k].weight);

			// printf("Added: Edge between %d and %d, W: %.0f\n", city_edge[k].i_id, city_edge[k].j_id, city_edge[k].weight);

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

//Find distance between each city and stored in struct
void find_distance(struct city cities[], int num_of_cities, struct edge *city_edge, int num_of_edges) {
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
	for (i = 0; i < num_of_edges; i++) {
		printf("Edge between %d and %d, W: %.0f\n", city_edge[i].i_id, city_edge[i].j_id, city_edge[i].weight);
	}
}
