#include "./malloc.h"

t_heapHeader *g_heapHead;

int main()
{
    void *allocatedMemory1 = malloc(15);
    malloc(15);
    malloc(1555);
    malloc(155555);
    show_alloc_mem();
    return 0;
}
