#pragma once

#include "ifb-engine.hpp"

global IFBEngine ifb_engine;

internal void 
ifb_engine_systems_initialize() {

    //memory
    ifb_engine.memory = ifb_engine_memory_create_and_initialize();
    ifb_assert(ifb_engine.memory);

    //assets
    ifb_engine.assets = ifb_engine_assets_create_and_initialize();
    ifb_assert(ifb_engine.assets);
}

external IFBEnginePtr
ifb_engine_create_and_initialize(
    IFBPlatformApi platform) {

    //validate the platform api
    ifb_engine_platform_validate(platform);

    //initialize the engine
    ifb_engine = {0};
    ifb_engine_systems_initialize();

    //post initialization stuff
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