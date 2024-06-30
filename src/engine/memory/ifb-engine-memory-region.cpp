#pragma once

#include "ifb-engine-memory.hpp"

internal void
ifb_engine_memory_region_get_next(
    IFBEngineMemoryPtr        memory_ptr,
    IFBEngineMemoryRegionPtr* region_ptr_ptr) {

    if (!memory_ptr->regions) {
        region_ptr_ptr = &memory_ptr->regions
        return;
    }

    for (
        IFBEngineMemoryRegionPtr region_ptr = memory_ptr->regions;
        region_ptr->next != NULL;
        region_ptr = region_ptr->next) {

        if (!region_ptr->next) {
            
            memory next_region_memory = 
                region_ptr->block.memory + 
                region_ptr->block.memory_size_bytes;

            IFBEngineMemoryRegionPtr next_region_ptr = 
                (IFBEngineMemoryRegionPtr)next_region_memory;

            

            region_ptr_ptr = &region_ptr->next;
            return;
        }
    }
}

internal IFBEngineMemoryRegionPtr
ifb_engine_memory_region_create(
    u64  region_size_bytes,
    char region_tag[32]) {

    IFBEngineMemoryPtr       memory_ptr = ifb_engine_memory_get();
    IFBEngineMemoryRegionPtr region_ptr = NULL;

    //get the next region
    ifb_engine_memory_region_get_next(
        memory_ptr,
        &region_ptr);

    region_ptr = 


}
