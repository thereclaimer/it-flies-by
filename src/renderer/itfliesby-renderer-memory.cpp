#pragma once

#include "itfliesby-renderer.hpp"

global ItfliesbyRendererMemory renderer_memory;

internal void
itfliesby_renderer_memory_create(
    memory core_memory,
    u64    core_memory_size_bytes) {

    ifb_assert(
        core_memory && 
        core_memory_size_bytes == ITFLIESBY_RENDERER_MEMORY_SIZE_BYTES);

    //initialize the arena
    renderer_memory = {0};
    renderer_memory.arena = itfliesby_memory_arena_create("RENDERER ARENA",core_memory_size_bytes,core_memory);

    //initialize partitions
    renderer_memory.partitions.core = 
        itfliesby_memory_partition_create(
            renderer_memory.arena,
            "RENDERER CORE PRTN",
            ITFLIESBY_RENDERER_MEMORY_PARTITION_CORE_SIZE_BYTES);

    renderer_memory.partitions.uniform_buffers = 
        itfliesby_memory_partition_create(
            renderer_memory.arena,
            "RENDERER UNIFORM PRTN",
            ITFLIESBY_RENDERER_MEMORY_PARTITION_UNIFORM_BUFFER_BYTES);

    ifb_assert(renderer_memory.partitions.core);
    ifb_assert(renderer_memory.partitions.uniform_buffers);

    //initialize allocators
    renderer_memory.allocators.core_system_allocator = 
        itfliesby_memory_allocator_linear_create(
            renderer_memory.partitions.core,
            "RENDERER CORE ALCTR",
            ITFLIESBY_RENDERER_MEMORY_ALLOCATOR_CORE_SYSTEM_SIZE_BYTES);

    renderer_memory.allocators.uniform_buffer_allocator = 
        itfliesby_memory_allocator_linear_create(
            renderer_memory.partitions.uniform_buffers,
            "RENDERER UNIFORM ALCTR",
            ITFLIESBY_RENDERER_MEMORY_ALLOCATOR_UNIFORM_BUFFER_BYTES);

    ifb_assert(renderer_memory.allocators.core_system_allocator);
    ifb_assert(renderer_memory.allocators.uniform_buffer_allocator);
}

internal ItfliesbyRenderer*
itfliesby_renderer_memory_allocate_core() {

    ItfliesbyRenderer* renderer = 
        (ItfliesbyRenderer*)itfliesby_memory_allocator_linear_allocate(
            renderer_memory.allocators.core_system_allocator,
            sizeof(ItfliesbyRenderer)
        );

    ifb_assert(renderer);

    *renderer = {0};

    return(renderer);
}

internal GLbyte*
itfliesby_renderer_memory_allocate_uniform_buffer_memory(
    u32 uniform_size_bytes,
    u32 num_elements) {

    u32 uniform_buffer_size_bytes = uniform_size_bytes * num_elements; 

    GLbyte* uniform_buffer = 
        (GLbyte*)itfliesby_memory_allocator_linear_allocate(
            renderer_memory.allocators.uniform_buffer_allocator,
            uniform_buffer_size_bytes);

    ifb_assert(uniform_buffer);

    return(uniform_buffer);
}