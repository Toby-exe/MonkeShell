/**
 * @file mem.c
 * @authors Tobias Wondwossen, Jayden Mingle 
 * @date 2023-10-20 
 */
#include "mem.h"
char heap[HEAP_LEN];
size_t heapOffset = 0;

/**
 * @brief sets the first n bytes of the memory area pointed to by s to the specified value.
 * 
 * Details: allocates block_sz bytes in the heap array and returns a pointer to the
 * allocated block in the heap. can store any type using 
 * (type *)alloc(amount(in size_t) x sizeof(type))
 *
 * @param[in] block_sz The number of bytes to be allocated.
 * @return A pointer to the start of the allocated block in the heap.
 */
void *alloc(size_t block_sz)
{
	void *ptr = &heap[heapOffset];
	heapOffset += block_sz;
	return ptr;
}

/**
 * @brief clears the heap and resets the pointer offset.
 */
void free_all()
{
	c_memset(heap, '\0', HEAP_LEN);
	heapOffset = 0;
}