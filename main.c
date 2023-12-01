#include "./malloc.h"

t_heapHeader *g_heapHead;

int main()
{
    void* test = malloc(15);
    void* test1 = malloc(15);
    void* test2 = malloc(15);
    show_alloc_mem();
    free(test);
    show_alloc_mem();
    test = malloc(15);
    show_alloc_mem();
    void* test3 = malloc(1555555);
    void* test4 = malloc(1555555);
    show_alloc_mem();
    free(test3);
    show_alloc_mem();
    free(test1);
    free(test);
    free(test2);
    free(test4);
    return 0;
}
