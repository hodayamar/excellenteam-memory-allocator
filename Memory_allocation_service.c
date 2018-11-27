#include "Memory_allocation_service.h"

#define AVAILABLE 0
#define MANAGER_SIZE sizeof(size_t)
#define UNAVAILABLE 1

struct MemoryAllocator
{

    void* memory_ptr;
    size_t size_of_memory;
};

MemoryAllocator* MemoryAllocator_init(void* memoryPool, size_t size)
{


    MemoryAllocator *p_MemoryAllocator = malloc(sizeof(MemoryAllocator));

    assert(memoryPool != NULL);

    if(size % sizeof(size_t) != 0)
        size -= size % sizeof(size_t);

    p_MemoryAllocator-> memory_ptr = memoryPool;
    p_MemoryAllocator-> size_of_memory = size;
    *((size_t*)p_MemoryAllocator->memory_ptr) = p_MemoryAllocator->size_of_memory - MANAGER_SIZE;

    return p_MemoryAllocator;
}

void* MemoryAllocator_allocate(MemoryAllocator* allocator, size_t size)
{


    /*Iterate on block managers*/
    void* current_block = allocator->memory_ptr;

    /*Size of current block*/
    size_t index;

    size_t *end_of_allocator = (size_t*)current_block + (allocator->size_of_memory/sizeof(size_t));

    assert(allocator->memory_ptr == NULL);

    /*Align size to size_of(size_t)*/
    while(size++ % sizeof(size_t)){}

    while(current_block != end_of_allocator)
    {
        /*If the current block is available*/
        if((*((size_t*)current_block) & AVAILABLE) == AVAILABLE)
        {
            /*If the current block's size fits*/
            if((*((size_t*)current_block) - AVAILABLE >= size))
            {
                /*If the current block's size fits*/
                if((*((size_t*)current_block) - AVAILABLE > size))
                    *(((size_t*)current_block) + size + MANAGER_SIZE) = *((size_t*)current_block) - size - MANAGER_SIZE;

                *((size_t*)current_block) = size + UNAVAILABLE;

                return current_block;
            }

            /*If the next block is available*/
            else if((*((size_t*)current_block + *((size_t*)current_block) + MANAGER_SIZE) & AVAILABLE) == AVAILABLE)

                *((size_t*)current_block) += *((size_t*)current_block + *((size_t*)current_block) + MANAGER_SIZE) - AVAILABLE;
        }

        else
        {
                /*Move to th next block*/
                index = *((size_t*)current_block) + MANAGER_SIZE;
                current_block = index +(size_t*)current_block;
        }

    }

    return NULL;
}

/* Return number of still allocated blocks */
size_t MemoryAllocator_free(MemoryAllocator* allocator, void* ptr){


    void* current_block = allocator->memory_ptr;
    size_t index;
    size_t still_allocated_blocks = 0;
    size_t *end_of_allocator = (size_t*)current_block + allocator->size_of_memory;

    assert(allocator->memory_ptr == NULL);

    *(size_t*)ptr -= UNAVAILABLE;

    while(current_block != end_of_allocator)
    {
        if(*((size_t*)current_block) & UNAVAILABLE)
            still_allocated_blocks += 1;

        index = *((size_t*)current_block) + MANAGER_SIZE;
        current_block = index + (size_t*)current_block;
    }

    return still_allocated_blocks;

}

/* Return the size of largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator){

    void* current_block = allocator->memory_ptr;
    size_t index;
    size_t largest_free_block = *((size_t*)current_block);
    size_t *end_of_allocator = (size_t*)current_block + allocator->size_of_memory;

    assert(allocator->memory_ptr == NULL);

    MemoryAllocator_allocate(allocator, allocator->size_of_memory);

    while(current_block != end_of_allocator)
    {

        if(((*((size_t*)current_block)) > largest_free_block) && ((*((size_t*)current_block) & AVAILABLE) == AVAILABLE))
            largest_free_block = *((size_t*)current_block);

        index = *((size_t*)current_block) + MANAGER_SIZE;
        current_block = index + (size_t*)current_block;
    }

    return largest_free_block;
}

void MemoryAllocator_final_free(MemoryAllocator* allocator)
{
    free(allocator->memory_ptr);
    free(allocator);
}



