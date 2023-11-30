#include "./malloc.h"

int getGroupBySize(size_t size)
{
    if (size <= (size_t)TINY_BLOCK_SIZE)
        return TINY;
    else if (size <= (size_t)MIDDLE_BLOCK_SIZE)
        return MIDDLE;
    return LARGE;
}

int getNewHeapSize(size_t size)
{
    if (size <= (size_t)TINY_BLOCK_SIZE)
        return (size_t)TINY_HEAP_SIZE;
    else if (size <= (size_t)MIDDLE_BLOCK_SIZE)
        return (size_t)MIDDLE_HEAP_SIZE;
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
    ft_printf("Malloced %p -  %p\n", (void *)newHeap, (void *)((char *)newHeap + totalHeapSize));
    initHeaders(size, newHeap, totalHeapSize);
    return HEAP_TO_BLOCK(newHeap);
}

void *malloc(size_t size)
{
    void *ret;
    t_block *block;
    int perfectFit = 0;

    if (size <= 0)
        return NULL;
    block = findBlock(size, &perfectFit);
    if(perfectFit)
    {
        block->isFree = 0;
        return BLOCK_TO_DATA(block);
    }
    if (block)
    {
        splitBlock(block, size);
        return BLOCK_TO_DATA(block);
    }
    ret = createNewHeap(size);
    return BLOCK_TO_DATA(ret);
}
