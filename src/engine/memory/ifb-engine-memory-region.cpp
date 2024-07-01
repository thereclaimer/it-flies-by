#pragma once

#include "ifb-engine-memory.hpp"

internal IFBEngineMemoryRegionPtr
ifb_engine_memory_region_create(
    u64  region_size_bytes,
    char region_tag[32]) {

    //make sure we can fit the region
    u64 region_space_required  = ifb_engine_memory_region_total_required_size_bytes(region_size_bytes);
    u64 memory_space_available = ifb_engine_memory_space_availabe_bytes();
    if (memory_space_available < region_space_required) {
        return(NULL);
    }

    //we can, so set things up
    IFBEngineMemoryPtr       memory_ptr        = ifb_engine_memory_get();
    IFBEngineMemoryRegionPtr region_last_ptr   = memory_ptr->regions; 
    IFBEngineMemoryRegionPtr region_new_ptr    = NULL; 
    memory                   region_new_memory = NULL;

    //if there's no existing regions, just grab the first one
    if (!region_last_ptr) {
        region_new_ptr      = (IFBEngineMemoryRegionPtr)memory_ptr->platform_memory_block.memory;
        region_new_memory   = (memory)region_new_ptr; 
        memory_ptr->regions = region_new_ptr;
    }
    //if there are existing regions, grab the last one
    else {

        for (
            region_last_ptr;
            region_last_ptr && region_last_ptr->next;
            region_last_ptr =  region_last_ptr->next);
        
        memory region_last_memory = (memory)region_last_ptr;
        u64    region_last_size   = ifb_engine_memory_region_total_size_bytes(region_last_ptr);            
    
        region_new_memory     = region_last_memory + region_last_size;
        region_new_ptr        = (IFBEngineMemoryRegionPtr)region_new_memory;         
        region_last_ptr->next = region_new_ptr; 
    }

    //make sure we got a region to initialize
    ifb_assert(region_new_ptr);

    //we have a region, so initialize it
    region_new_ptr->arena_allocators        = NULL;
    region_new_ptr->block.memory            = region_new_memory + sizeof(IFBEngineMemoryRegion);
    region_new_ptr->block.memory_size_bytes = region_size_bytes;
    region_new_ptr->next                    = NULL; 
    region_new_ptr->tag                     = ifb_tag(region_tag);                    

    return(region_new_ptr);
}

internal u64
ifb_engine_memory_region_total_size_bytes(
    IFBEngineMemoryRegionPtr region_ptr) {

    u64 region_total_size_bytes = 
        sizeof(IFBEngineMemoryRegion) +
        region_ptr->block.memory_size_bytes;

    return(region_total_size_bytes);
}

internal u64
ifb_engine_memory_region_total_required_size_bytes(
    u64 region_size_bytes) {

    u64 required_size_bytes = 
        sizeof(IFBEngineMemoryRegion) +
        region_size_bytes;        

    return(required_size_bytes);
}

internal u64
ifb_engine_memory_region_space_availabe_bytes(
    IFBEngineMemoryRegionPtr region_ptr) {

    u64 space_available = region_ptr->block.memory_size_bytes;

    for (
        IFBEngineMemoryArenaAllocatorPtr allocator = region_ptr;
        allocator && allocator->next;
        allocator =  allocator->next) {

        space_available -= ifb_engine_memory_arena_allocator_total_size_bytes(allocator);
    }

    return(space_available);
}