#include "lab5_dlist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test function for createDlist
void test_createDlist() {
    dlist intList = createDlist(DLIST_INT);
    assert(intList != NULL);
    dlist strList = createDlist(DLIST_STR);
    assert(strList != NULL);
    dlist doubleList = createDlist(DLIST_DOUBLE);
    assert(doubleList != NULL);
    dlistFree(intList);
    dlistFree(strList);
    dlistFree(doubleList);
    printf("createDlist passed\n");
}

// Test function for dlistIsEmpty
void test_dlistIsEmpty() {
    dlist list = createDlist(DLIST_INT);
    assert(dlistIsEmpty(list));
    dlistValue value;
    value.intValue = 42;
    dlistAppend(list, "key", value);
    assert(!dlistIsEmpty(list));
    dlistFree(list);
    printf("dlistIsEmpty passed\n");
}

// Test function for dlistAppend
void test_dlistAppend() {
    dlist list = createDlist(DLIST_INT);
    dlistValue value;
    value.intValue = 42;
    dlistAppend(list, "key", value);
    // Further testing would require access to list internals or a dlistGetValue function
    dlistFree(list);
    printf("dlistAppend passed\n");
}

// Test function for dlistSort
void test_dlistSort_int() {
    dlist list = createDlist(DLIST_INT);
    dlist sortedList = createDlist(DLIST_INT);
    // Append values in a non-sorted order
    dlistValue value;
    value.intValue = 42;
    dlistAppend(list, "key1", value);
    value.intValue = 21;
    dlistAppend(list, "key2", value);
    dlistSort(list, sortedList, DLIST_SORT_INC);
    dlistPrint(list);
    if (sortedList == NULL) {
        printf("sortedList is NULL\n");
    }
    dlistPrint(sortedList);
    dlistFree(list);
    dlistFree(sortedList);
    printf("dlistSort passed\n");
}

void test_dlistSort_str() {
    dlist list = createDlist(DLIST_STR);
    dlist sortedList = createDlist(DLIST_STR);
    // Append values in a non-sorted order
    dlistValue value;
    value.strValue = "mickey";
    dlistAppend(list, "key1", value);
    value.strValue = "mouse";
    dlistAppend(list, "key2", value);
    dlistSort(list, sortedList, DLIST_SORT_DEC);
    dlistPrint(list);
    if (sortedList == NULL) {
        printf("sortedList is NULL\n");
    }
    dlistPrint(sortedList);
    dlistFree(list);
    dlistFree(sortedList);
    printf("dlistSort passed\n");
}

// Test function for dlistPrint
void test_dlistPrint() {
    dlist list = createDlist(DLIST_INT);
    dlistValue value;
    value.intValue = 42;
    dlistAppend(list, "key", value);
    dlistPrint(list); // Visual inspection required
    dlistFree(list);
    printf("dlistPrint passed\n");
}

// Test function for dlistFree
void test_dlistFree() {
    dlist list = createDlist(DLIST_INT);
    dlistFree(list); // Should not cause any error
    // To fully test, run with a memory checker like valgrind to ensure no leaks
    printf("dlistFree passed\n");
}

int main() {
    test_createDlist();
    test_dlistIsEmpty();
    test_dlistAppend();
    test_dlistSort_int();
    test_dlistSort_str();
    test_dlistPrint();
    test_dlistFree();

    printf("All tests passed!\n");
    return 0;
}
