#include "./malloc.h"

void splitBlock(t_block *block, size_t size)
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

//Create a second block as well with the available mem
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
                if(currentBlock->isFree && currentBlock->size >= size + sizeof(t_block))
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