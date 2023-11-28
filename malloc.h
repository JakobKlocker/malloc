#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

typedef enum group{
    TINY,
    MIDDLE,
    LARGE
} t_group;

typedef struct s_block{
    struct s_block *next;
    int isFree;
    int size;
} t_block;

typedef struct s_heapHeader{
    struct s_heapHeader *next;
    struct s_heapHeader *prev;
    t_block *blocks;
    t_group group;
    size_t totalSize;

} t_heapHeader;


extern t_heapHeader *g_heapHead;