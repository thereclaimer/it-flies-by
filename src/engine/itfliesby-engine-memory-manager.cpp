#pragma once

#include "itfliesby-engine-memory-manager.hpp"
#include "itfliesby-engine-globals.hpp"

ItfliesbyEngineMemoryManager* memory_manager;

internal ItfliesbyEngineMemoryManager*
itfliesby_engine_memory_manager_create() {

    memory platform_memory = platform_api.memory_allocate(ITFLIESBY_ENGINE_MEMORY_MANAGER_MEMORY_SIZE_BYTES);

    ITFLIESBY_ASSERT(platform_memory);

    memory_manager = (ItfliesbyEngineMemoryManager*)platform_memory;

    memory_manager->arena_memory_size_bytes = ITFLIESBY_ENGINE_MEMORY_MANAGER_MEMORY_SIZE_BYTES;
    memory_manager->arena_memory            = platform_memory += sizeof(ItfliesbyEngineMemoryManager);

    return(memory_manager);
}

external ItfliesbyEngineMemoryArenaHandle
itfliesby_engine_memory_arena_push(
    u64  memory_size_bytes,
    char tag[32]) {

    ItfliesbyEngineMemoryArena* memory_manager_arenas     = memory_manager->arenas; 
    ItfliesbyEngineMemoryArena* new_arena                 = NULL;
    memory                      new_arena_reserved_memory = NULL;
    
    if (memory_manager_arenas == NULL) {

        //get our base arena memory
        new_arena_reserved_memory = memory_manager->arena_memory;
        ITFLIESBY_ASSERT(new_arena_reserved_memory);

        //make the new arena the first in the list
        memory_manager->arenas = (ItfliesbyEngineMemoryArena*)new_arena_reserved_memory;
        new_arena = memory_manager->arenas;
    } 
    else {

        ItfliesbyEngineMemoryArena* previous_arena = NULL;

        for (
            previous_arena = memory_manager_arenas;
            previous_arena->next != NULL;
            previous_arena = previous_arena->next);

        ITFLIESBY_ASSERT(previous_arena);

        //set the new arena
        new_arena_reserved_memory = previous_arena->memory + previous_arena->memory_size_bytes;
        new_arena = (ItfliesbyEngineMemoryArena*)new_arena_reserved_memory;

        //add it to the list
        previous_arena->next = new_arena;
    }
    
    //we should have a new arena at this point
    ITFLIESBY_ASSERT(new_arena);

    //the reservations table is at the very end of the arena and grows downward towards the beginning
    memory reservations_memory = new_arena_reserved_memory + sizeof(ItfliesbyEngineMemoryArena) + (memory_size_bytes - 1); 
    ItfliesbyEngineMemoryArenaReservationTable* reservations_table = 
        (ItfliesbyEngineMemoryArenaReservationTable*)reservations_memory;
    reservations_table->count_reservations             = 0;
    reservations_table->reservation_arena_offset_bytes = NULL;
    reservations_table->reservation_size_bytes         = NULL;

    //set our arena info
    new_arena->memory             = new_arena_reserved_memory + sizeof(ItfliesbyEngineMemoryArena);
    new_arena->memory_size_bytes  = memory_size_bytes;
    new_arena->next               = NULL;
    new_arena->reservations       = reservations_table; 
    strcpy(
        new_arena->tag,
        tag);

    return(new_arena);
}

external u64 
itfliesby_engine_memory_arena_count(
    ItfliesbyEngineMemoryArenaHandle arena_handle) {

    return(0);
}

external u64 
itfliesby_engine_memory_arena_size_bytes_total(
    ItfliesbyEngineMemoryArenaHandle arena_handle) {

    return(0);
}

external u64 
itfliesby_engine_memory_arena_size_bytes_available(
    ItfliesbyEngineMemoryArenaHandle arena_handle) {

    return(0);
}

external u64 
itfliesby_engine_memory_arena_size_bytes_used(
    ItfliesbyEngineMemoryArenaHandle arena_handle) {

    return(0);
}

external b8
itfliesby_engine_memory_arena_chunk_can_reserve(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes,
    u64                              reservation_offset_bytes) {

    return(false);
}

external memory
itfliesby_engine_memory_arena_chunk_reserve(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes,
    u64                              reservation_offset_bytes) {

    return(NULL);
}

external memory
itfliesby_engine_memory_arena_chunk_reserve_next(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes) {

    return(NULL);
}

external memory
itfliesby_engine_memory_arena_chunk_size(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    memory                           arena_memory_chunk) {

    return(NULL);
}

external void
itfliesby_engine_memory_arena_chunk_release(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    memory                           arena_memory_chunk) {

}
