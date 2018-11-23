#include "Memory_allocation_service.h"

struct MemoryAllocator{

    void* memory_ptr;
    size_t size_of_memory;
};

MemoryAllocator* MemoryAllocator_init(void* memoryPool, size_t size){

    MemoryAllocator *p_MemoryAllocator = malloc(sizeof(MemoryAllocator));

    if(size % sizeof(size_t) != 0)
        size -= size % sizeof(size_t);

    p_MemoryAllocator-> memory_ptr = memoryPool;
    p_MemoryAllocator-> size_of_memory = size;


    return p_MemoryAllocator;

}

void* MemoryAllocator_allocate(MemoryAllocator* allocator, size_t size){

return NULL;
}

/* Return number of still allocated blocks */
size_t MemoryAllocator_free(MemoryAllocator* allocator, void* ptr){
    return 0;


}

/* Return the size of largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator){

    return 0;

}


