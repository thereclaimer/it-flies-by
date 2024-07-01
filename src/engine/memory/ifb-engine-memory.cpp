#pragma once

#include "ifb-engine-memory.hpp"

global IFBEngineMemory ifb_engine_memory;

internal IFBEngineMemoryPtr
ifb_engine_memory_create_and_initialize() {

    ifb_engine_memory = {0};

    //get our platform memory
    memory platform_memory = ifb_engine_platform_memory_allocate(IFB_ENGINE_MEMORY_REQUIREMENT);
    ifb_assert(platform_memory);

    //initialize engine memory
    ifb_engine_memory.platform_memory_block.block      = platform_memory;
    ifb_engine_memory.platform_memory_block.size_bytes = IFB_ENGINE_MEMORY_REQUIREMENT;
    ifb_engine_memory.regions                          = NULL;

    return(&ifb_engine_memory);
}

internal IFBEngineMemoryPtr
ifb_engine_memory_get() {
    return(&ifb_engine_memory);
}

internal u64
ifb_engine_memory_space_availabe_bytes() {

    u64 space_available = ifb_engine_memory.platform_memory_block.memory_size_bytes;

    for (
        IFBEngineMemoryRegionPtr region_ptr = ifb_engine_memory.regions;
        region_ptr && region_ptr->next;
        region_ptr = region_ptr->next) {

        space_available -= ifb_engine_memory_region_total_size_bytes(region_ptr);
    }

    return(space_available);
}