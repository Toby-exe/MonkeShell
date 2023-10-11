#include "mem.h"

char heap[HEAP_LEN];
size_t heapOffset = 0;
Block allocatedBlocks[MAX_BLOCKS];
size_t blockCount = 0;

//call before an alloc if the alloc would go out of bounds (dynamic array)
char *resize()
{
    /*int oldSize = sizeof(heap) / sizeof(heap[0]);
    int newSize = oldSize * 1.25;
    size_t newHeap[newSize];

    _strncpy(newHeap, heap, oldSize);

    return newHeap;*/
}

/*
allocates block_sz bytes in the heap array and returns a pointer to the
allocated block in the heap.
can store any type using (type *)alloc(amount(in size_t) x sizeof(type))
*/
void *alloc(size_t block_sz)
{
	/*if(heapOffset + block_sz >= HEAP_LEN)
		heap = resize();*/

	void *ptr = &heap[heapOffset];
	heapOffset += block_sz;
	return ptr;
}

void *_realloc(void *ptr, size_t block_sz)
{
	// must keep previous ptr intact
	void *newBlock;
	_free(ptr);
	newBlock = alloc(block_sz);

	return newBlock;
}

/*
shift elements to the right of the ptr to fill in the gap
increment through heap array until /0 or HEAP_LEN - 1
shift heapOffset to left by the same amount
*/
void _free(void *ptr)
{
	// invalid pointer
	if (ptr == NULL || ptr < heap || ptr >= heap + HEAP_LEN)
	{
		//printf("BAD POINTER\n");
		return;
	}

	char *block_start = (char *)ptr;
	char *block_end = block_start;

	// Find the end of the allocated block or the end of the heap
	while (*block_end != '\0' && block_end < heap + HEAP_LEN)
	{
		block_end++;
	}

	// Calculate the size of the freed block
	size_t block_size = block_end - block_start + 1; //(first character to \0)
	//printf("SIZE OF THE FREED BLOCK IS %zu\n", block_size);

	// Shift elements to the right of the freed block to fill the gap
	char *src = block_end;
	char *dst = block_start;

	//printf("START = %c & END = %c", *dst, *src);

	//*src < heap + heapOffset &&
	src++; // skip null terminator of freed block so it isn't copied
	while (*src < heap + heapOffset && src < heap + HEAP_LEN)
	{
		*dst++ = *src++;
	}

	// Null-terminate the remaining part after shifting
	while (dst < heap + HEAP_LEN)
	{
		*dst++ = '\0';
	}

	// Update heapOffset by shifting it left by the same amount
	heapOffset -= block_size;
}

void free_all()
{
	// reset heap size if resize() gets implemented properly
	char *ptr = heap;

	// clear heap (memset(heap, '\0', HEAP_LEN);)
	while (ptr < heap + heapOffset)
		*ptr++ = NULL;

	// reset offset
	heapOffset = 0;
}