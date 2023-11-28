#include "./malloc.h"

int getGroupBySize(size_t size)
{
    if (size <= TINY_BLOCK_SIZE)
        return TINY;
    else if (size <= MIDDLE_BLOCK_SIZE)
        return MIDDLE;
    return LARGE;
}

int getNewHeapSize(size_t size)
{
    if (size <= TINY_BLOCK_SIZE)
        return TINY_HEAP_SIZE;
    else if (size <= MIDDLE_BLOCK_SIZE)
        return MIDDLE_HEAP_SIZE;
    return size + sizeof(t_heapHeader) + sizeof(t_block);
}

void initHeaders(size_t size, t_heapHeader *newHeap, size_t totalHeapSize)
{
    t_group group = getGroupBySize(size);
    createNewHeapHeader(newHeap, group, totalHeapSize);
    createNewBlock(size, newHeap, group, totalHeapSize);
    newHeap->blocks = HEAP_TO_BLOCK(newHeap);
}

void *createNewHeap(size_t size)
{
    size_t totalHeapSize = getNewHeapSize(size);
    t_heapHeader *newHeap = mmap(NULL, totalHeapSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (newHeap == MAP_FAILED)
        return NULL;
    initHeaders(size, newHeap, totalHeapSize);
    return HEAP_TO_BLOCK(newHeap);
}

void *malloc(size_t size)
{
    void *ret;
    t_block *block;

    if (size <= 0)
        return NULL;
    if(block = findBlock(size))
    {
        splitBlock(block, size);
        return BLOCK_TO_DATA(block);
    }
    ret = createNewHeap(size);
    return BLOCK_TO_DATA(ret);
}
