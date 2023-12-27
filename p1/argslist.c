#include "argslist.h"

char *stringCopy(const char *s)
{
    size_t len = strlen(s) + 1;
    void *new = malloc(len);
    if (new == NULL)
        return NULL;
    return (char *)memcpy(new, s, len);
}

// Function to initialize a new dynamic string array
void initDSA(DynamicStringArray *arr, size_t initialCapacity)
{
    arr->data = (char **)malloc(initialCapacity * sizeof(char *));
    if (arr->data == NULL)
    {
        perror("Failed to allocate memory for dynamic array");
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = initialCapacity;
}

// Function to add a new string to the dynamic array
void addDSA(DynamicStringArray *arr, char* str)
{
    if (arr->size == arr->capacity)
    {
        // If the array is full, double its capacity
        arr->capacity *= 2;
        arr->data = (char **)realloc(arr->data, arr->capacity * sizeof(char *));
        if (arr->data == NULL)
        {
            perror("Failed to reallocate memory for dynamic array");
            exit(EXIT_FAILURE);
        }
    }

    // Allocate memory for the new string and copy it
    if (str == NULL)
    {
        arr->data[arr->size] = NULL;
        arr->size++;
        return;
    }
    arr->data[arr->size] = stringCopy(str);
    if (arr->data[arr->size] == NULL)
    {
        perror("Failed to allocate memory for string");
        exit(EXIT_FAILURE);
    }
    arr->size++;
}

// Function to remove an element from the dynamic array
void removeDSA(DynamicStringArray *arr, size_t index)
{
    if (index >= arr->size)
    {
        perror("Index out of bounds");
        exit(EXIT_FAILURE);
    }
    free(arr->data[index]);
    for (size_t i = index; i < arr->size - 1; i++)
    {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
}

// Function to free the memory used by the dynamic array
void freeDSA(DynamicStringArray *arr)
{
    if (arr->size == 0)
    {
        return;
    }
    for (size_t i = 0; i < arr->size; i++)
    {
        free(arr->data[i]);
    }
    free(arr->data);
    arr->size = 0;
    arr->capacity = 0;
}

/* for debug
void printDSA(DynamicStringArray *arr)
{
    for (size_t i = 0; i < arr->size; i++)
    {
        if (arr->data[i] != NULL)
            fprintf(stderr, "%s\n", arr->data[i]);
        else
            fprintf(stderr, "NULL\n");
    }
    fflush(stderr);
}
*/

void deleteFrontDSA(DynamicStringArray *arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        removeDSA(arr, 0);
    }
}

/*
int main() {
    DynamicStringArray myArray;
    initDSA(&myArray, 10); // Initialize with an initial capacity of 10

    char input[1024];
    fgets(input, 1024, stdin);
    for (char *temp = strtok(input, " \n"); temp != NULL; temp = strtok(NULL, " \n"))
    {
        printf("%s\n", temp);
        addDSA(&myArray, temp);
    }

    printf("The array has %zu elements\n", myArray.size);

    // Print the strings
    for (size_t i = 0; i < myArray.size; i++) {
        printf("%s\n", myArray.data[i]);
    }

    addDSA(&myArray, NULL);

    // Free the memory used by the dynamic array
    freeDSA(&myArray);
}
*/
