#include "./malloc.h"

#define TINY_HEAP_SIZE (getpagesize() * 4)
#define MIDDLE_HEAP_SIZE (getpagesize() * 16)
#define TINY_BLOCK_SIZE (TINY_HEAP_SIZE / 128)
#define MIDDLE_BLOCK_SIZE (MIDDLE_HEAP_SIZE / 128)

#define HEAP_TO_BLOCK(heap) ((void*) heap + sizeof(t_heapHeader))
// #define BLOCK_TO_HEAP(block) ((void*) heap - sizeof(t_block))
#define BLOCK_TO_DATA(block) ((void*) block + sizeof(t_block))

t_heapHeader *g_heapHead;

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

t_heapHeader *getLastHeader(t_heapHeader *header)
{
    while (header && header->next)
    {
        header = header->next;
    }
    return header;
}

void *createNewHeapHeader(t_heapHeader *newHeap, t_group group, size_t totalHeapSize)
{
    t_heapHeader *lastHeader = getLastHeader(g_heapHead);
    newHeap->next = NULL;
    newHeap->group = group;
    newHeap->totalSize = totalHeapSize;
    if (lastHeader)
    {
        lastHeader->next = newHeap;
        newHeap->prev = lastHeader;
    }
    else
        g_heapHead = newHeap;
}

void createNewBlock(size_t size, t_heapHeader *newHeap, t_group group, size_t totalHeapSize)
{
    t_block *firstBlock = HEAP_TO_BLOCK(newHeap);
    firstBlock->isFree = 0;
    firstBlock->size = size;

    if (group != LARGE)
    {
        t_block *secondBlock = (t_block *)((char *)firstBlock + sizeof(t_block) + size);
        secondBlock->isFree = 1;
        secondBlock->next = NULL;
        secondBlock->size = totalHeapSize - (sizeof(t_block) * 2) - sizeof(t_heapHeader);
        firstBlock->next = secondBlock;
    }
    else
        firstBlock->next = NULL;
}

void initHeaders(size_t size, t_heapHeader *newHeap, size_t totalHeapSize)
{
    t_group group = getGroupBySize(size);
    createNewHeapHeader(newHeap, group, totalHeapSize);
    createNewBlock(size, newHeap, group, totalHeapSize);
}

void *createNewHeap(size_t size)
{
    size_t totalHeapSize = getNewHeapSize(size);

    t_heapHeader *newHeap = mmap(NULL, totalHeapSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (newHeap == MAP_FAILED)
    {
        return NULL;
    }
    initHeaders(size, newHeap, totalHeapSize);
    return newHeap;
}

t_block *findBlock(size_t size)
{
    t_heapHeader *currentHeap = g_heapHead;
    t_group group = getGroupBySize(size);

    while(currentHeap)
    {
        if(currentHeap->group == group)
        {
            t_block* currentBlock = currentHeap->blocks;
            while(currentBlock)
            {
                if(!currentBlock->isFree && currentBlock->size >= size + sizeof(t_block))
                {
                    return currentBlock;
                }
                currentBlock = currentBlock->next;
            }
        }
        currentHeap = currentHeap->next;
    }
    return NULL;
}

// Make sure to check if there's space for a new Block, otherwise dont split
void *splitBlock(t_block *block, size_t size)
{
    t_block *newBlock;

    newBlock = BLOCK_TO_DATA(block) + size;
    newBlock->isFree = 1;
    newBlock->next = block->next;
    newBlock->size = block->size - size - sizeof(t_block);
    block->isFree = 0;
    block->next = newBlock;
    block->size = size;
}

void *malloc(size_t size)
{
    void *ret;
    t_block *block;

    if (size <= 0)
        return NULL;
    if(block = findBlock(size))
    {
        ret = splitBlock(block, size);

    }
    ret = createNewHeap(size);
    return ret; // Return the allocated memory address
}

int main()
{
    void *allocatedMemory1 = malloc(25666);
    return 0;
}
