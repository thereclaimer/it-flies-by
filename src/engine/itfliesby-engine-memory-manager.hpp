#ifndef ITFLIESBY_ENGINE_MEMORY_MANAGER_HPP
#define ITFLIESBY_ENGINE_MEMORY_MANAGER_HPP

#include <itfliesby.hpp>
#include "itfliesby-engine-globals.hpp"

#define ITFLIESBY_ENGINE_MEMORY_MANAGER_MEMORY_SIZE_BYTES ITFLIESBY_MATH_GIGABYTES(2)

struct ItfliesbyEngineMemoryChunkReservation {
    u64    memory_size_bytes;
    u64    memory_arena_offset;
};  

struct ItfliesbyEngineMemoryArena {
    char                                        tag[32];
    u64                                         memory_size_bytes;
    u64                                         reservations_count;
    memory                                      memory;
    ItfliesbyEngineMemoryChunkReservation*      reservations;
    ItfliesbyEngineMemoryArena*                 next;
};


struct ItfliesbyEngineMemoryManager {
    u64                         arena_memory_size_bytes;
    memory                      arena_memory;
    ItfliesbyEngineMemoryArena* arenas;
};


ItfliesbyEngineMemoryManager*
itfliesby_engine_memory_manager_create();

/**
 * This is for reserving a KNOWN chunk that is available
 * this does not check for bounds, we are assuming here
 * that you have already done the check to make sure
 * the reservation is a valid one. Fuck around and find out
 */
memory
itfliesby_engine_memory_arena_chunk_reserve_known();

//---------------------------------------------
// MEMORY MANAGER API
//---------------------------------------------

typedef void* ItfliesbyEngineMemoryArenaHandle;

api ItfliesbyEngineMemoryArenaHandle
itfliesby_engine_memory_arena_push(
    u64  memory_size_bytes,
    char tag[32]);

api u64 
itfliesby_engine_memory_arena_count();

api u64 
itfliesby_engine_memory_arena_size_bytes_total(
    ItfliesbyEngineMemoryArenaHandle arena_handle);

api u64 
itfliesby_engine_memory_arena_size_bytes_available(
    ItfliesbyEngineMemoryArenaHandle arena_handle);

api u64 
itfliesby_engine_memory_arena_size_bytes_used(
    ItfliesbyEngineMemoryArenaHandle arena_handle);

api b8
itfliesby_engine_memory_arena_chunk_can_reserve(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes,
    u64                              reservation_start);

api memory
itfliesby_engine_memory_arena_chunk_reserve(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes,
    u64                              reservation_start);

api memory
itfliesby_engine_memory_arena_chunk_reserve_next(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    u64                              reservation_size_bytes);

api memory
itfliesby_engine_memory_arena_chunk_size(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    memory                           arena_memory_chunk);

api void
itfliesby_engine_memory_arena_chunk_release(
    ItfliesbyEngineMemoryArenaHandle arena_handle,
    memory                           arena_memory_chunk);

#endif //ITFLIESBY_ENGINE_MEMORY_MANAGER_HPP