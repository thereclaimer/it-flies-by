#pragma once

#include "ifb-engine-memory.hpp"

internal u64
ifb_engine_memory_arena_total_size_bytes(
    IFBEngineMemoryArenaPtr arena_ptr) {

    u64 arena_total_size_bytes = 
        sizeof(IFBEngineMemoryArena) +
        arena_ptr->block.memory_size_bytes;

    return(arena_total_size_bytes);
}

internal u64
ifb_engine_memory_arena_total_required_size_bytes(
    u64 arena_size_bytes) {

    u64 required_size_bytes = 
        sizeof(IFBEngineMemoryArena) +
        arena_size_bytes;        

    return(required_size_bytes);
}

internal IFBEngineMemoryArenaPtr
ifb_engine_memory_arena_reserve(
    IFBEngineMemoryArenaAllocatorPtr allocator_ptr) {

    if (!allocator_ptr->arenas.available) {
        return(NULL);
    }

    //get the first of the available arenas and set the next one as the new first available
    IFBEngineMemoryArenaPtr reserved_arena = allocator_ptr->arenas.available;     
    allocator_ptr->arenas.available        = reserved_arena->next;

    //put this new arena at the front of the reserved arenas
    reserved_arena->previous       = NULL;
    reserved_arena->next           = allocator_ptr->arenas.reserved;
    allocator_ptr->arenas.reserved = reserved_arena;

    //reset the reserved arena
    reserved_arena->bytes_used = 0;
    memset(
        reserved_arena->block.memory,
        0,
        allocator_ptr->arenas.arena_size_bytes);

    return(reserved_arena);      

}

internal void
ifb_engine_memory_arena_release(
    IFBEngineMemoryArenaPtr arena_ptr) {

    if (!arena_ptr) {
        return;
    }

    IFBEngineMemoryArenaPtr arena_previous = arena_ptr->previous;
    IFBEngineMemoryArenaPtr arena_next     = arena_ptr->next;

    if (arena_previous) {
        arena_previous->next = arena_next;
    }

    if (arena_next) {
        arena_next->previous = arena_previous;
    }

    IFBEngineMemoryArenaAllocatorPtr allocator_ptr = arena_ptr->allocator;

    arena_ptr->previous             = NULL;
    arena_ptr->next                 = allocator_ptr->arenas.available;
    allocator_ptr->arenas.available = arena_ptr;
}

internal memory
ifb_engine_memory_arena_bytes_push(
    IFBEngineMemoryArenaPtr          arena_ptr,
    u64                              bytes_count) {

    if (!arena_ptr) {
        return(NULL);
    }

    u64 new_size  = arena_ptr->bytes_used + bytes_count;
    u64 max_size  = arena_ptr->block.memory_size_bytes; 

    if (new_size >= max_size) {
        return(NULL);
    }

    ifb_assert(arena_ptr->block.memory);

    memory allocation     = &arena_ptr->block.memory[arena_ptr->bytes_used];
    arena_ptr->bytes_used = new_size;

    return(allocation);

}

internal memory
ifb_engine_memory_arena_bytes_pop(
    IFBEngineMemoryArenaPtr          arena_ptr,
    u64                              bytes_count) {

    if (!arena_ptr) {
        return(NULL);
    }

    ifb_assert(arena_ptr->block.memory);

    if (arena_ptr->bytes_used < bytes_count) {
        arena_ptr->bytes_used = 0;
        return(NULL);
    }

    arena_ptr->bytes_used -= bytes_count;

    memory new_memory =
        arena_ptr->block.memory +
        arena_ptr->bytes_used;
        
    return(new_memory);
}

internal void    
ifb_engine_memory_arena_clear(
    IFBEngineMemoryArenaPtr arena_ptr) {

    if (!arena_ptr) {
        return;
    }

    ifb_assert(arena_ptr);

    memset(
        arena_ptr->block.memory,
        0,
        arena_ptr->block.memory_size_bytes);

    arena_ptr->bytes_used = 0;

}

internal u64
ifb_engine_memory_arena_space_available(
    IFBEngineMemoryArenaPtr arena_ptr) {

    if (!arena_ptr) {
        return(0);
    }    

    u64 max_space       = arena_ptr->bytes_used;
    u64 space_available = max_space - arena_ptr->bytes_used;     

    return(space_available);

}

internal u64
ifb_engine_memory_arena_space_used(
    IFBEngineMemoryArenaPtr arena_ptr) {

    if (!arena_ptr) {
        return(0);
    }

    u64 space_used = arena_ptr->bytes_used;
}