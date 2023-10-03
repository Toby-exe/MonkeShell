#ifndef MEM_H
#define MEM_H
//Where static allocation with a heap will be
//can be implemented with an array, a doubly linked list, or a binary search tree
//eventually may be replaced with a custom malloc/calloc and free
#include "types.h"

#define HEAP_LEN 4096

size_t heap[HEAP_LEN];

void *_alloc(int chunks);
void _free(int chunk);
void _free_all();

#endif