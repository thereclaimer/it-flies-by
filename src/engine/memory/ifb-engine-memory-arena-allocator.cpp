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
        allocator_new_memory         = (memory)region_new_ptr; 
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
        allocator_new_ptr        = (IFBEngineMemoryRegionPtr)allocator_new_ptr;         
        allocator_last_ptr->next = allocator_new_ptr; 
    }

    //make sure we got an allocator to initialize
    ifb_assert(allocator_new_ptr);

    //we have an allocator, initialize it
    allocator_new_ptr->block.memory            = allocator_new_memory + sizeof(IFBEngineMemoryArenaAllocator);
    allocator_new_ptr->block.memory_size_bytes = allocator_size_bytes;
    allocator_new_ptr->tag                     = ifb_tag(allocator_tag);
    allocator_new_ptr->arenas.arena_size_bytes = arena_size_bytes;
    allocator_new_ptr->arenas.reserved         = NULL;

    u64 remaining_arena_space_available = allocator_size_bytes;
    u64 

    for (

    )

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