#include <stdio.h>
#include <stdlib.h>

#define SIZE 536870912 // This is roughly 2GB when each element is an int

int main()
{
    int *array1, *array2;
    long long i;

    // Allocate two large arrays, each roughly 2GB
    array1 = (int *)malloc(SIZE * sizeof(int));
    array2 = (int *)malloc(SIZE * sizeof(int));

    if (!array1 || !array2)
    {
        printf("Memory allocation failed\n");
        return 1; // Exit if any allocation fails
    }

    // Fill the arrays with data
    for (i = 0; i < SIZE; i++)
    {
        array1[i] = i % 100;
        array2[i] = i % 100;
    }

    printf("Arrays have been initialized\n");

    // Continuously access the arrays
    int j;
    for (j = 0; j < 5; ++j)
    {
        for (i = 0; i < SIZE; i++)
        {
            array1[i] = (array1[i] + array2[i]) % 100;
            array2[i] = (array1[i] - array2[i]) % 100;
        }
        printf("Arrays have been accessed\n");
    }

    // Free the arrays (this line will never be reached due to the infinite loop)
    free(array1);
    free(array2);

    return 0;
}
