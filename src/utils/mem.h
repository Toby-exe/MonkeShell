#ifndef MEM_H
#define MEM_H
//Where static allocation with a heap will be
//can be implemented with an array, a doubly linked list, or a binary search tree
//eventually may be replaced with a custom malloc/calloc and free
/*
*   |###########|###########|###########|
*	|\0|\0|\0|\0|\0|\0|\0|\0|\0|\0|\0|\0|
*   |###########|###########|###########|
*	 ^
*
*   char *token = "ls"
*   command->argv[0] = (char *)alloc(_strlen(token) + 1);
*   _strcpy(command->argv[0], token);
*
*   |###########|###########|###########|
*	|l |s |\0|\0|\0|\0|\0|\0|\0|\0|\0|\0|
*   |###########|###########|###########|
*             ^
*/
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>    //temp
#include "types.h"
#include "str.h"

#define HEAP_LEN 4096
#define MAX_BLOCKS 2048
#define MAX_ARGS 1024

// ********** static memory allocation ********** //
extern char heap[HEAP_LEN];
extern size_t heapOffset;

typedef struct
{
    void *ptr;
    size_t size;
} Block;

extern Block allocatedBlocks[MAX_BLOCKS];
extern size_t blockCount;

char *resize();
void *alloc(size_t);
void *_realloc(void *, size_t);
void _free(void *);
void free_all();

// ********** process list (for signaling) ********** //
//can just be an array of pid_t's
// typedef struct
// {
//     pid_t pid;
//     CPtr next;
//     CPtr prev;
// } CNode;
// typedef CNode *CPtr;
// pid_t *cList;
// //cNode tail;
// //cNode head;

// void create_node();
// void add(CNode node, CNode head);
// void delete(CNode node, CNode head);
// void find(pid_t pid);

#endif