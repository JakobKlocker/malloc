#include "./malloc.h"

void show_alloc_mem()
{
    t_heapHeader *currentHeap = g_heapHead;
    ft_printf("heapHeader size: %li\n", sizeof(t_heapHeader));
    ft_printf("block size: %li\n", sizeof(t_block));
    while (currentHeap)
    {
        if (currentHeap->group == TINY)
            ft_printf("TINY: %p\n", (void *)currentHeap);
        else if (currentHeap->group == MIDDLE)
            ft_printf("SMALL: %p\n", (void *)currentHeap);
        else
            ft_printf("LARGE: %p\n", (void *)currentHeap);
        t_block *currentBlock = currentHeap->blocks;
        while (currentBlock)
        {
            if (!currentBlock->isFree)
            {
                char *tmp = (char *)BLOCK_TO_DATA(currentBlock);
                ft_printf("%p - %p : %li bytes\n", (void *)tmp, (void *)(tmp + currentBlock->size), currentBlock->size);
            }
            currentBlock = currentBlock->next;
        }
        currentHeap = currentHeap->next;
    }
}
