#ifndef MEM_H
#define MEM_H

// ********** custom memory allocation ********** //
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