#include "malloc.h"

int isValidPtr(void *ptr)
{
    t_heapHeader *currentHeader = g_heapHead;
    t_block *currentBlock;
    while(currentHeader)
    {
        currentBlock = currentHeader->blocks;
        while(currentBlock)
        {
            if(BLOCK_TO_DATA(currentBlock) == ptr)
                return 1;
            currentBlock = currentBlock->next;
        }
        currentHeader = currentHeader->next;
    }
    return 0;
}

void mergeRightBlock(void *ptr)
{
    t_block *block = DATA_TO_BLOCK(ptr);
    t_block *nextBlock = block->next;

    if(nextBlock && nextBlock->isFree)
    {
        if(nextBlock->next)
            nextBlock->next->prev = block;
        block->next = nextBlock->next;
        block->size += sizeof(t_block) + nextBlock->size;
    }
}

void mergeLeftBlock(void *ptr)
{
    t_block *block = DATA_TO_BLOCK(ptr);
    t_block *prevBlock = block->prev;

    if(prevBlock && prevBlock->isFree)
    {
        if(block->next)
            block->next->prev = prevBlock;
        prevBlock->next = block->next; 
        prevBlock->size += block->size + sizeof(t_block);
    }
}

t_heapHeader *getHeapHeaderFromBlock(t_block *block)
{
    while(block->prev)
    {
        block = block->prev;
    }
    return BLOCK_TO_HEAP(block);
}

void removeHeapIfEmpty(t_heapHeader *heap)
{
    t_block *block = heap->blocks;
    while(block)
    {
        if(!block->isFree)
            return;
        block = block->next;
    }
    // Everything is free
    if(heap->prev)
    {
        heap->prev->next = heap->next;
    }
    if(heap->next)
        heap->next->prev = heap->prev;
    if(!heap->prev)
        g_heapHead = heap->next;
    munmap(heap, heap->totalSize);
}

void free(void *ptr)
{
    if(!ptr || !isValidPtr(ptr))
        return;

    t_block *block = DATA_TO_BLOCK(ptr);
    t_heapHeader *heap = getHeapHeaderFromBlock(block); 
    block->isFree = 1;
    mergeRightBlock(ptr);
    mergeLeftBlock(ptr);
    removeHeapIfEmpty(heap);
}