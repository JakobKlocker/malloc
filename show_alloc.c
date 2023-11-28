#include "./malloc.h"

void show_alloc_mem()
{
    t_heapHeader *currentHeap = g_heapHead;

    while (currentHeap)
    {
        if (currentHeap->group == TINY)
            printf("TINY: %p\n", (void *)currentHeap);
        else if (currentHeap->group == MIDDLE)
            printf("SMALL: %p\n", (void *)currentHeap);
        else
            printf("LARGE: %p\n", (void *)currentHeap);
        t_block *currentBlock = currentHeap->blocks;
        while (currentBlock)
        {
            if (!currentBlock->isFree)
            {
                char *tmp = (char *)currentBlock;
                printf("%p - %p : %i bytes\n", (void *)tmp, (void *)(tmp + currentBlock->size), currentBlock->size);
            }
            currentBlock = currentBlock->next;
        }
        currentHeap = currentHeap->next;
    }
}