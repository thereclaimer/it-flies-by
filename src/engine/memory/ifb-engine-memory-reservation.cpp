#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

using namespace ifb_engine_memory;

/********************************************************************************************/
/* EXTERNAL                                                                                 */
/********************************************************************************************/

external IFBEngineMemoryReservation 
reserve(
    const char*                   tag_value,
    const size_t                  minimum_size,
    const IFBEngineMemoryPageType page_type) {

    IFBEngineMemoryContext& context = context_get();

    //insert a new reservation
    IFBEngineMemoryReservationTable& reservation_table = context.reservation_table;
    IFBEngineMemoryReservation_Impl* reservation       = reservation_table_insert(reservation_table);
    ifb_assert(reservation);

    //align the size to the allocation granularity
    size_t reservation_size = alignment_pow_2(minimum_size, context.allocation_granularity);

    //get the page size
    //TODO: large pages
    size_t page_size        = context.page_size_small;

    //reserve system memory
    memory reserved_memory = ifb_engine_platform_memory_reserve(reservation_size);
    ifb_assert(reserved_memory);

    //initialize the reservation
    reservation->tag           = ifb_tag(tag_value);
    reservation->page_type     = page_type;
    reservation->owner_process = ifb_engine_platform_process_id();
    reservation->owner_thread  = ifb_engine_platform_thread_id();
    reservation->total_size    = reservation_size;
    reservation->page_size     = page_size;
    reservation->count_regions = 0;
    reservation->start         = reserved_memory;
    region_table_allocate(reservation->region_table);
}

external void
release(
    const IFBEngineMemoryReservation reservation) {

    
}

external size_t
reservation_space_total(
    const IFBEngineMemoryReservation reservation) {

}

external size_t
reservation_space_free(
    const IFBEngineMemoryReservation reservation) {

}

external size_t
reservation_space_occupied(
    const IFBEngineMemoryReservation reservation) {

}

external size_t
reservation_page_size(
    const IFBEngineMemoryReservation reservation) {

}

external size_t
reservation_page_count(
    const IFBEngineMemoryReservation reservation) {

}

external size_t
reservation_region_count(
    const IFBEngineMemoryReservation reservation) {

}

/********************************************************************************************/
/* INTERNAL                                                                                 */
/********************************************************************************************/

internal void
reservation_table_allocate(
    IFBEngineMemoryReservationTable& reservation_table) {

    const size_t table_size      = IFB_ENGINE_MEMORY_RESERVATION_TABLE_SIZE;
    const size_t small_page_size = context_page_size_small();
    const size_t pages_available = table_size / small_page_size;   

    reservation_table.reservations = 
        (IFBEngineMemoryReservation_Impl*)ifb_engine_platform_memory_reserve(table_size);

    ifb_assert(reservation_table.reservations);

    reservation_table.count_total     = 0;
    reservation_table.count_used      = 0;
    reservation_table.pages_used      = 0;
    reservation_table.pages_available = pages_available;
}

internal IFBEngineMemoryReservation_Impl*
reservation_table_insert(
    IFBEngineMemoryReservationTable& reservation_table) {

    const size_t reservation_size = sizeof(IFBEngineMemoryReservation_Impl);

    //commit another page if we have to
    if (reservation_table.count_used == reservation_table.count_total) {

        ifb_assert(reservation_table.pages_used < reservation_table.pages_available);
        
        size_t small_page_size       = context_page_size_small();
        size_t reservations_per_page = small_page_size / reservation_size;
        size_t committed_size        = small_page_size * reservation_table.pages_used;

        memory reservation_table_memory = (memory)reservation_table.reservations;
        memory offset                   = reservation_table_memory + committed_size;   
    
        memory committed_memory = 
            ifb_engine_platform_memory_commit(
                offset,
                small_page_size);

        ifb_assert(committed_memory == offset);

        ++reservation_table.pages_used;
        reservation_table.count_total += reservations_per_page;
    } 

    //get the next pointer in the table and increase the reservations used
    size_t index = reservation_table.count_used;
    IFBEngineMemoryReservation_Impl* reservation_pointer = &reservation_table.reservations[index];
    ++reservation_table.count_used;
    return(reservation_pointer);
}
