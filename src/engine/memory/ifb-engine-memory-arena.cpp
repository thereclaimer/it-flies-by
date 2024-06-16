#pragma once

#include "ifb-engine-memory.hpp"

internal IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_recycle(
    IFBEngineMemoryManager_Ptr memory_manager_ptr) {

    ifb_assert(memory_manager_ptr);
    IFBEngineMemoryArena_Ptr arenas_released = memory_manager_ptr->arenas_released;

    if (!arenas_released) {
        return(NULL);
    }

    IFBEngineMemoryArena_Ptr tail_arena = NULL;
    for (
        IFBEngineMemoryArena_Ptr arena = arenas_released;
        arena->next != NULL;
        arena = arena->next) {
        
        tail_arena = arena;
    }

    ifb_assert(tail_arena);

    IFBEngineMemoryArena_Ptr previous_arena_ptr = tail_arena->previous;
    previous_arena_ptr->next = NULL;
    tail_arena->previous     = NULL;

    return(previous_arena_ptr);
}

internal IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_released_tail(
    IFBEngineMemoryManager_Ptr memory_manager_ptr) {

    ifb_assert(memory_manager_ptr);
    IFBEngineMemoryArena_Ptr arenas_released = memory_manager_ptr->arenas_released;

    if (!arenas_released) {
        return(NULL);
    }

    IFBEngineMemoryArena_Ptr tail_arena = NULL;
    for (
        IFBEngineMemoryArena_Ptr arena = arenas_released;
        arena->next != NULL;
        arena = arena->next) {
        
        tail_arena = arena;
    }

    ifb_assert(tail_arena);

    return(tail_arena);
}

internal IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_reserved_tail(
    IFBEngineMemoryManager_Ptr memory_manager_ptr) {
    
    ifb_assert(memory_manager_ptr);
    IFBEngineMemoryArena_Ptr arenas_reserved = memory_manager_ptr->arenas_reserved;

    if (!arenas_reserved) {
        return(NULL);
    }

    IFBEngineMemoryArena_Ptr tail_arena = NULL;
    for (
        IFBEngineMemoryArena_Ptr arena = arenas_reserved;
        arena->next != NULL;
        arena = arena->next) {
        
        tail_arena = arena;
    }

    ifb_assert(tail_arena);

    return(tail_arena);
}


internal void
ifb_engine_memory_arena_reset_new(
          IFBEngineMemoryArena_Ptr   new_arena,
          IFBEngineMemoryManager_Ptr manager_ptr,
    const char                       tag[32]) {

    new_arena->manager_ptr = manager_ptr;

    u64 arena_memory_offset = manager_ptr->arena_size - sizeof(IFBEngineMemoryArena);

    //zero the memory
    memset(
        new_arena->memory,
        0,
        arena_memory_offset
    );

    //reset the used memory count
    new_arena->used_memory = 0;

    //copy the new tag
    strcpy(
        new_arena->tag,
        tag);
}

internal IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_reserve(
          IFBEngineMemoryManager_Ptr memory_manager_ptr,
    const char                       tag[32]) {

    // first, see if we have space available for an arena
    u64 space_available = ifb_engine_memory_manager_space_available(memory_manager_ptr);
    if (space_available < memory_manager_ptr->arena_size) {
        return(NULL);
    }

    // we have space, so get the tail arena if we have one
    IFBEngineMemoryArena_Ptr tail_arena = ifb_engine_memory_arena_reserved_tail(memory_manager_ptr);
    
    if (!tail_arena) {

        // we don't have one, so make our first arena
        memory_manager_ptr->arenas_reserved = (IFBEngineMemoryArena_Ptr)memory_manager_ptr->managed_memory;
        IFBEngineMemoryArena_Ptr first_arena_ptr = memory_manager_ptr->arenas_reserved; 

        ifb_engine_memory_arena_reset_new(
            first_arena_ptr,
            memory_manager_ptr,
            tag);

        return(first_arena_ptr);
    }

    // we do have one, so lets check if we have an arena we can recycle
    IFBEngineMemoryArena_Ptr recycled_arena = 
        ifb_engine_memory_arena_recycle(memory_manager_ptr);

    if (recycled_arena) {

        //we have one, so add that to the tail
        tail_arena->next = recycled_arena;
        recycled_arena->previous = tail_arena;

        ifb_engine_memory_arena_reset_new(
            recycled_arena,
            memory_manager_ptr,
            tag);

        return(recycled_arena);
    }
    
    // with nothing to recycle,we're creating a new arena,
    // so we just add that to the tail
    IFBEngineMemoryArena_Ptr new_arena = (IFBEngineMemoryArena_Ptr)(((memory)tail_arena) + memory_manager_ptr->arena_size);
    tail_arena->next = new_arena;
    new_arena->previous = tail_arena;

    ifb_engine_memory_arena_reset_new(
        new_arena,
        memory_manager_ptr,
        tag);

    return(new_arena);
}

