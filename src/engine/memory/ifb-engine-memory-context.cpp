#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

global IFBEngineMemoryContext memory_context;

/********************************************************************************************/
/* EXTERNAL                                                                                 */
/********************************************************************************************/

external void
ifb_engine_memory::context_create(
    void) {

    memory_context = {0};
    memory_context.allocation_granularity = ifb_engine_platform_memory_allocation_granularity();
    memory_context.page_size_small        = ifb_engine_platform_memory_page_size_small();
    memory_context.page_size_large        = ifb_engine_platform_memory_page_size_large();
    memory_context.owner_process_id       = ifb_engine_platform_process_id();
    memory_context.reservations           = NULL;
}

external void
ifb_engine_memory::context_destroy(
    void) {

    for (
        IFBEngineMemoryReservation_Impl* reservation = memory_context.reservations;
        reservation != NULL;
        reservation = reservation->next) {

        ifb_engine_memory::release_memory(reservation);
    }

    memory_context = {0};
}

external const size_t
ifb_engine_memory::context_granularity(
    void) {

    return(memory_context.allocation_granularity);
}

external const size_t
ifb_engine_memory::context_page_size_small(
    void) {

    return(memory_context.page_size_small);
}

external const size_t
ifb_engine_memory::context_page_size_large(
    void) {

    return(memory_context.page_size_large);
}

external const size_t
ifb_engine_memory::context_reservation_count(
    void) {

    size_t reservation_count = 0;

    for (
        IFBEngineMemoryReservation_Impl* reservation = memory_context.reservations;
        reservation != NULL;
        reservation = reservation->next) {

        ++reservation_count;       
    }
}

external const size_t
ifb_engine_memory::context_reserved_size_total(
    void) {

    size_t reserved_size_total = 0;

    for (
        IFBEngineMemoryReservation_Impl* reservation = memory_context.reservations;
        reservation != NULL;
        reservation = reservation->next) {

        reserved_size_total += ifb_engine_memory::reservation_space_total(reservation);
    }

    return(reserved_size_total);
}

external const size_t
ifb_engine_memory::context_align_to_small_page(
    const size_t size) {

    size_t alignment = 
        ifb_engine_memory::alignment_pow_2(
            size,
            memory_context.page_size_small);

    return(alignment);
}

external const size_t
ifb_engine_memory::context_align_to_large_page(
    const size_t size) {

    size_t alignment = 
        ifb_engine_memory::alignment_pow_2(
            size,
            memory_context.page_size_large);

    return(alignment);
}

external const size_t
ifb_engine_memory::context_align_to_allocation_granularity(
    const size_t size) {
        
    size_t alignment = 
        ifb_engine_memory::alignment_pow_2(
            size,
            memory_context.allocation_granularity);

    return(alignment);
}

external const size_t
ifb_engine_memory::context_page_size(
    const IFBEngineMemoryPageType page_type) {

    size_t page_size = 
        page_type == IFBEngineMemoryPageType_Small
        ? memory_context.page_size_small
        : memory_context.page_size_large;

    return(page_size);
}

/********************************************************************************************/
/* INTERNAL                                                                                 */
/********************************************************************************************/

internal IFBEngineMemoryContext&
context_get(
    void) {

    return(memory_context);
}

internal void
context_add_reservation(
    IFBEngineMemoryReservation_Impl* reservation) {

    ifb_assert(reservation);

    reservation->next = memory_context.reservations;
    memory_context.reservations->previous = reservation;
    memory_context.reservations = reservation;
}

internal void
context_remove_reservation(
    IFBEngineMemoryReservation_Impl* reservation) {

    ifb_assert(reservation);

    if (reservation->previous) {
        reservation->previous->next = reservation->next;
    }

    if (reservation->next) {
        reservation->next->previous = reservation->previous;
    }

    reservation->previous = NULL;
    reservation->next     = NULL;
}