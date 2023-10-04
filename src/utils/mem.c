#include "mem.h"

size_t *resize()
{
    int oldSize = (sizeof(heap) / sizeof(heap[0]));
    int newSize = oldSize * 1.25;
    size_t newHeap[newSize];

    for(int i = 0; i < oldSize; i++)
    {
        newHeap[i] = heap[i];
    }

    return newHeap;
}

/*
allocates block_sz bytes in the heap array and returns a pointer to the 
allocated block in the heap.
can store any type using alloc(amount(in size_t) x sizeof(type))
*/
void *alloc(size_t block_sz)
{
    void *ptr = &heap[heapOffset];
    heapOffset += block_sz;
    return ptr;
}

void free_all()
{
    //memset(heap, '\0', 4096);
    heapOffset = 0;
}