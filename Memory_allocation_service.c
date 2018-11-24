#include "Memory_allocation_service.h"

#define AVAILABLE 0
#define UNAVAILABLE 1

struct MemoryAllocator
{

    void* memory_ptr;
    size_t size_of_memory;
};

MemoryAllocator* MemoryAllocator_init(void* memoryPool, size_t size)
{

    MemoryAllocator *p_MemoryAllocator = malloc(sizeof(MemoryAllocator));

    if(size % sizeof(size_t) != 0)
        size -= size % sizeof(size_t);

    p_MemoryAllocator-> memory_ptr = memoryPool;
    p_MemoryAllocator-> size_of_memory = size;
    *((size_t*)p_MemoryAllocator->memory_ptr) = p_MemoryAllocator->size_of_memory;
    *((size_t*)p_MemoryAllocator->memory_ptr)|= 1 << 0;
    printf("%lu", *((size_t*)p_MemoryAllocator->memory_ptr));
    return p_MemoryAllocator;
}

void* MemoryAllocator_allocate(MemoryAllocator* allocator, size_t size)
{

    void* next_block = allocator->memory_ptr;
    size_t index;
    size_t pow_of_two = 1;
    size_t not_located = 1;

    while(pow_of_two < size)
        pow_of_two *= 2;

    size = pow_of_two;

    while(not_located)
    {

        if(*((size_t*)next_block) & 1)
        {
            index = *((size_t*)next_block);

        }

        else if(*((size_t*)next_block) >= size)
        {
            *((size_t*)allocator->memory_ptr + (size_t)((size_t*)next_block) + size + 1 - (size_t)(size_t*)allocator->memory_ptr) =
                    *((size_t*)next_block) - size;

            *((size_t*)allocator->memory_ptr + (size_t)((size_t*)next_block) - (size_t)(size_t*)allocator->memory_ptr ) = size + 1;

            return next_block;
        }

        else if((*((size_t*)next_block + *((size_t*)next_block) + 1) & 0) == 0)
        {
            *((size_t*)allocator->memory_ptr + (size_t)((size_t*)next_block - (size_t)(size_t*)allocator->memory_ptr )) =
                    *((size_t*)next_block) + *((size_t*)next_block + *((size_t*)next_block) + 1);

            index = *((size_t*)next_block);
        }

        if(((size_t)((index + (size_t*)next_block) - (size_t)(size_t*)allocator->memory_ptr)) > allocator->size_of_memory)
            not_located = 0;
            return NULL;

        next_block = index + (size_t*)next_block;
    }

}

/* Return number of still allocated blocks */
size_t MemoryAllocator_free(MemoryAllocator* allocator, void* ptr){

    size_t end_of_array = 1;
    void* next_block = allocator->memory_ptr;
    size_t index;
    size_t still_allocated_blocks = 0;

    while(end_of_array)
    {
        if(*((size_t*)next_block) & 1)
            still_allocated_blocks += 1;

        index = *((size_t*)next_block);
        if((size_t*)allocator->memory_ptr - (index + (size_t*)next_block) <= allocator->size_of_memory)
            next_block = index + (size_t*)next_block;
        else
            end_of_array = 0;
    }

    return still_allocated_blocks;

}

/* Return the size of largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator){

    size_t end_of_array = 1;
    void* next_block = allocator->memory_ptr;
    size_t index;
    size_t largest_free_block = *((size_t*)next_block);

    MemoryAllocator_allocate(allocator, allocator->size_of_memory);

    while(end_of_array)
    {
        index = *((size_t*)next_block);

        if(index >= largest_free_block)
            largest_free_block = *((size_t*)next_block);

        if((size_t*)allocator->memory_ptr - (index + (size_t*)next_block) <= allocator->size_of_memory)
            next_block = index + (size_t*)next_block;
        else
            end_of_array = 0;

    }

    return largest_free_block;
}


