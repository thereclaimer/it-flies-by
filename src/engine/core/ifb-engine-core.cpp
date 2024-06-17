#pragma once

#include "ifb-engine-core.hpp"

global IFBEngine ifb_engine;

external IFBEngine_Ptr
ifb_engine_create_and_initialize(
    IFBPlatformApi platform) {

    //validate the platform api
    ifb_assert(platform.file_size);
    ifb_assert(platform.file_read);
    ifb_assert(platform.file_write);
    ifb_assert(platform.file_open);
    ifb_assert(platform.file_close);
    ifb_assert(platform.memory_allocate);
    ifb_assert(platform.memory_free);
    ifb_assert(platform.ticks);
    ifb_assert(platform.delta_time_ms);
    ifb_assert(platform.sleep);

    ifb_platform_api = platform;

    //initialize the engine
    ifb_engine = {0};

    //memory
    ifb_engine.memory = ifb_engine_memory_create_and_initialize();
    ifb_assert(ifb_engine.memory);

    ifb_engine.frame_allocator.arena = 
        ifb_engine_memory_arena_reserve_64mb(
            "ENGINE FRAME");

    return(&ifb_engine);
}

external void
ifb_engine_render_frame() {

    ifb_nop();


    //reset the frame allocator
    ifb_engine_memory_arena_reset(ifb_engine.frame_allocator.arena);

    return;
}

internal memory
ifb_engine_frame_allocator_reserve(
    u64 size) {
    
    ifb_assert(ifb_engine.frame_allocator.arena);

    memory reservation = 
        ifb_engine_memory_arena_bytes_push(
            ifb_engine.frame_allocator.arena,
            size);

    ifb_assert(reservation);

    return(reservation);
}