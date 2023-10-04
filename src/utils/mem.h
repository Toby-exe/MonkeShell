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
*   command->argv[0] = token;
*
*   |###########|###########|###########|
*	|l |s |\0|\0|\0|\0|\0|\0|\0|\0|\0|\0|
*   |###########|###########|###########|
*             ^
*/
#include <fcntl.h>
#include <unistd.h>
#include "types.h"

#define HEAP_LEN 4096
#define MAX_ARGS 1024

// ********** static memory allocation ********** //
size_t heap[HEAP_LEN];
static int heapOffset = 0;

size_t *resize();
void *alloc(size_t);
void free(int block);   //void free(void *ptr);
void free_all();

// ********** process list (for signalling) ********** //
typedef CNode *CPtr;
typedef struct
{
    pid_t pid;
    CPtr next;
    CPtr prev;
} CNode;
//cNode tail;
//cNode head;

void create_node();
void add(CNode node, CNode head);
void delete(CNode node, CNode head);
void find(pid_t pid);


#endif