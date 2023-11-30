#include "./malloc.h"

t_heapHeader *g_heapHead;

int main()
{
    void* test = malloc(15);
    void* test1 = malloc(15);
    void* test2 = malloc(15);
    show_alloc_mem();
    free(test1);
    show_alloc_mem();
    return 0;
}
