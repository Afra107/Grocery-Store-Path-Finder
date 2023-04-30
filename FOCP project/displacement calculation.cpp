#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
// distance function declaration
float distance(float x1, float x2, float y1, float y2);
int main(void)
{
	int n;
	float x_1, x_2, y_1, y_2;
	// array for storing distances
	float dis[1000];
	// array containing coordinates of items
	int coordinate[202][2];

	printf("no of items (coordinates): ");
	scanf_s("%d", &n);
	printf("\n");

	// coordinates of entry point
	coordinate[0][0] = 20;
	coordinate[0][1] = 10;
	// coordinates of items
	for (int k = 1; k <= n; k++)
	{
		printf("Enter coordinates of item %d:  ",k);
		scanf_s("%d , %d", &coordinate[k][0], &coordinate[k][1]);
	}
	printf("\n\n");
	// coordinates of checkout
	coordinate[n + 1][0] = 20;
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
			// calling 'distance' function
			dis[d] = distance(x_1, x_2, y_1, y_2);
			// only for display
			if (i == 0 && j == (n + 1))
				printf("distance between entry and checkout is:\t%.2f\n", dis[d]);
			else if (i == 0)
				printf("distance between entry and item %d is:\t%.2f\n", j, dis[d]);
			else if (j == n + 1)
				printf("distance between item %d and checkout is: %.2f\n", i, dis[d]);
			else
				printf("distance between item %d and item %d is:\t%.2f\n", i, j, dis[d]);

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
		printf("%d\t%.2f\n", l, dis[l]);
	}
	return 0;
}

// distance function
float distance(float x1, float x2, float y1, float y2)
{
	float dist;
	dist = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	return dist;
}