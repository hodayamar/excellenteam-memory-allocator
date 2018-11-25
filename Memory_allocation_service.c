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
    size_t *end_of_allocator = (size_t*)next_block + (allocator->size_of_memory/sizeof(size_t));

    while(pow_of_two < size)
        pow_of_two *= 2;

    size = pow_of_two;

    while(next_block != end_of_allocator)
    {

        if(*((size_t*)next_block) >= size)
        {
            *(((size_t*)next_block) + size + 1 ) = *((size_t*)next_block) - size;

            *((size_t*)next_block) = size + 1;

            return next_block;
        }

        else if((*((size_t*)next_block + *((size_t*)next_block) + 1) & 0) == 0)
        {
            *((size_t*)next_block) = *((size_t*)next_block) + *((size_t*)next_block + *((size_t*)next_block) + 1);

            index = *((size_t*)next_block);
        }

        next_block = index + (size_t*)next_block;
    }
    return NULL;
}

/* Return number of still allocated blocks */
size_t MemoryAllocator_free(MemoryAllocator* allocator, void* ptr){

    void* next_block = allocator->memory_ptr;
    size_t index;
    size_t still_allocated_blocks = 0;
    size_t *end_of_allocator = (size_t*)next_block + (allocator->size_of_memory/sizeof(size_t));


    while(next_block != end_of_allocator)
    {
        if(*((size_t*)next_block) & 1)
            still_allocated_blocks += 1;

        index = *((size_t*)next_block);

        next_block = index + (size_t*)next_block;

    }

    return still_allocated_blocks;

}

/* Return the size of largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator){

    void* next_block = allocator->memory_ptr;
    size_t index;
    size_t largest_free_block = *((size_t*)next_block);
    size_t *end_of_allocator = (size_t*)next_block + (allocator->size_of_memory/sizeof(size_t));

    MemoryAllocator_allocate(allocator, allocator->size_of_memory);

    while(next_block != end_of_allocator)
    {
        index = *((size_t*)next_block);

        if(index >= largest_free_block)
            largest_free_block = *((size_t*)next_block);

        next_block = index + (size_t*)next_block;

    }

    return largest_free_block;
}


