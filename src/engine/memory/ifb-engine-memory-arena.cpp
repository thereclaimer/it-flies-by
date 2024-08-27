#pragma once

#include "ifb-engine-memory-internal.hpp"

external const size_t 
ifb_engine::memory_arena_calculate_minimum_size_for_struct(
    const size_t struct_size,
    const size_t struct_alignment,
    const size_t struct_count) {

}

external const IFBEngineMemoryArena 
ifb_engine::memory_arena_commit(
    const IFBEngineMemoryReservation reservation) {

}

external void 
ifb_engine::memory_arena_decommit(
    const IFBEngineMemoryArena arena) {

}

external const size_t
ifb_engine::memory_arena_size_total(
    const IFBEngineMemoryArena arena) {

}

external const size_t
ifb_engine::memory_arena_size_used(
    const IFBEngineMemoryArena arena) {

}

external const size_t
ifb_engine::memory_arena_size_free(
    const IFBEngineMemoryArena arena) {

}

external memory 
ifb_engine::memory_arena_push_bytes(
    const IFBEngineMemoryArena arena, 
    const size_t               size,
    const size_t               alignment) {

}

external void
ifb_engine::memory_arena_pull_bytes(
    const IFBEngineMemoryArena arena, 
    const size_t               size,
    const size_t               alignment) {

}