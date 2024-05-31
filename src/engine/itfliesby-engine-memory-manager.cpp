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
    //set our arena info
    new_arena->memory             = new_arena_reserved_memory + sizeof(ItfliesbyEngineMemoryArena);
    new_arena->memory_size_bytes  = memory_size_bytes;
    new_arena->next               = NULL;
    new_arena->reservations       = NULL;
    new_arena->reservations_count = 0; 
    strcpy(
        new_arena->tag,
        tag);

    return(new_arena);
}

external u64 
itfliesby_engine_memory_arena_count() {

    if (!memory_manager) {
        return(0);
    }

    u64 arena_count = 0;

    for (
        ItfliesbyEngineMemoryArena* arena = memory_manager->arenas;
        arena != NULL && arena->next != NULL;
        arena = arena->next) {

        ++arena_count;
    }

    return(arena_count);
}

external u64 
itfliesby_engine_memory_arena_size_bytes_total(
    ItfliesbyEngineMemoryArenaHandle arena_handle) {

    ItfliesbyEngineMemoryArena* arena = (ItfliesbyEngineMemoryArena*)arena_handle;

    if (!arena) {
        return(0);
    }

    return(arena->memory_size_bytes);
}

external u64 
itfliesby_engine_memory_arena_size_bytes_available(
    ItfliesbyEngineMemoryArenaHandle arena_handle) {

    ItfliesbyEngineMemoryArena* arena = (ItfliesbyEngineMemoryArena*)arena_handle;

    if (!arena) {
        return(0);
    }

    u64 reservations_count = arena->reservations_count;

    ItfliesbyEngineMemoryChunkReservation* reservations = arena->reservations;
    if (!reservations || reservations_count == 0) {
        return(0);
    }

    u64 bytes_used = 0;
    ItfliesbyEngineMemoryChunkReservation i_reservation;

    for (
        u64 index = 0;
        index < reservations_count;
        ++index) {

        i_reservation = reservations[index];
        bytes_used += i_reservation.memory_size_bytes;
    }

    u64 bytes_available = arena->memory_size_bytes - bytes_used;

    return(bytes_available);
}

external u64 
itfliesby_engine_memory_arena_size_bytes_used(
    ItfliesbyEngineMemoryArenaHandle arena_handle) {

    ItfliesbyEngineMemoryArena* arena = (ItfliesbyEngineMemoryArena*)arena_handle;

    if (!arena) {
        return(0);
    }

    u64 reservations_count = arena->reservations_count;

    ItfliesbyEngineMemoryChunkReservation* reservations = arena->reservations;
    if (!reservations || reservations_count == 0) {
        return(0);
    }

    u64 bytes_used = 0;
    ItfliesbyEngineMemoryChunkReservation i_reservation;

    for (
        u64 index = 0;
        index < reservations_count;
        ++index) {

        i_reservation = reservations[index];
        bytes_used += i_reservation.memory_size_bytes;
    }

    return(bytes_used);
}

external b8
itfliesby_engine_memory_arena_chunk_can_reserve(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes,
    u64                              reservation_start) {

    ItfliesbyEngineMemoryArena* arena_ptr = (ItfliesbyEngineMemoryArena*)arena_handle;

    if (!arena_ptr) {
        return(false);
    }

    //stuff we'll need to check
    ItfliesbyEngineMemoryArena arena = *arena_ptr;
    ItfliesbyEngineMemoryChunkReservation* reservations = arena.reservations;

    u64 reservations_count   = arena.reservations_count;
    u64 bytes_used           = 0;
    u64 i_reservation_ending = 0;
    u64 reservation_ending   = reservation_size_bytes + reservation_start;

    //sanity check, the reservation shouldnt be bigger than the arena
    if (reservation_size_bytes >= arena.memory_size_bytes) {
        return(false);
    }

    //we need to make sure we aren't going to collide with the reservation table
    //and we need to account for what the table will look like with this new reservation
    u64 new_reservation_table       = sizeof(ItfliesbyEngineMemoryChunkReservation) * (reservations_count + 1);
    u64 new_reservation_table_start = arena.memory_size_bytes - new_reservation_table;
    if (reservation_ending >= new_reservation_table_start) {
        return(false);
    }

    //in this loop, we are adding up the used bytes and making
    //sure that the memory is available
    ItfliesbyEngineMemoryChunkReservation i_reservation;

    for (
        u64 index = 0;
        index < reservations_count;
        ++index) {

        i_reservation        = reservations[index];
        i_reservation_ending = i_reservation.memory_arena_offset + i_reservation.memory_size_bytes;

        //add the used bytes
        bytes_used += i_reservation.memory_size_bytes;

        //check the range of the existing reservation
        if (
            (reservation_start  >= i_reservation.memory_arena_offset) &&
            (reservation_ending <  i_reservation_ending)) {

            //this reservation collides with another, cannot reserve
            return(false);
        }
    }

    //if the reservation can fit, we're done
    u64 bytes_available = arena.memory_size_bytes - bytes_used;
    if (reservation_size_bytes > bytes_available) {
        return(false);
    }

    return(true);
}

