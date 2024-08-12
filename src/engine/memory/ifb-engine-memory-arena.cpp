#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

external const IFBEngineMemoryArena 
ifb_engine_memory::arena_commit(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl && region_impl->uncommitted_arenas);

    //get the next arena and update the lists
    IFBEngineMemoryArena_Impl* arena = region_impl->uncommitted_arenas;
    arena->previous                         = NULL;
    region_impl->uncommitted_arenas         = arena->next;
    region_impl->committed_arenas->previous = NULL;
    arena->next                             = region_impl->committed_arenas;
    region_impl->committed_arenas           = arena;
    
    //commit the arena
    arena->committed_memory_start = 
        ifb_engine_platform_memory_commit(
            arena->reserved_memory_start,
            arena->size);
    ifb_assert(arena->committed_memory_start);

    //return the arena
    return(arena);
}

external void
ifb_engine_memory::arena_decommit(
    const IFBEngineMemoryArena arena) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(arena_impl && arena_impl->committed_memory_start);

    //decommit the memory
    ifb_engine_platform_memory_decommit(
        arena_impl->committed_memory_start;
        arena_impl->size);

    //reset the arena
    arena_impl->position               = 0;
    arena_impl->committed_memory_start = NULL;

    //update the lists
    IFBEngineMemoryRegion_Impl* region = arena_impl->region;
    region->uncommitted_arenas         = arena_impl->next;
    region->committed_arenas->previous = NULL;
    arena_impl->next                   = region->uncommitted_arenas;
    arena_impl->previous               = NULL;
    region->uncommitted_arenas         = arena_impl;
}

external void
ifb_engine_memory::arena_clear(
    const IFBEngineMemoryArena arena) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(arena_impl && arena_impl->committed_memory_start);

    arena_impl->position = 0;
    memset(arena_impl->committed_memory_start,arena_impl->size);

} 

external const size_t
ifb_engine_memory::arena_space_total(
    const IFBEngineMemoryArena arena) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(arena_impl && arena_impl->committed_memory_start);

    return(arena_impl->size);
}

external const size_t
ifb_engine_memory::arena_space_free(
    const IFBEngineMemoryArena arena) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(arena_impl && arena_impl->committed_memory_start);

    const size_t space_free = arena_impl->size - arena_impl->position;

    return(space_free);
}

external const size_t
ifb_engine_memory::arena_space_occupied(
    const IFBEngineMemoryArena arena) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(arena_impl && arena_impl->committed_memory_start);

    return(arena_impl->position);
}

external const size_t
ifb_engine_memory::arena_page_size(
    const IFBEngineMemoryArena arena) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(arena_impl && arena_impl->committed_memory_start);

    const size_t page_size = ifb_engine_memory::region_page_size(arena_impl->region);

    return(page_size);
}

external const size_t
ifb_engine_memory::arena_page_count(
    const IFBEngineMemoryArena arena) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(arena_impl && arena_impl->committed_memory_start);

    const size_t page_size  = ifb_engine_memory::region_page_size(arena_impl->region);
    const size_t page_count = arena_impl->size / page_size;

    return(page_count);
}


external const memory 
ifb_engine_memory::arena_push_bytes (
    const IFBEngineMemoryArena arena,
    const size_t               size) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(arena_impl && arena_impl->committed_memory_start);
    
    size_t new_position = 
        arena_impl->size + size;
    
    ifb_assert(new_position <= arena_impl->size);

    memory current_memory = 
        arena_impl->committed_memory_start +
        arena_impl->position; 

    arena_impl->position = new_position;

    return(current_memory);
}

external const memory 
ifb_engine_memory::arena_pull_bytes (
    const IFBEngineMemoryArena arena,
    const size_t               size) {

    IFBEngineMemoryArena_Impl* arena_impl = (IFBEngineMemoryArena_Impl*);
    ifb_assert(
        arena_impl && 
        arena_impl->committed_memory_start &&
        arena_impl->position >= size);

    arena_impl->position -= size;
    
    memory current_memory = 
        arena_impl->committed_memory_start +
        arena_impl->size;

    return(current_memory);
}