#pragma once

#include "ifb-engine-memory-internal.hpp"
#include "ifb-engine-platform.hpp"

global IFBEngineMemoryManager _memory_manager;

/********************************************************************************************/
/* EXTERNAL                                                                                 */
/********************************************************************************************/

external void 
ifb_engine::memory_manager_start_up(
    void) {

    _memory_manager = {0};

    _memory_manager.page_size              = ifb_engine_platform_memory_page_size_small();
    _memory_manager.allocation_granularity = ifb_engine_platform_memory_allocation_granularity();

    IFBEngineMemoryReservationTable& reservation_table = _memory_manager.reservation_table;
    reservation_table.max        = IFB_ENGINE_MEMORY_RESERVATIONS_MAX;
    reservation_table.count      = 0;
    reservation_table.next_start = NULL;

    for (
        size_t reservation_index = 0;
        reservation_index < reservation_table.max;
        ++reservation_index) {

        IFBEngineMemoryReservation_Impl& reservation_impl = reservation_table.array[reservation_index];
        reservation_impl       = {0};
        reservation_impl.index = reservation_index;
    }
}

external void 
ifb_engine::memory_manager_shut_down(
    void) {

    IFBEngineMemoryReservationTable& reservation_table = _memory_manager.reservation_table;

    for (
        size_t reservation_index = 0;
        reservation_index < _memory_manager.reservation_table.count;
        ++reservation_index) {

        IFBEngineMemoryReservation_Impl& reservation_impl = reservation_table.array[reservation_index];
        ifb_engine_platform_memory_release(
            reservation_impl.start,
            reservation_impl.size_total);
    }
}

external const size_t 
ifb_engine::memory_manager_page_size(
    void) {

    return(_memory_manager.page_size);
}

external const size_t 
ifb_engine::memory_manager_allocation_granularity(
    void) {

    return(_memory_manager.allocation_granularity);
}

external const size_t 
ifb_engine::memory_manager_reservation_count(
    void) {

    return(_memory_manager.reservation_table.count);
}

external const size_t 
ifb_engine::memory_manager_reservation_max(
    void) {

    return(_memory_manager.reservation_table.max);
}

external const memory 
ifb_engine::memory_manager_next_reservation_start(
    void) {

    return(_memory_manager.reservation_table.next_start);
}


external const size_t
ifb_engine::memory_manager_align_a_to_b(
    const size_t a, 
    const size_t b) {

    return(((a) + (b) - 1)&(~((b) - 1))); 
}

/********************************************************************************************/
/* INTERNAL                                                                                 */
/********************************************************************************************/

internal IFBEngineMemoryReservation_Impl* 
ifb_engine_internal::memory_manager_next_reservation(void) {

    const size_t reservation_index = _memory_manager.reservation_table.count;
    if (reservation_index >= _memory_manager.reservation_table.max) {
        return(NULL);
    }

    IFBEngineMemoryReservation_Impl* next_reservation_impl = 
        &_memory_manager.reservation_table.array[reservation_index];    

    return(next_reservation_impl);
}    

internal IFBEngineMemoryReservation_Impl* 
ifb_engine_internal::memory_manager_reservation_at_index(
    const size_t reservation_index) {

    if (reservation_index >= _memory_manager.reservation_table.max) {
        return(NULL);
    }

    IFBEngineMemoryReservation_Impl* reservation_impl = 
        &_memory_manager.reservation_table.array[reservation_index];    

    return(reservation_impl);
}