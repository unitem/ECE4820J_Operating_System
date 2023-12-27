#include<stdio.h>
#include<stdlib.h>
int compare(const void* e1, const void* e2)
{
	int* p1 = (int*)e1;
	int* p2 = (int*)e2;
	if (*p1 > *p2)
	{
		return 1;
	}
	else if (*p1 == *p2)
	{
		return 0;
	}
	else if (*p1 < *p2)
	{
		return -1;
	}
}
int main()
{
	int arr[10] = { 1,3,5,7,9,2,4,6,8,10 };
	qsort(arr, 10, 4, compare);
	for (int i = 0; i <= 9; i++)
	{
		printf("%d ", arr[i]);
	}
}