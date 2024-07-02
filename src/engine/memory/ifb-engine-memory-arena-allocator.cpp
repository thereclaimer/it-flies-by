#pragma once

#include "ifb-engine-memory.hpp"

internal IFBEngineMemoryArenaAllocatorPtr
ifb_engine_memory_arena_allocator_create(
    IFBEngineMemoryRegionPtr region_ptr,
    char                     allocator_tag[32],
    u64                      arena_size_bytes,
    u64                      allocator_size_bytes) {

    //make sure we have valid requirements for an allocator
    if (
        !region_ptr                               ||
        arena_size_bytes == 0                     ||
        allocator_size_bytes == 0                 ||
        arena_size_bytes >= allocator_size_bytes) {
        
        return(NULL);
    }

    //make sure we can fit the allocator
    u64 allocator_space_required = ifb_engine_memory_arena_allocator_total_required_size_bytes(allocator_size_bytes);
    u64 region_space_available   = ifb_engine_memory_region_space_availabe_bytes(region_ptr);
    if (region_space_available < allocator_space_required) {
        return(NULL);
    }

    //if so, set things up
    IFBEngineMemoryArenaAllocatorPtr allocator_last_ptr   = region_ptr->arena_allocators;     
    IFBEngineMemoryArenaAllocatorPtr allocator_new_ptr    = NULL;
    memory                           allocator_new_memory = NULL; 

    //basically the same as creating a region at first
    //if there's no existing allocator, just grab the first one
    if (!allocator_last_ptr) {
        allocator_new_ptr            = (IFBEngineMemoryArenaAllocatorPtr)region_ptr->block.memory;
        allocator_new_memory         = (memory)allocator_new_ptr; 
        region_ptr->arena_allocators = allocator_new_ptr;
    }
    //if there are existing allocators, grab the last one
    else {

        for ( 
            allocator_last_ptr;
            allocator_last_ptr && allocator_last_ptr->next;
            allocator_last_ptr =  allocator_last_ptr->next);
        
        memory allocator_last_memory = (memory)allocator_last_ptr;
        u64    allocator_last_size   = ifb_engine_memory_arena_allocator_total_size_bytes(allocator_last_ptr);            
    
        allocator_new_memory     = allocator_last_memory + allocator_last_size;
        allocator_new_ptr        = (IFBEngineMemoryArenaAllocatorPtr)allocator_new_memory;         
        allocator_last_ptr->next = allocator_new_ptr; 
    }

    //make sure we got an allocator to initialize
    ifb_assert(allocator_new_ptr);

    //we have an allocator, initialize it
    allocator_new_ptr->tag                     = ifb_tag(allocator_tag);
    allocator_new_ptr->block.memory            = allocator_new_memory + sizeof(IFBEngineMemoryArenaAllocator);
    allocator_new_ptr->block.memory_size_bytes = allocator_size_bytes;
    allocator_new_ptr->region                  = region_ptr;
    allocator_new_ptr->next                    = NULL;
    allocator_new_ptr->arenas.arena_size_bytes = arena_size_bytes;
    allocator_new_ptr->arenas.reserved         = NULL;

    //set initiali offsets
    u64 remaining_arena_space_available = allocator_size_bytes;
    u64 single_arena_total_size         = ifb_engine_memory_arena_total_required_size_bytes(arena_size_bytes); 
    u64 last_arena_offset               = allocator_size_bytes - single_arena_total_size; 

    //set the first allocator pointer
    memory arena_memory = allocator_new_ptr->block.memory;

    allocator_new_ptr->arenas.available = (IFBEngineMemoryArenaPtr)arena_memory; 

    IFBEngineMemoryArenaPtr arena_previous  = allocator_new_ptr->arenas.available;
    arena_previous->block.memory            = arena_memory + + sizeof(IFBEngineMemoryArena);
    arena_previous->block.memory_size_bytes = arena_size_bytes; 
    arena_previous->bytes_used              = 0;
    arena_previous->next                    = NULL;
    arena_previous->previous                = NULL;
    arena_previous->allocator               = allocator_new_ptr;    

    IFBEngineMemoryArenaPtr arena_current         = NULL;
    memory                  arena_current_memory  = NULL;
    memory                  arena_previous_memory = (memory)arena_previous; 

    for (
        u64 offset = single_arena_total_size;
        offset <  last_arena_offset;
        offset += single_arena_total_size) {

        arena_current_memory = arena_previous_memory + single_arena_total_size;
        arena_current        = (IFBEngineMemoryArenaPtr)arena_current_memory;
        arena_previous->next = arena_current;

        //TODO: something is going on that is causing the block memory to have the same address of the 
        //arena, the block memory should be at the end of the arena structure

        arena_current->block.memory            = arena_current_memory + sizeof(IFBEngineMemoryArena);
        arena_current->block.memory_size_bytes = arena_size_bytes;
        arena_current->bytes_used              = 0;
        arena_current->next                    = NULL;
        arena_current->allocator               = allocator_new_ptr;    

        arena_previous_memory = (memory)arena_current;
        arena_previous->next  = arena_current;
        arena_previous        = arena_current;
    }

    return(allocator_new_ptr);
}

internal u64
ifb_engine_memory_arena_allocator_total_size_bytes(
    IFBEngineMemoryArenaAllocatorPtr allocator_ptr) {

    u64 allocator_total_size_bytes = 
        sizeof(IFBEngineMemoryArenaAllocator) +
        allocator_ptr->block.memory_size_bytes;

    return(allocator_total_size_bytes);
}

internal u64
ifb_engine_memory_arena_allocator_total_required_size_bytes(
    u64 allocator_size_bytes) {

    u64 required_size_bytes = 
        sizeof(IFBEngineMemoryArenaAllocator) +
        allocator_size_bytes;

    return(required_size_bytes);
}