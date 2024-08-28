#pragma once

#include "ifb-engine-memory-internal.hpp"
#include "ifb-engine-platform.hpp"

/********************************************************************************************/
/* EXTERNAL                                                                              */
/********************************************************************************************/

external const size_t
ifb_engine::memory_reservation_calculate_minimum_size(
    const size_t struct_size,
    const size_t struct_alignment,
    const size_t struct_count) {
    
    const size_t struct_size_aligned = 
        ifb_engine::memory_manager_align_a_to_b(
            struct_size,
            struct_alignment);

    const size_t total_struct_size =
        struct_size_aligned *
        struct_count; 

    const size_t allocation_granularity = 
        ifb_engine::memory_manager_allocation_granularity();

    const size_t minimum_reservation_size = 
        ifb_engine::memory_manager_align_a_to_b(
            total_struct_size,
            allocation_granularity);

    return(minimum_reservation_size);
}

external const IFBEngineMemoryReservation 
ifb_engine::memory_reserve(
    const char*  tag_value,
    const size_t minimum_reservation_size,
    const size_t minimum_arena_size) {

    //get the sizes we need
    const size_t allocation_granularity = ifb_engine::memory_manager_allocation_granularity();
    const size_t page_size              = ifb_engine::memory_manager_page_size();

    //align the reservation and arena sizes to the page size
    const size_t aligned_reservation_size = ifb_engine::memory_manager_align_a_to_b(minimum_reservation_size, page_size);
    const size_t aligned_arena_size       = ifb_engine::memory_manager_align_a_to_b(minimum_arena_size,       page_size);

    //get the arena count and table size
    const size_t arena_count      = aligned_reservation_size / aligned_arena_size;  
    const size_t arena_table_size = ifb_engine_internal::memory_reservation_arena_table_size(arena_count);    
    const size_t reservation_size = aligned_reservation_size + arena_table_size;
    const size_t total_reservation_size_aligned = 
        ifb_engine::memory_manager_align_a_to_b(
            reservation_size,
            allocation_granularity);        

    //reserve the memory
    const memory next_reservation_start = ifb_engine::memory_manager_next_reservation_start();
    const memory reservation_start = 
        ifb_engine_platform_memory_reserve(
            next_reservation_start,
            total_reservation_size_aligned);

    ifb_assert(reservation_start);

    //get the next reservation
    IFBEngineMemoryReservation_Impl* new_reservation = 
        ifb_engine_internal::memory_manager_next_reservation();        
    ifb_assert(new_reservation);

    //initialize the reservation
    new_reservation->tag              = ifb_tag(tag_value);
    new_reservation->size_total       = total_reservation_size_aligned;
    new_reservation->size_usable      = aligned_reservation_size;
    new_reservation->start            = reservation_start;

    //initialize the arena table
    IFBEngineMemoryArenaTable& arena_table = new_reservation->arena_table;
    arena_table.row_count           = arena_count;
    arena_table.arena_size          = aligned_arena_size;
    arena_table.table_start         = reservation_start + aligned_reservation_size; 
    
    arena_table.columns.key         = (IFBEngineMemoryArenaKey*)arena_table.table_start;
    arena_table.columns.commit      = (memory*)(arena_table.table_start    + (sizeof(IFBEngineMemoryArenaKey) * arena_count));
    arena_table.columns.position    = (size_t*)(arena_table.columns.commit + (sizeof(memory) * arena_count));

    //set the starting reservation addresses
    memory reservation_offset = reservation_start;
    for (
        size_t arena_index = 0;
        arena_index < arena_count;
        ++arena_index) {

        IFBEngineMemoryArenaKey key;
        key.arena_index       = arena_index;
        key.reservation_index = new_reservation->index;

        arena_table.columns.key[arena_index]      = key;  
        arena_table.columns.commit[arena_index]   = NULL;
        arena_table.columns.position[arena_index] = 0;

        reservation_offset += aligned_arena_size;
    }

    return(new_reservation);
}

external const IFBEngineMemoryReservation
ifb_engine::memory_reserve_struct_pool(
    const char*  tag_value,
    const size_t struct_size,
    const size_t struct_alignment,
    const size_t struct_count) {

    //the pool size is our reservation 
    const size_t aligned_pool_size = 
        ifb_engine::memory_reservation_calculate_minimum_size(
            struct_size,
            struct_alignment,
            struct_count);

    //we'll use a page as an arena
    const size_t arena_size = ifb_engine::memory_manager_page_size();

    //create the reservation and return
    const IFBEngineMemoryReservation new_reservation = 
        ifb_engine::memory_reserve(
            tag_value,
            aligned_pool_size,
            arena_size);

    ifb_assert(new_reservation);

    return(new_reservation);
}

external const IFBEngineMemoryReservation
ifb_engine::memory_reserve_arena_pool(
    const char*  tag_value,
    const size_t minimum_arena_size,
    const size_t arena_count) {

    //calculate the sizes
    const size_t page_size               = ifb_engine::memory_manager_page_size();
    const size_t arena_size_aligned      = ifb_engine::memory_manager_align_a_to_b(minimum_arena_size,page_size);
    const size_t arena_pool_size_aligned = arena_size_aligned * arena_count; 

    //create the reservation and return
    const IFBEngineMemoryReservation new_reservation = 
        ifb_engine::memory_reserve(
            tag_value,
            arena_pool_size_aligned,
            arena_size_aligned);

    ifb_assert(new_reservation);

    return(new_reservation);
}

external const size_t
ifb_engine::memory_reservation_size_total(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation;

    return(reservation_impl->size_total);    
}

external const size_t
ifb_engine::memory_reservation_size_used(
    const IFBEngineMemoryReservation reservation) {

    //get our stuff
    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation;
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;

    //add up the sizes of committed arenas
    size_t reservation_size_used = 0;
    for (
        size_t arena_index = 0;
        arena_index < arena_table.row_count;
        ++arena_index) {
        
        reservation_size_used +=
            arena_table.columns.commit[arena_index]
            ? arena_table.arena_size
            : 0;
    }

    return(reservation_size_used);
}

external const size_t
ifb_engine::memory_reservation_size_free(
    const IFBEngineMemoryReservation reservation) {

    //get our stuff
    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation;
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;

    //add up the sizes of uncommitted arenas
    size_t reservation_size_free = 0;
    for (
        size_t arena_index = 0;
        arena_index < arena_table.row_count;
        ++arena_index) {
        
        reservation_size_free +=
            !arena_table.columns.commit[arena_index]
            ? arena_table.arena_size
            : 0;
    }

    return(reservation_size_free);
}

external const char*
ifb_engine::memory_reservation_tag_value(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation;
    return(reservation_impl->tag.value);
}


external void 
ifb_engine::memory_reservation_reset(
    const IFBEngineMemoryReservation reservation) {

    //get our stuff
    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation;
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;
    
    for (
        size_t arena_index = 0;
        arena_index < arena_table.row_count;
        ++arena_index) {
        
        if (arena_table.columns.commit[arena_index]) {

            ifb_engine::memory_arena_decommit(&arena_table.columns.key[arena_index]);
            arena_table.columns.commit[arena_index]   = NULL;
            arena_table.columns.position[arena_index] = 0;
        }
    }
}

/********************************************************************************************/
/* INTERNAL                                                                                 */
/********************************************************************************************/

internal const size_t 
ifb_engine_internal::memory_reservation_arena_table_size(
    const size_t arena_count) {

    const size_t row_size   = sizeof(IFBEngineMemoryArena_Impl);
    const size_t table_size = row_size * arena_count;

    return(table_size);  
}