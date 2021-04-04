#include <stdio.h>
#include "sort.h"

void main()
{
	int i;
	//定义整型数组
	int array[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };
	int array2[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };

	//计算数组长度
	int len = sizeof(array) / sizeof(int);
	//遍历数组
	printf("Sort Array:\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d\t", array[i]);
	}
	printf("\n");

	// selectionSort
	selectionSort(array, len);
	// printf
	printf("Selection Sort:\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d  ", array[i]);
	}
	
	// insertionSort
	insertionSort(array2, len);
	// printf
	printf("\n==========Gorgeous Split Line==========\n");
	printf("Insertion Sort:	\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d  ", array2[i]);
	}
	printf("\n");
}
