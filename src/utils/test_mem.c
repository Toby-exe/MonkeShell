#include <stdio.h>
#include <string.h>
#include "str.h"
#include "mem.h"

// #define HEAP_LEN 	4096
// #define MAX_BLOCKS 	2048
// #define MAX_ARGS 	1024

// char heap[HEAP_LEN];
// size_t heapOffset = 0;

// //used to separate data from metadata and keep track of allocated blocks even after a free
// typedef struct {
//     void *ptr;
//     size_t size;
// } Block;

// Block allocatedBlocks[MAX_BLOCKS];
// size_t blockCount = 0;

// /*
// allocates block_sz bytes in the heap array and returns a pointer to the
// allocated block in the heap.
// can store any type using (type *)alloc(amount(in size_t) x sizeof(type))
// */
// void *alloc(size_t block_sz)
// {
//     if (heapOffset + block_sz > HEAP_LEN) 	//will be replaced by heap = resize(); if implemented
// 	{
//         printf("OUT OF MEMORY\n");
//         return NULL;
//     }

// 	//allocate memory from the heap
// 	void *ptr = &heap[heapOffset];
// 	heapOffset += block_sz;

// 	//Store the allocated block's information in allocatedBlocks
// 	if (blockCount < MAX_BLOCKS) 
// 	{
//         allocatedBlocks[blockCount].ptr = ptr;
//         allocatedBlocks[blockCount].size = block_sz;
//         blockCount++;
//     } 
// 	else
//         printf("MAX BLOCKS EXCEEDED\n");

// 	return ptr;
// }

// void *_realloc(void *ptr, size_t block_sz)
// {
// 	// must keep previous ptr intact
// 	void *newBlock;
// 	_free(ptr);
// 	newBlock = alloc(block_sz);

// 	return newBlock;
// }

// /*
// shift elements to the right of the ptr to fill in the gap
// increment through heap array until /0 or HEAP_LEN - 1
// shift heapOffset to left by the same amount
// */
// void _free(void *ptr)
// {
// 	// invalid pointer
// 	if (ptr == NULL || ptr < heap || ptr >= heap + HEAP_LEN)
// 	{
// 		printf("BAD POINTER\n");
// 		return;
// 	}

// 	// Find the block information based on the pointer
//     int blockIndex = -1;
//     for (int i = 0; i < blockCount; i++) 
// 	{
//         if (allocatedBlocks[i].ptr == ptr) 
// 		{
//             blockIndex = i;
//             break;
//         }
//     }

// 	//pointer wasn't found in the allocatedBlocks array
// 	if(blockIndex = -1) 
// 	{
// 		printf("INVALID POINTER\n");
// 		return;
// 	}

// 	 // Get the size of the freed block
//     size_t block_size = allocatedBlocks[blockIndex].size;

//     // Update the heapOffset by shifting it left by the size of the freed block
//     heapOffset -= block_size;

//     // Shift elements to the right of the freed block to fill the gap
//     char *src = (char *)ptr + block_size;
//     char *dst = (char *)ptr;

//     while (src < heap + heapOffset + HEAP_LEN) {
//         *dst++ = *src++;
//     }

// 	// Null-terminate the remaining part after shifting
//     while (dst < heap + heapOffset + HEAP_LEN) {
//         *dst++ = '\0';
//     }

//     // Remove the freed block from the allocatedBlocks array
//     for (int i = blockIndex; i < blockCount - 1; i++) {
//         allocatedBlocks[i] = allocatedBlocks[i + 1];
//     }

//     blockCount--;
// }

// void free_all()
// {
// 	// reset heap size if resize() gets implemented properly
// 	char *ptr = heap;

// 	// clear heap (memset(heap, '\0', HEAP_LEN);)
// 	while (ptr < heap + heapOffset)
// 		*ptr++ = (0);

// 	// reset offset
// 	heapOffset = 0;
// }

void printHeap()
{
	printf("Entire Heap: ");
	for (int i = 0; i < HEAP_LEN; i++)
	{
		if (heap[i] == '\0')
			printf("0");
		else
			putchar(heap[i]);
	}

	printf("\n");
	//printBlocks();
}

// void printBlocks()
// {
// 	printf("All Blocks: ");
// 	for (int i = 0; i < MAX_BLOCKS; i++)
// 	{
// 		const char *printPtr = (char *)allocatedBlocks[i].ptr;
// 		for(int j = 0; j < allocatedBlocks[i].size; j++)
// 		{
// 			if (printPtr == '\0')
// 				printf("0");
// 			else
// 				putchar(heap[i]);

// 			printPtr++;
// 		}
// 		printf("\n");
// 	}

// 	printf("\n");
// }

// eventually replace this with a series of CMake google tests
int main()
{
	// *** alloc, free, free_all test *** //
	char *strings[1024];
	char buff[64];
	char *s;
	int d;

	strings[0] = alloc((strlen("my balls") + 1) * sizeof(char));
	strcpy(strings[0], "my balls");

	strings[1] = alloc((strlen("pluh") + 1) * sizeof(char));
	strcpy(strings[1], "pluh");

	int *i = (int *)alloc(3 * sizeof(int));
	*i = 4;
	*(i + 1) = 8;
	*(i + 2) = 7;

	strings[2] = alloc((strlen("yessir") + 1) * sizeof(char));
	strcpy(strings[2], "yessir");

	printHeap();

	_free(strings[1]);
	// after a free, the freed block get taken over by the block next to (as expected)
	// but any blocks after lose their start locations. need a way to shift
	// all pointer rather than just elements.
	//printf("\n%s\n", strings[2]);
	// free_all();

	printHeap();

	strings[3] = alloc((strlen("yuh") + 1) * sizeof(char));
	strcpy(strings[3], "yuh");

	printHeap();

    /*scanf("%d", d);

	for(int i = 0; i < d; i++)
	{
		s = fgets(buff, 64, stdin);

		if(s[0] == '.')
			break;

		strings[i] = alloc((strlen(s) + 1) * sizeof(char));
		strcpy(strings[i], s);
	}*/

	// *** realloc test *** //
	/*int *ptr = (int *)malloc(sizeof(int) * 2);
	int i;
	int *ptr_new;

	*ptr = 10;
	*(ptr + 1) = 20;

	ptr_new = (int *)realloc(ptr, sizeof(int) * 3);
	*(ptr_new + 2) = 30;
	for (i = 0; i < 3; i++)
		printf("%d ", *(ptr_new + i));

	getchar();
	return 0;*/

	return 0;
}