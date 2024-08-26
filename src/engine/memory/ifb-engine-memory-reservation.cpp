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

}

external const IFBEngineMemoryReservation 
ifb_engine::memory_reserve(
    const char*  tag_value,
    const size_t minimum_reservation_size,
    const size_t minimum_arena_size) {

}

external const IFBEngineMemoryReservation
ifb_engine::memory_reserve_struct_pool(
    const char*  tag_value,
    const size_t struct_size,
    const size_t struct_alignment,
    const size_t struct_count) {

}

external const IFBEngineMemoryReservation
ifb_engine::memory_reserve_arena_pool(
    const char*  tag_value,
    const size_t minimum_arena_size,
    const size_t arena_count) {

}

external const size_t
ifb_engine::memory_reservation_size_total(
    const IFBEngineMemoryReservation reservation) {

}

external const size_t
ifb_engine::memory_reservation_size_used(
    const IFBEngineMemoryReservation reservation) {

}

external const size_t
ifb_engine::memory_reservation_size_free(
    const IFBEngineMemoryReservation reservation) {

}

external const char*
ifb_engine::memory_reservation_tag_value(
    const IFBEngineMemoryReservation reservation) {

}


external void 
ifb_engine::memory_reservation_reset(
    const IFBEngineMemoryReservation reservation) {

}

/********************************************************************************************/
/* INTERNAL                                                                                 */
/********************************************************************************************/

internal const size_t 
ifb_engine::memory_reservation_arena_table_size(
    const size_t aligned_reservation_size,
    const size_t aligned_arena_size) {

}