void
ifb_engine_memory_arena_release(
    IFBEngineMemoryArena_Ptr arena_ptr) {

    //make sure we have a valid arena
    if (!arena_ptr) {
        return;
    }

    IFBEngineMemoryManager_Ptr manager_ptr = arena_ptr->manager_ptr;

    //get the arenas adjancent to this one
    IFBEngineMemoryArena_Ptr arena_previous = arena_ptr->previous;
    IFBEngineMemoryArena_Ptr arena_next     = arena_ptr->next;

    //if they aren't null, join them together
    if (arena_previous) {
        arena_previous->next = arena_next;
    }
    
    if (arena_next) {
        arena_next->previous = arena_previous;
    }

    //add this to the list of released arenas
    IFBEngineMemoryArena_Ptr released_tail_arena = 
        ifb_engine_memory_arena_released_tail(manager_ptr);
    
    if (!released_tail_arena) {
        manager_ptr->arenas_released = released_tail_arena;
        return;
    }

    released_tail_arena->next = released_tail_arena;
}

internal memory
ifb_engine_memory_arena_bytes_push(
    IFBEngineMemoryArena_Ptr arena_ptr,
    u64                      size_bytes) {

    if (!arena_ptr || size_bytes == 0) {
        return(NULL);
    }

    IFBEngineMemoryManager_Ptr manager_ptr = arena_ptr->manager_ptr;

    u64 new_arena_used_size = arena_ptr->used_memory + size_bytes; 
    u64 max_arena_memory    = manager_ptr->arena_size - sizeof(IFBEngineMemoryArena);

    if (new_arena_used_size > max_arena_memory) {
        return(NULL);
    }

    memory pushed_memory = arena_ptr->memory + size_bytes;

    return(pushed_memory);
}

internal void
ifb_engine_memory_arena_bytes_pop(
    IFBEngineMemoryArena_Ptr arena_ptr,
    u64                size_bytes) {

    if (!arena_ptr || size_bytes == 0) {
        return;
    }

    if (size_bytes > arena_ptr->used_memory) {
        size_bytes = arena_ptr->used_memory;
    }

    memory new_arena_end = 
        arena_ptr->memory + 
        (arena_ptr->used_memory - size_bytes);

    memset(
        new_arena_end,
        0,
        size_bytes);

    arena_ptr->used_memory -= size_bytes;
}

internal void
ifb_engine_memory_arena_reset(
    IFBEngineMemoryArena_Ptr arena_ptr) {

    if (!arena_ptr) {
        return;
    }

    IFBEngineMemoryManager_Ptr manager_ptr = arena_ptr->manager_ptr;

    memset(
        arena_ptr->memory,
        0,
        manager_ptr->arena_size);

    arena_ptr->used_memory = 0;
}


internal IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_reserve_64kb(
    const char arena_tag[32]) {

    ifb_assert(ifb_engine_memory.platform_memory);

    IFBEngineMemoryArena_Ptr arena_64kb = 
        ifb_engine_memory_arena_reserve(
            &ifb_engine_memory.memory_manager_64kb,
            arena_tag);

    ifb_assert(arena_64kb);

    return(arena_64kb);
}

internal IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_reserve_64mb(
    const char arena_tag[32]) {

    ifb_assert(ifb_engine_memory.platform_memory);

    IFBEngineMemoryArena_Ptr arena_64mb = 
        ifb_engine_memory_arena_reserve(
            &ifb_engine_memory.memory_manager_64mb,
            arena_tag);

    ifb_assert(arena_64mb);

    return(arena_64mb);
}