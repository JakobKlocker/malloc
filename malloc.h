#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "./ft_printf.h"

#define TINY_HEAP_SIZE (getpagesize() * 4)
#define MIDDLE_HEAP_SIZE (getpagesize() * 16)
#define TINY_BLOCK_SIZE (TINY_HEAP_SIZE / 128)
#define MIDDLE_BLOCK_SIZE (MIDDLE_HEAP_SIZE / 128)

#define HEAP_TO_BLOCK(heap) ((void*) heap + sizeof(t_heapHeader))
#define BLOCK_TO_HEAP(block) ((void*) block - sizeof(t_heapHeader))
#define BLOCK_TO_DATA(block) ((void*) block + sizeof(t_block))
#define DATA_TO_BLOCK(data) ((void*) data - sizeof(t_block))

typedef enum group{
    TINY,
    MIDDLE,
    LARGE
} t_group;

typedef struct s_block{
    struct s_block *next;
    struct s_block *prev;
    int isFree;
    size_t size;
} t_block;

typedef struct s_heapHeader{
    struct s_heapHeader *next;
    struct s_heapHeader *prev;
    t_block *blocks;
    t_group group;
    size_t totalSize;

} t_heapHeader;

extern t_heapHeader *g_heapHead;

//block.c
void splitBlock(t_block *block, size_t size);
void createNewBlock(size_t size, t_heapHeader *newHeap, t_group group, size_t totalHeapSize);
t_block *findBlock(size_t size, int *perfectFit);

//heap_header.c
void createNewHeapHeader(t_heapHeader *newHeap, t_group group, size_t totalHeapSize);
t_heapHeader *getLastHeader(t_heapHeader *header);

//malloc.c
int getGroupBySize(size_t size);
int getNewHeapSize(size_t size);
void initHeaders(size_t size, t_heapHeader *newHeap, size_t totalHeapSize);
void *createNewHeap(size_t size);
void *malloc(size_t size);

// free.x
int isValidPtr(void *ptr);
void mergeRightBlock(void *ptr);
void mergeLeftBlock(void *ptr);
t_heapHeader *getHeapHeaderFromBlock(t_block *block);
void removeHeapIfEmpty(t_heapHeader *heap);
void free(void *ptr);

//show_alloc.c
void show_alloc_mem();