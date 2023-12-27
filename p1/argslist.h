#ifndef ARGS_LIST_H
#define ARGS_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// dynamic array to store strings
typedef struct {
    char** data;    // Pointer to an array of string pointers
    size_t size;    // Current number of elements in the array
    size_t capacity; // Maximum number of elements the array can hold
} DynamicStringArray;

char *stringCopy(const char *s);
// EFFECTS: Copy the string s and return a pointer to the copy

void initDSA(DynamicStringArray *arr, size_t initialCapacity);
// EFFECTS: Initialize the array with the given capacity
// MODIFIES: arr

void addDSA(DynamicStringArray *arr, char *str);
// EFFECTS: Add a string to the end of the array
// MODIFIES: arr

void removeDSA(DynamicStringArray *arr, size_t index);
// EFFECTS: Remove an element from the array

void freeDSA(DynamicStringArray *arr);
// EFFECTS: Free the memory allocated for the array
// MODIFIES: arr

/*
void printDSA(DynamicStringArray *arr);
// EFFECTS: Print the array
*/

void deleteFrontDSA(DynamicStringArray *arr, size_t n);
// EFFECTS: Delete the first n elements of the array

#endif