external memory
itfliesby_engine_memory_arena_chunk_reserve(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes,
    u64                              reservation_start) {


    ItfliesbyEngineMemoryArena* arena_ptr = (ItfliesbyEngineMemoryArena*)arena_handle;

    if (!arena_ptr) {
        return(NULL);
    }

    //stuff we'll need to check
    ItfliesbyEngineMemoryArena arena = *arena_ptr;
    ItfliesbyEngineMemoryChunkReservation* reservations = arena.reservations;

    u64 reservations_count    = arena.reservations_count;
    u64 bytes_used            = 0;
    u64 i_reservation_ending  = 0;
    u64 reservation_ending    = reservation_size_bytes + reservation_start;

    //sanity check, the reservation shouldnt be bigger than the arena
    if (reservation_size_bytes >= arena.memory_size_bytes) {
        return(NULL);
    }

    //we need to make sure we aren't going to collide with the reservation table
    //and we need to account for what the table will look like with this new reservation
    u64 new_reservation_table       = sizeof(ItfliesbyEngineMemoryChunkReservation) * (reservations_count + 1);
    u64 new_reservation_table_start = arena.memory_size_bytes - new_reservation_table;
    if (reservation_ending >= new_reservation_table_start) {
        return(NULL);
    }

    //in this loop, we are adding up the used bytes and making
    //sure that the memory is available
    ItfliesbyEngineMemoryChunkReservation i_reservation;

    for (
        u64 index = 0;
        index < reservations_count;
        ++index) {

        i_reservation        = reservations[index];
        i_reservation_ending = i_reservation.memory_arena_offset + i_reservation.memory_size_bytes;

        //add the used bytes
        bytes_used += i_reservation.memory_size_bytes;

        //check the range of the existing reservation
        if (
            (reservation_start  >= i_reservation.memory_arena_offset) &&
            (reservation_ending <  i_reservation_ending)) {

            //this reservation collides with another, cannot reserve
            return(NULL);
        }
    }

    //if the reservation can fit, we're done
    u64 bytes_available = arena.memory_size_bytes - bytes_used;
    if (reservation_size_bytes > bytes_available) {
        return(NULL);
    }

    //we're good, so we need to set the new memory
    //first, we'll adjust the reservation table
    memory arena_reservations_start = arena.memory + new_reservation_table_start;
    arena.reservations = (ItfliesbyEngineMemoryChunkReservation*)arena_reservations_start;
    ++arena.reservations_count;

    //make the new reservation
    ItfliesbyEngineMemoryChunkReservation new_reservation = {0};
    new_reservation.memory_arena_offset = reservation_start;
    new_reservation.memory_size_bytes   = reservation_size_bytes;

    //the reservation table grows downward, so the new entry is always at index 0
    arena.reservations[0] = new_reservation;

    //now we can zero and return the new memory
    memory new_memory_chunk = arena.memory + new_reservation.memory_arena_offset; 
    ITFLIESBY_ASSERT(new_memory_chunk);

    memset(
        new_memory_chunk,
        0,
        new_reservation.memory_size_bytes);

    //write the arena data back
    *arena_ptr = arena;

    return(new_memory_chunk);
}

external memory
itfliesby_engine_memory_arena_chunk_reserve_next(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes) {

    ItfliesbyEngineMemoryArena* arena_ptr = (ItfliesbyEngineMemoryArena*)arena_handle;

    if (!arena_ptr) {
        return(NULL);
    }

    ItfliesbyEngineMemoryArena arena = *arena_ptr;

    //couple of sanity checks, make sure this isn't a stupid allocation
    memory arena_memory_end               = arena.memory + arena.memory_size_bytes - 1;
    u64    new_reservation_table_size     = sizeof(ItfliesbyEngineMemoryChunkReservation) * (arena.reservations_count + 1);
    memory new_reservation_table_start    = arena_memory_end - new_reservation_table_size;  
    u64    new_reservation_max_size_bytes = arena.memory_size_bytes - new_reservation_table_size; 

    if (reservation_size_bytes > new_reservation_max_size_bytes) {
        return(NULL);
    }



    //what we need to do here is check in between all of our existing reservations
    //to get the first available slot that can fit our new reservation

    ItfliesbyEngineMemoryChunkReservation i_reservation;
    u64                                   i_previous_reservation_ending;





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
