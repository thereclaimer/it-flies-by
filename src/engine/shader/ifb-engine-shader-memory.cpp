#pragma once

#include "ifb-engine-shader-internal.hpp"

internal void
ifb_engine_shader::memory_reserve(
    void) {

    //reserve the memory
    const size_t shader_memory_reservation_size = ifb_engine_memory_megabytes(64); 
    
    ifb_engine_memory::reserve_memory(
        "SHADER MANAGER"
        ,shader_memory_reservation_size)

}

internal void
ifb_engine_shader::memory_release(
    void) {

}

internal const IFBEngineMemoryArena
ifb_engine_shader::memory_arena_commit(
    void) {

}

internal const IFBEngineMemoryArena
ifb_engine_shader::memory_arena_decommit(
    void) {

}