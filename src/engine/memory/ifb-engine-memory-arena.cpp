#pragma once

#include "ifb-engine-memory.hpp"

internal void
ifb_engine::memory_arena_commit(
          IFBEngineMemoryArena&  arena,
          IFBEngineMemoryRegion& region, 
    const u64                    minimum_size) {

    IFBEngineMemoryContext& context = ifb_engine::memory_context_get();

    //align the arena to the size of a page
    u64 arena_size = 
        ifb_engine::memory_alignment_pow_2(
            minimum_size,
            context.page_size);

    //make sure we can fit the arena
    bool can_fit = 
        ifb_engine::stack_can_push(
            region.stack,
            arena_size);

    ifb_assert(can_fit);

    //grab the stack pointer
    memory arena_memory = ifb_engine::stack_pointer(region.stack);

    //update the region stack
    ifb_engine::stack_push(region.stack, arena_size);

    //create the stack for the arena
    ifb_engine::stack_create(
        arena.stack,
        arena_memory,
        arena_size);

    //commit the arena
    memory committed_memory = 
        ifb_engine_platform_memory_commit(
            arena_memory,
            arena_size);

    ifb_assert(committed_memory == arena_memory);

    //put the arena at the front
    arena.next    = region.arenas;
    region.arenas = arena;
}

internal memory
ifb_engine::memory_arena_push_bytes(
          IFBEngineMemoryArena& arena, 
    const u64                   size) {

    //if we can't fit, just return null
    bool can_fit = ifb_engine::stack_can_push(arena.stack,size);
    if (!can_fit) {
        return(NULL);
    }

    //grab the stack pointer
    memory arena_memory = ifb_engine::stack_pointer(arena.stack);

    //update the stack
    ifb_engine::stack_push(arena.stack,size);

    //return the memory
    return(arena_memory);
}

internal memory
ifb_engine::memory_arena_pull_bytes(
          IFBEngineMemoryArena& arena,
    const u64                   size) {

    //if we can't pull, just return null
    bool can_pull = ifb_engine::stack_can_pull(arena.stack,size);
    if (!can_pull) {
        return(NULL);
    }

    //pull the bytes off the stack
    ifb_engine::stack_pull(arena.stack,size);

    //return the stack pointer
    memory arena_memory = ifb_engine::stack_pointer(arena.stack);
    return(arena_memory);
}

internal void
ifb_engine::memory_arena_clear(
    IFBEngineMemoryArena& arena) {

    ifb_engine::stack_clear(arena.stack);
}

internal void
ifb_engine::memory_arena_decommit(
    IFBEngineMemoryArena& arena) {

    //clear the stack
    ifb_engine::stack_clear(arena.stack);

    //grab the stack pointer
    memory arena_memory = ifb_engine::stack_pointer(arena.stack);

    //decommit the memory
    ifb_engine_platform_memory_decommit(
        arena_memory,
        arena.stack.size);
}