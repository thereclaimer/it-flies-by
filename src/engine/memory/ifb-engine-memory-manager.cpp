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
}

external void 
ifb_engine::memory_manager_shut_down(
    void) {

    for (
        size_t reservation_index = 0;
        reservation_index < _memory_manager.reservation_table.count;
        ++reservation_index) {
            
        //TODO
        ifb_panic();
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

external const size_t
ifb_engine::memory_manager_align_a_to_b(
    const size_t a, 
    const size_t b) {

    return(((a) + (b) - 1)&(~((b) - 1))); 
}