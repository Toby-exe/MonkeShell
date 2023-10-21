/**
 * @file mem.h
 * @brief A header file defining custom memory allocation functions and constants
 * @authors Tobias Wondwossen, Jayden Mingle
 * 
 * Details: This header file defines custom memory allocation functions alloc and free_all. It also defines a heap 
 * of fixed size (HEAP_LEN) for allocation, and a heapOffset variable to keep track of the current offset in the 
 * heap. The alloc function is used to allocate a specified amount of memory from the heap, and the free_all 
 * function is used to reset the heapOffset to zero, effectively freeing all allocated memory.
 * 
 * Assumptions/Limitations: The functions in this file are assumed to be compatible with the rest of the program. 
 * The alloc function does not handle cases where the requested size exceeds the remaining space in the heap. The 
 * free_all function does not individually free allocated blocks, but resets the entire heap. The heap size is 
 * fixed and defined by HEAP_LEN.
 *
 *@date 2023-10-20
 */
#ifndef MEM_H
#define MEM_H

// ********** custom memory allocation ********** 
/*
*   EXAMPLE:
*             |>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
*	heap ->   |\0|\0|\0|\0|\0|\0|\0|\0|  ....  |\0|
*             |>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
*	           ^
*
*   ALLOCATING A STRING:
*
*       char *token = "ls"
*       command->argv[0] = (char *)alloc((_strlen(token) + 1) * sizeof(char));
*       _strcpy(command->argv[0], token);
*
*             |>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
*	heap ->   |l |s |\0|\0|\0|\0|\0|\0|  ....  |\0|
*             |>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
*                    ^
*/
#include <stdlib.h>
#include "str.h"

#define HEAP_LEN 4096

extern char heap[HEAP_LEN];
extern size_t heapOffset;

void *alloc(size_t);
void free_all();

#endif