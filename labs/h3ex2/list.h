#ifndef LIST_H
#define LIST_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Node
// Define the structure for a node in the linked list
{
    char *pch;
    void *pv;
    struct Node *next;
};

struct Node *createNode(char *pch, void *pv);
// Create a new node and return a pointer to it

void displayList(struct Node **head, int dataType);
// Display the contents of the linked list

void freeList(struct Node **head);
// Free the memory used by the linked list

void appendNode(struct Node **head, char *pch, void *pv);
// Add a new node at the end of the linked list

struct Node *searchValue(struct Node **head, void *target, int (*compare)(void *, void *));
// search for a specific value in the linked list
// returns a pointer to the first node in the linked list that contains the target value
// returns NULL if the target value is not found
// To search int, use compareInt; to search double, use compareDouble; to search char*, use strcmp

int compareInt(void *a, void *b);
// Comparison function for searching integers

int compareDouble(void *a, void *b);
// Comparison function for searching doubles

int readFile(struct Node **head, const char *filename);
// Read data from a file and add it to the linked list

void writeFile(struct Node **head, const char *filename);
// Write the contents of the linked list to a file

#endif
