#pragma once

#include "ifb-engine-core.hpp"
#include "ifb-engine-memory.hpp"

internal IFBEngineMemory_Ptr
ifb_engine_memory_create_and_initialize() {

    ifb_engine_memory = {0};

    //aquire platform memory
    ifb_engine_memory.platform_memory = ifb_engine_platform_memory_allocate(IFB_ENGINE_MEMORY_REQUIREMENT);    
    ITFLIESBY_ASSERT(ifb_engine_memory.platform_memory);

    ifb_engine_memory.platform_memory_size_bytes = IFB_ENGINE_MEMORY_REQUIREMENT;

    //initialize the memory managers
    const u64 manager_memory_size = IFB_ENGINE_MEMORY_REQUIREMENT / 2;

    ifb_engine_memory.memory_manager_64kb = 
        ifb_engine_memory_manager_create_and_initialize(
            manager_memory_size,
            ITFLIESBY_MATH_KILOBYTES(64),
            ifb_engine_memory.platform_memory);
    
    ifb_engine_memory.memory_manager_64mb = 
        ifb_engine_memory_manager_create_and_initialize(
            manager_memory_size,
            ITFLIESBY_MATH_MEGABYTES(64),
            &ifb_engine_memory.platform_memory[manager_memory_size]);
}