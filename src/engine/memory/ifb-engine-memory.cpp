#pragma once

#include "ifb-engine-memory.hpp"

global IFBEngineMemory ifb_engine_memory;

internal void 
ifb_engine_memory_create() {

    //sizes we're working with
    u64 page_size                  = ifb_engine_platform_memory_page_size();
    u64 base_memory_requirement    = IFB_ENGINE_MEMORY_REQUIREMENT;
    u64 aligned_memory_requirement = ifb_engine_memory_alignment_pow_2(base_memory_requirement,page_size);

    //reserve memory
    memory reserved_memory = ifb_engine_platform_memory_reserve(aligned_memory_requirement);
    ifb_assert(reserved_memory);

    //initialize structure
    ifb_engine_memory = {0};
    ifb_engine_memory.start          = reserved_memory;
    ifb_engine_memory.total_size     = aligned_memory_requirement;
    ifb_engine_memory.position       = 0;
    ifb_engine_memory.page_size      = page_size;
    ifb_engine_memory.system_regions = NULL;
}

internal void
ifb_engine_memory_destroy() {

    ifb_engine_platform_memory_release(ifb_engine_memory.start);
}

internal void 
ifb_engine_memory_system_region_reserve(
          IFBEngineMemorySystemRegion* system_region,
    const char*                        tag,
    const u64                          minimum_total_size,
    const u64                          minimum_arena_size) {

    ifb_assert(region);

    //region size
    u64 region_size = 
        ifb_engine_memory_alignment_pow_2(
            minimum_total_size,
            ifb_engine_memory.page_size); 

    //make sure the new region can fit
    u64  space_remaining = ifb_engine_memory.total_size - ifb_engine_memory.position;
    u64  new_position    = ifb_engine_memory.position + region_size;
    bool can_fit         = new_position <= new_position;
    ifb_assert(can_fit);  

    //put the new region at the front
    //this will be constant time during initialization
    //iterating through regions will be an uncommon operation
    if (ifb_engine_memory.system_regions) {
        ifb_engine_memory.system_regions->previous = region;
    }

    //initialize the new region
    region->tag        = ifb_tag(tag);
    region->total_size = region_size;  
    region->position   = 0;
    region->next       = ifb_engine_memory.system_regions;
    region->previous   = NULL;
    region->regions    = NULL; 
    region->start      = (ifb_engine_memory.start + region->position);

    //update the memory structure
    ifb_engine_memory.system_regions  = region;    
    ifb_engine_memory.position       += region_size;
}

/*
    //arena size
    u64 arena_size = 
        minimum_arena_size < ifb_engine_memory.page_size
        ? ifb_engine_memory.page_size
        : ifb_engine_memory_alignment_pow_2(
            minimum_arena_size,
            ifb_engine_memory.page_size);

    //make sure the arena can fit in the region
    ifb_assert(arena_size <= region_size);

*/