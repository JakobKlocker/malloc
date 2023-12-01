#include "./malloc.h"

size_t getSizeFromBlock(void *ptr)
{
    t_block *block = DATA_TO_BLOCK(ptr);
    return block->size;
}

void *realloc(void *ptr, size_t size)
{
    write(1, "Own realc Called\n", 18);

    void *ret;

    if (!ptr)
        ret = malloc(size);
    else if (ptr && size == 0)
    {
        free(ptr);
        ret = NULL;
    }
    else if (!isValidPtr(ptr))
        ret = NULL;
    else
    {
        size_t oldSize = getSizeFromBlock(ptr);
        ret = malloc(size);
        if (ret)
        {
            size_t copySize = (size <= oldSize) ? size : oldSize;
            memmove(ret, ptr, copySize); // FIX LINKER PROBLEM AND CHANGE TO FT_MEMMOVE!!!
            free(ptr);
        }
    }
    return ret;
}
