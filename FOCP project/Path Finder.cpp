// GROCERY STORE PATH FINDER 

#define _CRT_SECURE_NO_WARNINGS

// libraries 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>

//define dimensions for 2d array for map
#define COLUMN 20
#define ROW 20
//dimensions for 2d array for items inventory
#define MAX_LINE 192
#define MAX_CHARS 200
#define MAX_PATH 100

// user-defined functions in program
int findShortestPathLength(int mat[20][20], int x1, int y1, int x2, int y2);
int findShortestPath(int mat[20][20], char visited[20][20], int i, int j, int x, int y, int min_dist, int dist);
int isSafe(int mat[20][20], char visited[20][20], int x, int y, int row, int col);
int displayShortestPathLength(int mat[20][20], int **t_mat, int **t_mat2, int x1, int y1, int x2, int y2, int pass);
int displayShortestPath(int mat_in[20][20], char mat2[20][20], char t_map[20][20], char visited[20][20], int i, int j, int x, int y, int min_dist, int dist, int pass);

// main body
int main(void)
{
	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hconsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("\n=================================================================================================================================================\n");
	printf("\n\t\t\t\t%5c%5c%5c       WELCOME       %c%5c%5c\t\t\t\t\n", 4, 4, 4, 4, 4, 4);
	printf("\n=================================================================================================================================================\n\n");

	//arrays for x and y coordinates of item's position and extra to ask user for number of list items
	int x[192], y[192], item_no;

	//2d array to store map
	int location[20][20];

	//2d array to store items
	char items[192][200];

	//1d array to store index positions of desired items
	int index_positions[192];

	//2d array to input list
	char list[192][200];

	//coordinates of desired items in shopping list, temporary array
	int d_coordinates[192][2];
	int start[1][2];
	int stop[1][2];

	//file pointer for map
	FILE* map_pointer;


	//file pointer for items
	FILE* item_pointer;


	//open map
	map_pointer = fopen("map.txt", "r");

	//open file of items
	item_pointer = fopen("storeitem.txt", "r");


	//to check if there is any error in opening file
	if (item_pointer == NULL)
	{
		printf("Error in openning file\n");
	}
	if (map_pointer == NULL)
	{
		printf("Error in openning file\n");
	}

	//declare temporary variable for row
	int temp_row = 0;

	//read map from file into 2d array
	while (!feof(map_pointer))
	{
		if (ferror(map_pointer))
		{
			printf("Error reading file\n");
		}
		for (int j = 0; j < COLUMN; j++)
		{
			if (fscanf(map_pointer, "%d", &location[temp_row][j]) == EOF)
			{
				break;
			}
		}
		temp_row++;
		if (temp_row == ROW)
		{
			break;
		}
	}
	//close the map file
	fclose(map_pointer);


	//variable to define the index of arrays containig coordinates
	int c = 0;

	//storing location of items in coordinates
	for (int a = 0; a < ROW; a++)
	{
		for (int b = 0; b < COLUMN; b++)
		{
			if (location[a][b] == 1)
			{
				x[c] = a;
				y[c] = b;
				c++;
			}
		}
	}


	//reading store items from items file
	int line = 0;
	while (!feof(item_pointer) && !ferror(item_pointer))
	{
		if (fgets(items[line], MAX_CHARS, item_pointer) != NULL)
		{
			if ((strlen(items[line]) > 0) && (items[line][strlen(items[line]) - 1] == '\n'))
				items[line][strlen(items[line]) - 1] = '\0';
			line++;
		}
	}

	//closing items file
	fclose(item_pointer);

	SetConsoleTextAttribute(hconsole, FOREGROUND_RED);
	printf("How many number of items you want to buy (specify their names): ");
	SetConsoleTextAttribute(hconsole, FOREGROUND_GREEN);
	scanf_s("%d", &item_no);
	getchar();

	// taking shopping list from the user
	for (int d = 0; d < item_no; d++)
	{
		fgets(list[d], 200, stdin);
		if ((strlen(list[d]) > 0) && (list[d][strlen(list[d]) - 1] == '\n'))
			list[d][strlen(list[d]) - 1] = '\0';
	}

	//linear search
	for (int e = 0; e <= item_no; e++)
	{

		for (int f = 0; f <= 202; f++)
		{
			if (strcmp(items[f], list[e]) == 0)
			{
				index_positions[e] = f;
				break;
			}
			else
			{
				index_positions[e] = -1;
			}
		}
	}

	SetConsoleTextAttribute(hconsole, FOREGROUND_RED);
	printf("\nYour Shopping List has been received!\n");
	SetConsoleTextAttribute(hconsole, FOREGROUND_BLUE | FOREGROUND_RED);
	printf("\nHere is your optimal route. Follow ");
	SetConsoleTextAttribute(hconsole, FOREGROUND_GREEN);
	printf("GREEN ");
	SetConsoleTextAttribute(hconsole, FOREGROUND_BLUE | FOREGROUND_RED);
	printf("path to save your time.\n\n");
	
	for (int pass = 1; pass < item_no; pass++)
	{
		for (int s_pass = 0; s_pass < item_no - 1; s_pass++)
		{
			if (index_positions[s_pass] > index_positions[s_pass + 1])
			{
				int hold = index_positions[s_pass];
				index_positions[s_pass] = index_positions[s_pass + 1];
				index_positions[s_pass + 1] = hold;
			}
		}
	}

	//allocate dynamic memory for adjacency matrix to store distances
	int** ad_mat = (int**)malloc(sizeof(int*) * (item_no));
	for (int i = 0; i < item_no; i++)
	{
		ad_mat[i] = (int*)malloc(sizeof(int*) * (item_no));
	}

	//allocate memory for minimum spannig tree
	int** mst = (int**)malloc(sizeof(int*) * (item_no));
	for (int i = 0; i < item_no; i++)
	{
		mst[i] = (int*)malloc(sizeof(int*) * 2);
	}

	int k = 0;
	for (int h = 0; h < item_no; h++)
	{
		d_coordinates[k][0] = x[index_positions[h]];
		d_coordinates[k][1] = y[index_positions[h]];
		k++;
	}
	int m = 0;
	for (int i = 0; i < item_no; i++)
	{
		start[0][0] = d_coordinates[i][0];
		start[0][1] = d_coordinates[i][1];
		for (int l = 0; l < item_no; l++)
		{
			stop[0][0] = d_coordinates[l][0];
			stop[0][1] = d_coordinates[l][1];
			ad_mat[i][l] = findShortestPathLength(location, start[0][0], start[0][1], stop[0][0], stop[0][1]);
			if (ad_mat[i][l] == 0)
			{
				ad_mat[i][l] = 999;
			}
			m++;
		}
	}

	//kruskaal algorithm
	int min = 999, mincost = 0, v1, v2, ne = 1, u, v, ed1, ed2;
	int* parent;
	parent = (int*)(malloc(sizeof(int*) * (item_no)));
	for (int o = 0; o < item_no; o++)
	{
		parent[o] = o;
	}
	int z = 0;
	while (ne < item_no)
	{
		min = 999;
		for (int i = 0; i < item_no; i++)
		{
			for (int j = 0; j < item_no; j++)
			{
				if (ad_mat[i][j] < min)
				{
					min = ad_mat[i][j];
					v1 = u = ed1 = i;
					v2 = v = ed2 = j;
				}
			}
		}

		for (int p = 0; p < item_no; p++)
		{
			if (p == ed1)
			{
				u = parent[ed1];
			}
			if (p == ed2)
			{
				v = parent[ed2];
			}
		}

		int go = 0;
		if (u != v)
		{
			parent[ed2] = u;
			go = 1;
		}
		else
		{
			go = 0;
		}

		if (go == 1)
		{
			mst[z][0] = v1;
			mst[z][1] = v2;
			ne++;
			mincost += min;
			z++;
		}
		ad_mat[v1][v2] = ad_mat[v2][v1] = 999;

	}

	// display of output

	int value1, value2;
	//allocate dynamic memory for adjacency matrix to store distances
	int** route = (int**)malloc(sizeof(int*) * (ROW));
	for (int i = 0; i < ROW; i++)
	{
		route[i] = (int*)malloc(sizeof(int*) * (ROW));
	}

	//allocate dynamic memory for adjacency matrix to store distances
	int** t_route = (int**)malloc(sizeof(int*) * (COLUMN));
	for (int i = 0; i < COLUMN; i++)
	{
		t_route[i] = (int*)malloc(sizeof(int*) * (COLUMN));
	}

	for (int i = 0; i < ROW; i++)
		memcpy(route[i], location[i], sizeof(location[i]));
	for (int i = 0; i < ROW; i++)
		memcpy(t_route[i], location[i], sizeof(location[i]));


	for (int q = 0; q < item_no - 1; q++)
	{

		for (int i = 0; i < ROW; i++)
			memcpy(t_route[i], route[i], sizeof(route[i]));


		value1 = mst[q][0];
		value2 = mst[q][1];
		start[0][0] = d_coordinates[value1][0];
		start[0][1] = d_coordinates[value1][1];
		stop[0][0] = d_coordinates[value2][0];
		stop[0][1] = d_coordinates[value2][1];
		displayShortestPathLength(location, route, t_route, start[0][0], start[0][1], stop[0][0], stop[0][1], q + 2);

	}

	int ends[2];
	int ind = 0;
	bool found = false;
	for (int i = 0; i < item_no - 1; i++) {
		found = false;
		for (int j = 0; j < item_no - 1; j++) {
			if (mst[i][0] == mst[j][1]) {
				found = true;
				break;
			}
		}
		if (!found) {
			ends[ind++] = mst[i][0];
			break;
		}
	}

	for (int i = 0; i < item_no - 1; i++) {
		found = false;
		for (int j = 0; j < item_no - 1; j++) {
			if (mst[i][1] == mst[j][0]) {
				found = true;
				break;
			}
		}
		if (!found) {
			ends[ind++] = mst[i][1];
			break;
		}
	}

	displayShortestPathLength(location, route, t_route, 19, 10, d_coordinates[ends[0]][0], d_coordinates[ends[0]][1], 83);
	displayShortestPathLength(location, route, t_route, 19, 11, d_coordinates[ends[1]][0], d_coordinates[ends[1]][1], 69);

	
	// display of path to the user
	for (int s = 0; s < 20; s++)
	{
		for (int t = 0; t < 20; t++)
		{
			SetConsoleTextAttribute(hconsole, FOREGROUND_GREEN);
			if (route[s][t] == 1)
			{
				SetConsoleTextAttribute(hconsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
				printf(" %d", route[s][t]);
			}
			else if (route[s][t] == 0)
			{
				SetConsoleTextAttribute(hconsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				printf(" %d", route[s][t]);
			}
			else if (route[s][t] >= 10)
				printf(" %c", (char)route[s][t]);
			else
				printf(" %d", route[s][t]);
		}
		printf("\n");
	}

	SetConsoleTextAttribute(hconsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("\n=================================================================================================================================================\n");
	printf("\n\t\t\t\t%5c%5c%5c       GOOD BYE       %c%5c%5c\t\t\t\t\n", 4, 4, 4, 4, 4, 4);
	printf("\n=================================================================================================================================================\n\n");
	SetConsoleTextAttribute(hconsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	for (int i = 0; i < item_no; i++)
		free(ad_mat[i]);

	free(ad_mat);

	for (int i = 0; i < item_no; i++)
		free(mst[i]);

	free(mst);
	return 0;

}


// Checking if it is possible to go to (x, y) from the current position.
int isSafe(int mat[20][20], char visited[20][20], int x, int y, int row, int col)
{
	return (x >= 0 && x < 20 && y >= 0 && y < 20) &&
		mat[x][y] != 1 && !visited[x][y];
}

int findShortestPath(int mat[20][20], char visited[20][20], int i, int j, int x, int y, int min_dist, int dist)
{

	if ((i == x) && (j == y))
	{
		min_dist = ((min_dist < dist) ? min_dist : dist);
		return min_dist;
	}
	//Check if we have reached near the destination
	if ((i == x - 1 && j == y) || (i == x + 1 && j == y) || (i == x && j == y - 1) || (i == x && j == y + 1))
	{
		min_dist = ((min_dist < dist) ? min_dist : dist + 1);
		return min_dist;
	}

	// set (i, j) cell as visited
	visited[i][j] = 1;
	// go to the bottom cell
	if (isSafe(mat, visited, i + 1, j, x, y) && min_dist > dist + 1) {

		min_dist = findShortestPath(mat, visited, i + 1, j, x, y, min_dist, dist + 1);
	}
	// go to the right cell
	if (isSafe(mat, visited, i, j + 1, x, y) && min_dist > dist + 1) {

		min_dist = findShortestPath(mat, visited, i, j + 1, x, y, min_dist, dist + 1);
	}
	// go to the top cell
	if (isSafe(mat, visited, i - 1, j, x, y) && min_dist > dist + 1) {

		min_dist = findShortestPath(mat, visited, i - 1, j, x, y, min_dist, dist + 1);
	}
	// go to the left cell
	if (isSafe(mat, visited, i, j - 1, x, y) && min_dist > dist + 1) {

		min_dist = findShortestPath(mat, visited, i, j - 1, x, y, min_dist, dist + 1);
	}
	// backtrack: remove (i, j) from the visited matrix
	visited[i][j] = 0;
	return min_dist;
}

int findShortestPathLength(int mat[20][20], int x1, int y1, int x2, int y2) {
	if ((mat[x1 - 1][y1] && mat[x1 + 1][y2] && mat[x1][y1 - 1] && mat[x1][y1 + 1]) || (mat[x2 - 1][y2] && mat[x2 + 1][y2] && mat[x2][y2 - 1] && mat[x2][y2 + 1]))
		return -1;
	if ((x1 == 19 && y1 == 10 && x2 == 19 && y2 == 11) || (x1 == 19 && y1 == 11 && x2 == 19 && y2 == 10))
	{
		return 0;
	}

	char visited[20][20];
	memset(visited, 0, sizeof(visited));

	int dis = MAX_PATH;
	dis = findShortestPath(mat, visited, x1, y1, x2, y2, dis, 0);

	if (dis != MAX_PATH)
		return dis;
	return -1;
}

int displayShortestPath(int mat_in[20][20], char mat2[20][20], char t_map[20][20], char visited[20][20], int i, int j, int x, int y, int min_dist, int dist, int pass)
{
	int mat[20][20];
	memcpy(mat, mat_in, sizeof(mat));

	if ((i == x) && (j == y))
	{
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				mat2[i][j] = t_map[i][j];
			}
		}

		min_dist = ((min_dist < dist) ? min_dist : dist);
		for (int p1 = 0; p1 < 20; p1++)
		{
			for (int p2 = 0; p2 < 20; p2++)
			{
				if (visited[p1][p2] == 1 && mat2[p1][p2] == 0)
				{
					mat2[p1][p2] = pass;
				}
			}
		}
		return min_dist;
	}
	//Check if we have reached near the destination
	if ((i == x - 1 && j == y) || (i == x + 1 && j == y) || (i == x && j == y - 1) || (i == x && j == y + 1))
	{
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				mat2[i][j] = t_map[i][j];
			}
		}

		min_dist = ((min_dist < dist) ? min_dist : dist + 1);
		for (int p1 = 0; p1 < 20; p1++)
		{
			for (int p2 = 0; p2 < 20; p2++)
			{
				if (visited[p1][p2] == 1 && mat2[p1][p2] == 0)
				{
					mat2[p1][p2] = pass;
				}
			}
		}
		return min_dist;
	}

	// set (i, j) cell as visited
	visited[i][j] = 1;
	// go to the bottom cell
	if (isSafe(mat, visited, i + 1, j, x, y) && min_dist > dist+1) {

		min_dist = displayShortestPath(mat, mat2, t_map, visited, i + 1, j, x, y, min_dist, dist + 1, pass);
	}
	// go to the right cell
	if (isSafe(mat, visited, i, j + 1, x, y) && min_dist > dist + 1) {

		min_dist = displayShortestPath(mat, mat2, t_map, visited, i, j + 1, x, y, min_dist, dist + 1, pass);
	}
	// go to the top cell
	if (isSafe(mat, visited, i - 1, j, x, y) && min_dist > dist + 1 ) {

		min_dist = displayShortestPath(mat, mat2, t_map, visited, i - 1, j, x, y, min_dist, dist + 1, pass);
	}
	// go to the left cell
	if (isSafe(mat, visited, i, j - 1, x, y) && min_dist > dist + 1 ) {

		min_dist = displayShortestPath(mat, mat2, t_map, visited, i, j - 1, x, y, min_dist, dist + 1, pass);
	}
	// backtrack: remove (i, j) from the visited matrix
	visited[i][j] = 0;
	return min_dist;
}

