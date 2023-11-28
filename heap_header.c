#include "./malloc.h"

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

t_heapHeader *getLastHeader(t_heapHeader *header)
{
    while (header && header->next)
    {
        header = header->next;
    }
    return header;
}
