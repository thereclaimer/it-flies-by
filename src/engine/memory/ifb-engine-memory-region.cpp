#pragma once

#include "ifb-engine-memory.hpp"

internal void
ifb_engine::memory_region_create(
          IFBEngineMemoryRegion&      region,
          IFBEngineMemoryReservation& reservation,
    const char*                       tag_value,
    const u64                         minimum_size) {

    IFBEngineMemoryContext& context = ifb_engine::memory_context_get();

    //align the region to a page size
    u64 region_size = 
        ifb_engine::memory_alignment_pow_2(
            minimum_size,
            context.page_size);

    //make sure we can fit the region
    bool can_fit = 
        ifb_engine::stack_can_push(
            reservation.stack,
            region_size);

    ifb_assert(can_fit);

    //grab the stack pointer
    memory region_memory = ifb_engine::stack_pointer(reservation.stack);

    //update the reservation stack
    ifb_engine::stack_push(reservation.stack,region_size);

    //create the stack for this region
    ifb_engine::stack_create(
        region.stack,
        region_memory,
        region_size);

    //everything else
    region.arenas      = NULL;
    region.tag         = ifb_tag(tag_value);
    region.reservation = &reservation;

    //put the region at the front
    region.next         = reservation.regions;
    reservation.regions = region;
}