int displayShortestPathLength(int mat[20][20], int **t_mat, int **t_mat2, int x1, int y1, int x2, int y2, int pass) {
	if ((mat[x1 - 1][y1] && mat[x1 + 1][y2] && mat[x1][y1 - 1] && mat[x1][y1 + 1]) || (mat[x2 - 1][y2] && mat[x2 + 1][y2] && mat[x2][y2 - 1] && mat[x2][y2 + 1]))
		return -1;
	if ((x1 == 19 && y1 == 10 && x2 == 19 && y2 == 11) || (x1 == 19 && y1 == 11 && x2 == 19 && y2 == 10))
	{
		return 0;
	}

	char visited[20][20];
	memset(visited, 0, sizeof(visited));

	char t_mat_st[20][20];
	char t_mat2_st[20][20];
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			t_mat_st[i][j] = (char)t_mat[i][j];
		}
	}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			t_mat2_st[i][j] = (char)t_mat2[i][j];
		}
	}


	int dis = MAX_PATH;
	dis = displayShortestPath(mat, t_mat_st, t_mat2_st, visited, x1, y1, x2, y2, dis, 0, pass);

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			t_mat2[i][j] = (int)t_mat_st[i][j];
		}
	}

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			t_mat[i][j] = (int)t_mat_st[i][j];
		}
	}

	if (dis != MAX_PATH)
		return dis;
	return -1;
}