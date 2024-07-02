#pragma once

#include "ifb-engine.hpp"

global IFBEngineCoreMemory ifb_engine_core_memory;

internal IFBEngineCoreMemoryFrameAllocator
ifb_engine_core_memory_frame_allocator_create() {

    IFBEngineCoreMemoryFrameAllocator frame_allocator = {0};

    frame_allocator.arena = ifb_engine_core_memory_arena_reserve_16kb();
    ifb_assert(frame_allocator.arena);

    return(frame_allocator);
}

internal IFBEngineCoreMemoryArenaAllocator
ifb_engine_core_memory_arena_allocator_create() {

    IFBEngineCoreMemoryArenaAllocator arena_allocator = {0};

    arena_allocator.arena_16kb = 
        ifb_engine_memory_arena_allocator_create(
            ifb_engine_core_memory.region,
            "ENGINE CORE ARENA 16KB",
            IFB_MATH_KILOBYTES(16),
            IFB_MATH_MEGABYTES(8));

    ifb_assert(arena_allocator.arena_16kb);

    return(arena_allocator);
}

internal IFBEngineCoreMemoryPtr
ifb_engine_core_memory_create_and_initialize() {

    ifb_engine_core_memory = {0};

    //create core memory region
    ifb_engine_core_memory.region = 
        ifb_engine_memory_region_create(
            IFB_ENGINE_CORE_MEMORY_REGION_SIZE,
            "ENGINE CORE");
    ifb_assert(ifb_engine_core_memory.region);

    //create the allocators
    ifb_engine_core_memory.allocators.arena = ifb_engine_core_memory_arena_allocator_create();
    ifb_engine_core_memory.allocators.frame = ifb_engine_core_memory_frame_allocator_create();

    return(&ifb_engine_core_memory);
}

internal void
ifb_engine_core_memory_frame_reset() {

    IFBEngineCoreMemoryFrameAllocator& frame_allocator = ifb_engine_core_memory.allocators.frame;

    ifb_engine_memory_arena_clear(frame_allocator.arena);
}

internal memory
ifb_engine_core_memory_frame_bytes_push(
    u64 size) {

    memory frame_memory =
        ifb_engine_memory_arena_bytes_push(
            ifb_engine_core_memory.allocators.frame.arena,
            size);

    ifb_assert(frame_memory);

    ifb_engine_core_memory.allocators.frame.stack_ptr = frame_memory;

    return(frame_memory);
}

internal memory
ifb_engine_memory_frame_bytes_pop(
    u64 size) {

    memory frame_memory =
        ifb_engine_memory_arena_bytes_pop(
            ifb_engine_core_memory.allocators.frame.arena,
            size);

    ifb_assert(frame_memory);

    ifb_engine_core_memory.allocators.frame.stack_ptr = frame_memory;

    return(frame_memory);
}


internal IFBEngineMemoryArenaPtr
ifb_engine_core_memory_arena_reserve_16kb() {

    IFBEngineMemoryArenaPtr arena_16kb = 
        ifb_engine_memory_arena_reserve(
            ifb_engine_core_memory.allocators.arena.arena_16kb);

    ifb_assert(arena_16kb);

    return(arena_16kb);
}