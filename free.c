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

void free(void *ptr)
{
    if(!ptr || !isValidPtr(ptr))
        return;
    t_block *block = DATA_TO_BLOCK(ptr);
    block->isFree = 1;

    while(block->prev && block->prev->isFree)
    {
        block = block->prev;
    }
    //got most left block which is free, all right next to it which are free can merge

}