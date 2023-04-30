#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// user-defined functions
int findShortestPathLength(int mat[20][20], int x1, int y1, int x2, int y2);
int findShortestPath(int mat[20][20], int visited[20][20], int i, int j, int x, int y, int min_dist, int dist);
int isSafe(int mat[20][20], int visited[20][20], int x, int y, int row, int col);

int main(void)
{
	int n;
	int x_1, x_2, y_1, y_2;
	// map of grocery store
	int mat[20][20] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1},
						{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1} };
	// array for storing distances
	int dis[1000];
	// array containing coordinates of items
	int coordinate[202][2];

	printf("no of items (coordinates): ");
	scanf_s("%d", &n);
	printf("\n");

	// coordinates of entry point
	coordinate[0][0] = 19;
	coordinate[0][1] = 10;
	// coordinates of items
	for (int k = 1; k <= n; k++)
	{
		printf("Enter coordinates of item %d:  ",k);
		scanf_s("%d , %d", &coordinate[k][0], &coordinate[k][1]);
		if (coordinate[k][0] == 0 && coordinate[k][1] == 0)
		{
			coordinate[k][0] = 1;
			coordinate[k][1] = 1;
		}
		else if (coordinate[k][0] == 0 && coordinate[k][1] == 19)
		{
			coordinate[k][0] = 1;
			coordinate[k][1] = 18;
		}
		else if (coordinate[k][0] == 0)
		{
			coordinate[k][0] = 1;
		}
		else if (coordinate[k][1] % 4 == 0)
		{
			coordinate[k][1]++;
		}
		else if (coordinate[k][1] % 4 == 3)
		{
			coordinate[k][1]--;
		}
		printf("Standing point w.r.t item is: %d,%d\n", coordinate[k][0], coordinate[k][1]);
	}
	printf("\n\n");
	// coordinates of checkout
	coordinate[n + 1][0] = 19;
	coordinate[n + 1][1] = 11;

	// variable for indicating index position of distance array
	int d = 0;

	// loop for calculating all distances
	for (int i = 0; i <= n+1; i++)
	{
		x_1 = coordinate[i][0];
		y_1 = coordinate[i][1];
		for (int j = i + 1; j <= n+1; j++)
		{
			x_2 = coordinate[j][0];
			y_2 = coordinate[j][1];
			// calculating distances
			dis[d] = findShortestPathLength(mat, x_1, y_1, x_2, y_2);
			// only for display
			if (i == 0 && j == (n + 1))
				printf("distance between entry and checkout is:\t%d\n", dis[d]);
			else if (i == 0)
				printf("distance between entry and item %d is:\t%d\n", j, dis[d]);
			else if (j == n + 1)
				printf("distance between item %d and checkout is: %d\n", i, dis[d]);
			else
				printf("distance between item %d and item %d is:\t%d\n", i, j, dis[d]);

			d++;
		}
		printf("\n");
	}

	// (only) to check whether or not distances are stored in distance array,
	printf("\nindex\tdistance\n");
	int sum = 0;
	int k = n + 1;
	while (k != 0)
	{
		sum += k;
		k--;
	}
	for (int l = 0; l < sum; l++)
	{
		printf("%d\t%d\n", l, dis[l]);
	}
	return 0;
}

// Check if it is possible to go to (x, y) from the current position. The
// function returns 0 if the cell has value 0 or already visited
int isSafe(int mat[20][20], int visited[20][20], int x, int y, int row, int col)
{
	return (x >= 0 && x < 20 && y >= 0 && y < 20) &&
		mat[x][y] == 0 && !visited[x][y];
}

int findShortestPath(int mat[20][20], int visited[20][20], int i, int j, int x, int y, int min_dist, int dist)
{

	if ((i == x) && (j == y))
	{
		min_dist = ((min_dist < dist) ? min_dist : dist);
		return min_dist;
	}
	// set (i, j) cell as visited
	visited[i][j] = 1;
	// go to the bottom cell
	if (isSafe(mat, visited, i + 1, j, x, y)) {

		min_dist = findShortestPath(mat, visited, i + 1, j, x, y, min_dist, dist + 1);
	}
	// go to the right cell
	if (isSafe(mat, visited, i, j + 1, x, y)) {

		min_dist = findShortestPath(mat, visited, i, j + 1, x, y, min_dist, dist + 1);
	}
	// go to the top cell
	if (isSafe(mat, visited, i - 1, j, x, y)) {

		min_dist = findShortestPath(mat, visited, i - 1, j, x, y, min_dist, dist + 1);
	}
	// go to the left cell
	if (isSafe(mat, visited, i, j - 1, x, y)) {

		min_dist = findShortestPath(mat, visited, i, j - 1, x, y, min_dist, dist + 1);
	}
	// backtrack: remove (i, j) from the visited matrix
	visited[i][j] = 0;
	return min_dist;
}

// Wrapper over findShortestPath() function
int findShortestPathLength(int mat[20][20], int x1, int y1, int x2, int y2) {
	if (mat[x1][y1] == 1 || mat[x2][y2] == 1)
		return -1;

	int visited[20][20];
	memset(visited, 0, sizeof(visited));

	int dis = INT_MAX;
	dis = findShortestPath(mat, visited, x1, y1, x2, y2, dis, 0);

	if (dis != INT_MAX)
		return dis;
	return -1;
}
