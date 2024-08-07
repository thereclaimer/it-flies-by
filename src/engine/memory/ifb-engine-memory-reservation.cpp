#pragma once

#include "ifb-engine-memory.hpp"

internal void 
ifb_engine::memory_reserve(
          IFBEngineMemoryReservation& reservation,
    const char*                       tag_value,
    const u64                         minimum_size) {

    IFBEngineMemoryContext& context = ifb_engine::memory_context_get();

    //align the allocation size
    u64 reservation_size = 
        ifb_engine::memory_alignment_pow_2(
            minimum_size,
            context.allocation_granularity);

    //reserve the memory
    memory reserved_memory = 
        ifb_engine_platform_memory_reserve(reservation_size);

    //create the stack
    ifb_engine::stack_create(
        reservation.tag,
        reserved_memory,
        reservation_size);

    //everything else
    reservation.tag        = ifb_tag(tag_value);
    reservation.regions    = NULL;

    //put the reservation at the front
    reservation.next = context.reservations;
    context.reservations = reservation;
}