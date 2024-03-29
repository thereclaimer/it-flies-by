#pragma once

#include "itfliesby-memory.hpp"

external ItfliesbyMemoryArena*
itfliesby_memory_arena_create(
    char                       arena_tag[32],
    u64                        arena_size,
    memory                     arena_memory,
    ItfliesbyMemoryReturnCode* result) {
    
    if (result) *result = ITFLIESBY_MEMORY_RETURN_CODE_SUCCESS; 

    //we need to make sure there's enough memory for a basic scratch allocator
    if (!arena_memory) {
        if (result) *result = ITFLIESBY_MEMORY_RETURN_CODE_CORE_MEMORY_NULL;
        return(NULL);
    }
    if (arena_size < ITFLIESBY_MEMORY_ARENA_MINIMUM_SIZE) {
        if (result) *result = ITFLIESBY_MEMORY_RETURN_CODE_NOT_ENOUGH_ARENA_MEMORY;
        return(NULL);
    }

    ItfliesbyMemoryArena* arena = (ItfliesbyMemoryArena*)arena_memory;
    arena->partitions = NULL;
    arena->size       = arena_size - sizeof(ItfliesbyMemoryArena);
    strcpy(arena->tag,arena_tag);

    return(arena);
}

external void
itfliesby_memory_arena_destroy(
    ItfliesbyMemoryArena* arena) {

    arena = NULL;
}

external u64
itfliesby_memory_arena_size_total(
    ItfliesbyMemoryArena* arena) {

    if (!arena) {
        return(0);
    }

    return(arena->size);
}

external u64
itfliesby_memory_arena_size_free(
    ItfliesbyMemoryArena* arena) {

    if (!arena) {
        return(0);
    }

    u64 size_free = arena->size;

    for (
        ItfliesbyMemoryPartition* partition = arena->partitions;
        partition != NULL;
        partition = partition->next) {

        size_free -= partition->size;
    }

    return(size_free);
}

external u64
itfliesby_memory_arena_size_occupied(
    ItfliesbyMemoryArena* arena) {

    if (!arena) {
        return(0);
    }

    u64 size_occupied = 0;

    for (
        ItfliesbyMemoryPartition* partition = arena->partitions;
        partition != NULL;
        partition = partition->next) {

        size_occupied += partition->size;
    }

    return(size_occupied);
}