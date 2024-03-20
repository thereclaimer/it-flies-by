#pragma once

#include "itfliesby-memory.hpp"

external ItfliesbyMemoryPartition* 
itfliesby_memory_partition_create(
    ItfliesbyMemoryArena*      arena,
    char                       partition_tag[32],
    u64                        partition_size,
    ItfliesbyMemoryReturnCode* result) {

    ItfliesbyMemoryReturnCode result_local = ITFLIESBY_MEMORY_RETURN_CODE_SUCCESS; 

    u64 arena_space_available = itfliesby_memory_arena_size_free(arena);

    //check arguments
    if (!arena) {
        result_local = ITFLIESBY_MEMORY_RETURN_CODE_INVALID_ARGUMENT;
        return(NULL);
    }
    if ((sizeof(ItfliesbyMemoryPartition) + partition_size) > arena_space_available) {
        result_local = ITFLIESBY_MEMORY_RETURN_CODE_NOT_ENOUGH_ARENA_MEMORY;
        return(NULL);
    }

    //if this is the first partition, set it and we're done
    if (!arena->partitions) {
        
        ItfliesbyMemoryPartition* partition = (ItfliesbyMemoryPartition*)(arena + sizeof(ItfliesbyMemoryArena));
        partition->allocators = NULL;
        partition->arena      = arena;
        partition->next       = NULL;
        partition->size       = partition_size;
        strcpy(partition->tag,partition_tag);

        arena->partitions = partition;

        if (result) {
            *result = result_local; 
        }

        return(partition);
    }
    
    //go to the end of the partitions
    ItfliesbyMemoryPartition* current_partition = arena->partitions;
    for (
        current_partition;
        current_partition->next != NULL;
        current_partition = current_partition->next) {
    }

    //put the new partition at the end of the list
    ItfliesbyMemoryPartition* partition = (ItfliesbyMemoryPartition*)(current_partition + sizeof(ItfliesbyMemoryPartition) + current_partition->size);
    *partition = {0};

    partition->allocators = NULL;
    partition->arena      = arena;
    partition->next       = NULL;
    partition->size       = partition_size;

    current_partition->next = partition;

    return(partition);
}

external memory
itfliesby_memory_partition_raw_memory(
    ItfliesbyMemoryPartition* partition) {

    if (!partition) {
        return(NULL);
    }

    memory partition_memory = (memory)partition + sizeof(ItfliesbyMemoryPartition);
        
    return(partition_memory);
}

external u64
itfliesby_memory_partition_space_total(
    ItfliesbyMemoryPartition* partition) {

    if (!partition) {
        return(0L);
    }

    return(partition->size);
}

external u64
itfliesby_memory_partition_space_free(
    ItfliesbyMemoryPartition* partition) {

    if (!partition) {
        return(0L);
    }

    u64 space_free = partition->size;

    for (
        ItfliesbyMemoryAllocatorHeader* current_allocator_header = partition->allocators;
        current_allocator_header != NULL && current_allocator_header->next != NULL;
        current_allocator_header = current_allocator_header->next) {
        
        space_free -= sizeof(ItfliesbyMemoryAllocatorHeader) + current_allocator_header->size; 
    }

    return(space_free);
}

external u64
itfliesby_memory_partition_space_occupied(
    ItfliesbyMemoryPartition* partition) {

    if (!partition || !partition->allocators) {
        return(0L);
    }

    u64 space_occupied = 0;

    for (
        ItfliesbyMemoryAllocatorHeader* current_allocator_header = partition->allocators;
        current_allocator_header->next != NULL;
        current_allocator_header = current_allocator_header->next) {
        
        space_occupied += (sizeof(ItfliesbyMemoryAllocatorHeader) + current_allocator_header->size);
    }

    return(space_occupied);
}