#pragma once

#include "ifb-engine-memory.hpp"

internal u64
ifb_engine_memory_arena_total_size_bytes(
    IFBEngineMemoryArenaPtr arena_ptr) {

    u64 arena_total_size_bytes = 
        sizeof(IFBEngineMemoryArena`) +
        arena_ptr->block.memory_size_bytes;

    return(arena_total_size_bytes);
}

internal u64
ifb_engine_memory_arena_total_required_size_bytes(
    u64 arena_size_bytes) {

    u64 required_size_bytes = 
        sizeof(IFBEngineMemoryArena) +
        region_size_bytes;        

    return(required_size_bytes);
}