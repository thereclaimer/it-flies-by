#pragma once

#include "ifb-memory.hpp"

internal IFBMemoryArena_Ptr
ifb_memory_arena_recycle() {

    if (!_memory_manager.arenas_released) {
        return(NULL);
    }

    IFBMemoryArena_Ptr tail_arena = NULL;
    for (
        IFBMemoryArena_Ptr arena = _memory_manager.arenas_reserved;
        arena->next != NULL;
        arena = arena->next) {
        
        tail_arena = arena;
    }

    ITFLIESBY_ASSERT(tail_arena);

    IFBMemoryArena_Ptr previous_arena_ptr = tail_arena->previous;
    previous_arena_ptr->next = NULL;
    tail_arena->previous     = NULL;

    return(previous_arena_ptr);
}

internal IFBMemoryArena_Ptr
ifb_memory_arena_released_tail() {

    if (!_memory_manager.arenas_released) {
        return(NULL);
    }

    IFBMemoryArena_Ptr tail_arena = NULL;
    for (
        IFBMemoryArena_Ptr arena = _memory_manager.arenas_reserved;
        arena->next != NULL;
        arena = arena->next) {
        
        tail_arena = arena;
    }

    ITFLIESBY_ASSERT(tail_arena);

    return(tail_arena);
}

internal IFBMemoryArena_Ptr
ifb_memory_arena_reserved_tail() {
    
    if (!_memory_manager.arenas_reserved) {
        return(NULL);
    }

    IFBMemoryArena_Ptr tail_arena = NULL;
    for (
        IFBMemoryArena_Ptr arena = _memory_manager.arenas_reserved;
        arena->next != NULL;
        arena = arena->next) {
        
        tail_arena = arena;
    }

    ITFLIESBY_ASSERT(tail_arena);

    return(tail_arena);
}


internal void
ifb_memory_arena_reset_new(
          IFBMemoryArena_Ptr new_arena,
    const char               tag[32]) {

    u64 arena_memory_offset = IFB_MEMORY_ARENA_SIZE - sizeof(IFBMemoryArena);

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

internal IFBMemoryArena_Ptr
ifb_memory_arena_reserve(
    const char tag[32]) {

    // first, see if we have space available for an arena
    u64 space_available = ifb_memory_manager_space_available()
    if (space_available < IFB_MEMORY_ARENA_SIZE) {
        return(NULL);
    }

    // we have space, so get the tail arena if we have one
    IFBMemoryArena_Ptr tail_arena = ifb_memory_arena_reserved_tail();
    
    if (!tail_arena) {

        // we don't have one, so make our first arena
        _memory_manager.arenas_reserved = (IFBMemoryArena_Ptr)_memory_manager.managed_memory;
        IFBMemoryArena_Ptr first_arena_ptr = _memory_manager.arenas_reserved; 

        ifb_memory_arena_reset_new(
            first_arena_ptr,
            tag);

        return(first_arena_ptr);
    }

    // we do have one, so lets check if we have an arena we can recycle
    IFBMemoryArena_Ptr recycled_arena = ifb_memory_arena_recycle();
    if (recycled_arena) {

        //we have one, so add that to the tail
        tail_arena->next = recycled_arena;
        recycled_arena->previous = tail_arena;

        ifb_memory_arena_reset_new(
            recycled_arena,
            tag);

        return(recycled_arena);
    }
    
    // with nothing to recycle,we're creating a new arena,
    // so we just add that to the tail
    IFBMemoryArena_Ptr new_arena = (IFBMemoryArena_Ptr)(((memory)tail_arena) + IFB_MEMORY_ARENA_SIZE);
    tail_arena->next = new_arena;
    new_arena->previous = tail_arena;

    ifb_memory_arena_reset_new(
        new_arena,
        tag);

    return(new_arena);
}

void
ifb_memory_arena_release(
    IFBMemoryArena_Ptr arena_ptr) {

    //make sure we have a valid arena
    if (!arena_ptr) {
        return;
    }

    //get the arenas adjancent to this one
    IFBMemoryArena_Ptr arena_previous = arena_ptr->previous;
    IFBMemoryArena_Ptr arena_next     = arena_ptr->next;

    //if they aren't null, join them together
    if (arena_previous) {
        arena_previous->next = arena_next;
    }
    
    if (arena_next) {
        arena_next->previous = arena_previous;
    }

    //add this to the list of released arenas
    IFBMemoryArena_Ptr released_tail_arena = ifb_memory_arena_released_tail();
    if (!released_tail_arena) {
        _memory_manager.arenas_released = released_tail_arena;
        return;
    }

    released_tail_arena->next = released_tail_arena;
}

internal memory
ifb_memory_arena_bytes_push(
    IFBMemoryArena_Ptr arena_ptr,
    u64                size_bytes) {

    if (!arena_ptr || size_bytes == 0) {
        return(NULL);
    }

    u64 new_arena_used_size = arena_ptr->used_memory + size_bytes; 
    u64 max_arena_memory = IFB_MEMORY_ARENA_SIZE - sizeof(IFBMemoryArena);

    if (new_arena_used_size > max_arena_memory) {
        return(NULL);
    }

    memory pushed_memory = arena_ptr->memory + size_bytes;

    return(pushed_memory);
}

internal void
ifb_memory_arena_bytes_pop(
    IFBMemoryArena_Ptr arena_ptr,
    u64                size_bytes) {

    if (!arena_ptr || size_bytes == 0) {
        return(NULL);
    }

    if (size_bytes > arena_ptr->used_memory) {
        size_bytes = arena_ptr->used_memory;
    }

    memory new_arena_end = arena_ptr->memory + (arena_ptr->used_memory - size_bytes);

    memset(
        new_arena_end,
        0,
        size_bytes
    );

    arena_ptr->used_memory -= size_bytes;
}

internal void
ifb_memory_arena_reset(
    IFBMemoryArena_Ptr arena_ptr) {

    if (!arena_ptr) {
        return;
    }

    memset(
        arena_ptr->memory,
        0,
        IFB_MEMORY_ARENA_SIZE
    );

    arena_ptr->used_memory = 0;
}