#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

using namespace ifb_engine_memory; 

global IFBEngineMemoryContext memory_context;

external void
context_create(
    void) {

    //initialize the structure
    memory_context = {0};
    memory_context.allocation_granularity = ifb_engine_platform_memory_allocation_granularity();
    memory_context.page_size_small        = ifb_engine_platform_memory_page_size();
    memory_context.reservations_used      = NULL;

    //this is our memory for the resevation tracking
    memory_context.initial_reservation = 
        ifb_engine_platform_memory_reserve_small_pages(memory_context.allocation_granularity);
    ifb_assert(memory_context.initial_reservation);

    //commit the resevation tracking memory
    memory_context.initial_commit = 
        ifb_engine_platform_memory_commit(
            memory_context.initial_reservation,
            memory_context.allocation_granularity);
    ifb_assert(memory_context.initial_commit);

    //set the pointers up
    const size_t reservation_node_size = sizeof(IFBEngineMemoryReservation_Impl);
    memory_context.reservations_free = (IFBEngineMemoryReservation_Impl*)memory_context.initial_commit;
    IFBEngineMemoryReservation_Impl* previous = memory_context.reservations_free;
    IFBEngineMemoryReservation_Impl* current  = NULL;
    previous->previous = NULL;

    //initialize the free list of reservations
    for (
        size_t offset = 0;
        offset < memory_context.allocation_granularity;
        offset += reservation_node_size) {

        previous->next = (IFBEngineMemoryReservation_Impl*)((memory)previous + reservation_node_size);
        current        = previous->next;

        current->next             = NULL;
        current->previous         = previous;
        current->regions          = NULL;
        current->start            = NULL;
        current->tag              = {0};
        current->page_type        = 0;
        current->owner_process    = 0;
        current->owner_thread     = 0;
        current->reservation_size = 0 
        current->region_list_size = 0;
        current->total_size       = 0;
        current->page_size        = 0;  
    }
}

external void
context_destroy(
    void) {

}

external size_t
context_granularity(
    void) {

}

external size_t
context_page_size_small(
    void) {

}

external size_t
context_page_size_large(
    void) {

}

external size_t
context_reservation_count(
    void) {

}

external size_t
context_reserved_size_total(
    void) {

}
