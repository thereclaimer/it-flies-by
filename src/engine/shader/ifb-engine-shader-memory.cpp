#pragma once

#include "ifb-engine-shader.hpp"

internal void
ifb_engine_shader_memory_initialize(
    IFBEngineShaderMemory& shader_memory) {

    //create the memory region
    shader_memory.region = 
        ifb_engine_memory_region_create(
            IFB_ENGINE_SHADER_MEMORY_SIZE,
            "SHADERS");

    ifb_assert(shader_memory.region);

    //create the allocators
    shader_memory.allocators.arena_4kb = 
        ifb_engine_memory_arena_allocator_create(
            shader_memory.region,
            "SHADER ARENA 4KB",
            IFB_MATH_KILOBYTES(4),
            IFB_MATH_MEGABYTES(8));

    ifb_assert(shader_memory.allocators.arena_4kb);

    //create the arenas
    shader_memory.arenas.shader_program_4kb = ifb_engine_memory_arena_reserve(shader_memory.allocators.arena_4kb);            
    shader_memory.arenas.shader_uniform_4kb = ifb_engine_memory_arena_reserve(shader_memory.allocators.arena_4kb);            

    ifb_assert(shader_memory.arenas.shader_program_4kb);
    ifb_assert(shader_memory.arenas.shader_uniform_4kb);
}

IFBEngineOpenglShaderProgram*
ifb_engine_shader_program_allocate(
    const IFBEngineShaderMemory& shader_memory,
    const u32                    count) {

    u64 allocation_size = sizeof(IFBEngineOpenglShaderProgram) * count; 

    memory shader_program_memory = 
        ifb_engine_memory_arena_bytes_push(allocation_size);

    ifb_assert(shader_program_memory);

    IFBEngineOpenglShaderProgram* shader_programs =
        (IFBEngineOpenglShaderProgram*)shader_program_memory;

    return(shader_programs);
}

IFBEngineOpenglShaderUniform*
ifb_engine_shader_uniform_allocate(
    u32 count) {

}

