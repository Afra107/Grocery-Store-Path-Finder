#include<stdio.h>

int main(void)
{
	char list[202][50];
	int item_no;
	printf("Enter number of items you want to buy:  ");
	scanf_s("%d", &item_no);
	for (int i = 0; i <= item_no; i++)
	{
		fgets(list[i], 50, stdin);
	}
	// for checking only (not to use in program)
	for (int i = 0; i <= item_no; i++)
	{
		printf("%s\n", list[i]);
	}
	return 0;
}