#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRendererMemory ifb_engine_renderer_memory;

internal IFBEngineRendererMemoryAllocatorShader
ifb_engine_renderer_memory_allocator_shader_create() {

    IFBEngineMemoryArenaPtr arena = 
        ifb_engine_renderer_memory_arena_reserve(IFBEngineAssetMemoryArenaSize_8KB);
    ifb_assert(arena);

    IFBEngineRendererMemoryAllocatorShader shader_allocator = {0};    
    shader_allocator.arena_8k             = arena;
    shader_allocator.shaders_count        = 0;
    shader_allocator.stack_ptr            = NULL;

    return(shader_allocator);
}

internal IFBEngineRendererMemoryAllocatorUniform
ifb_engine_renderer_memory_allocator_uniform_create() {

    IFBEngineMemoryArenaPtr arena = 
        ifb_engine_renderer_memory_arena_reserve(IFBEngineAssetMemoryArenaSize_8KB);
    ifb_assert(arena);

    IFBEngineRendererMemoryAllocatorUniform uniform_allocator = {0};    
    uniform_allocator.arena_8k             = arena;
    uniform_allocator.uniforms_count_total = 0;
    uniform_allocator.stack_ptr            = NULL;

    return(uniform_allocator);
}

internal IFBEngineRendererMemoryAllocatorArena
ifb_engine_renderer_memory_allocator_arena_create() {

    IFBEngineRendererMemoryAllocatorArena arena_allocators = {0};
    
    IFBEngineMemoryRegionPtr region = ifb_engine_renderer_memory.region;

    arena_allocators.arena_256B = 
        ifb_engine_memory_arena_allocator_create(region, 
        "RENDERER ARENA 256B",
        IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_256B,
        IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_256B);
    
    ifb_assert(arena_allocators.arena_256B);

    arena_allocators.arena_512B = 
        ifb_engine_memory_arena_allocator_create(region, 
        "RENDERER ARENA 512B",
        IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_512B,
        IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_512B);
    
    ifb_assert(arena_allocators.arena_512B);

    arena_allocators.arena_1KB  =
         ifb_engine_memory_arena_allocator_create(region, 
        "RENDERER ARENA 1KB",
         IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_1KB,
         IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_1KB);
        
    ifb_assert(arena_allocators.arena_1KB);

    arena_allocators.arena_8KB  =
         ifb_engine_memory_arena_allocator_create(region, 
        "RENDERER ARENA 8KB",
         IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_8KB,
         IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_8KB);
        
    ifb_assert(arena_allocators.arena_8KB);

    arena_allocators.arena_16KB = 
        ifb_engine_memory_arena_allocator_create(region, 
        "RENDERER ARENA 16KB",
        IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_16KB,
        IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_16KB);
    
    ifb_assert(arena_allocators.arena_16KB);

    return(arena_allocators);
}

internal IFBEngineRendererMemoryPtr
ifb_engine_renderer_memory_create_and_initialize() {
    
    ifb_engine_renderer_memory = {0};    

    //create the region
    ifb_engine_renderer_memory.region = 
        ifb_engine_memory_region_create(
            IFB_ENGINE_RENDERER_MEMORY_REGION_SIZE,
            "ENGINE RENDERER");

    ifb_assert(ifb_engine_renderer_memory.region);

    //create allocators
    ifb_engine_renderer_memory.allocators.arena   = ifb_engine_renderer_memory_allocator_arena_create(); 
    ifb_engine_renderer_memory.allocators.uniform = ifb_engine_renderer_memory_allocator_uniform_create(); 
    ifb_engine_renderer_memory.allocators.shader  = ifb_engine_renderer_memory_allocator_shader_create(); 

    return(&ifb_engine_renderer_memory);
}

internal IFBEngineMemoryArenaPtr
ifb_engine_renderer_memory_arena_reserve_auto(
    u64 size_needed) {

    ifb_assert(size_needed > 0);

    //get the appropriate allocator
    IFBEngineMemoryArenaAllocatorPtr arena_allocator;

    if (size_needed <= IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_256B) {
        arena_allocator = ifb_engine_renderer_memory.allocators.arena.arena_256B; 
    }
    else if (
        size_needed >  IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_256B && 
        size_needed <= IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_512B) {

        arena_allocator = ifb_engine_renderer_memory.allocators.arena.arena_512B; 
    }
    else if (
        size_needed >  IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_512B && 
        size_needed <= IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_1KB) {

        arena_allocator = ifb_engine_renderer_memory.allocators.arena.arena_1KB; 
    }
    else if (
        size_needed >  IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_1KB && 
        size_needed <= IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_8KB) {

        arena_allocator = ifb_engine_renderer_memory.allocators.arena.arena_8KB; 
    }
    else if (
        size_needed >  IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_8KB && 
        size_needed <= IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_16KB) {

        arena_allocator = ifb_engine_renderer_memory.allocators.arena.arena_16KB; 
    }
    else {
        arena_allocator = NULL;
    }

    //sanity check
    ifb_assert(arena_allocator);

    //reserve the arena
    IFBEngineMemoryArenaPtr new_arena = 
        ifb_engine_memory_arena_reserve(arena_allocator);
    
    ifb_assert(new_arena);

    return(new_arena);

}

internal IFBEngineMemoryArenaPtr
ifb_engine_renderer_memory_arena_reserve(
    IFBEngineRendererMemoryArenaSize arena_size) {

    ifb_assert(
        arena_size > 0 &&
        arena_size < IFBEngineRendererMemoryArenaSize_Count);

    IFBEngineMemoryArenaAllocatorPtr arena_allocator = 
        ifb_engine_renderer_memory.allocators.arena.array[arena_size];

    ifb_assert(arena_allocator);

    IFBEngineMemoryArenaPtr new_arena = 
        ifb_engine_memory_arena_reserve(arena_allocator);
    
    ifb_assert(new_arena);

    return(new_arena);
}

internal void
ifb_engine_renderer_memory_arena_free(
    IFBEngineMemoryArenaPtr arena) {

    ifb_engine_memory_arena_release(arena);
}

internal IFBEngineRendererShaderUniformPtr
ifb_engine_renderer_memory_uniforms_push(
    u32 uniforms_count) {

    //validate the allocator
    IFBEngineRendererMemoryAllocatorUniform& uniform_allocator_ref = 
        ifb_engine_renderer_memory.allocators.uniform;

    ifb_assert(uniform_allocator_ref.arena_8k);

    //make the allocation
    u64 allocation_size = sizeof(IFBEngineRendererShaderUniform) * uniforms_count;
    
    uniform_allocator_ref.stack_ptr =     
        ifb_engine_memory_arena_bytes_push(
            uniform_allocator_ref.arena_8k,
            allocation_size        
        );

    ifb_assert(uniform_allocator_ref.stack_ptr);

    ++uniform_allocator_ref.uniforms_count_total;

    //return the uniforms
    IFBEngineRendererShaderUniformPtr uniforms = 
        (IFBEngineRendererShaderUniformPtr)uniform_allocator_ref.stack_ptr;

    return(uniforms);
}

internal IFBEngineRendererShaderPtr
ifb_engine_renderer_memory_allocator_shader_push() {

    //validate the allocator
    IFBEngineRendererMemoryAllocatorShader& shader_allocator_ref = 
        ifb_engine_renderer_memory.allocators.shader;    

    //make the allocation
    ifb_assert(shader_allocator_ref.arena_8k);

    u64 allocation_size = sizeof(IFBEngineRendererShader);

    shader_allocator_ref.stack_ptr = 
        ifb_engine_memory_arena_bytes_push(
            shader_allocator_ref.arena_8k,
            allocation_size        
        );

    ifb_assert(shader_allocator_ref.stack_ptr);

    ++shader_allocator_ref.shaders_count;

    IFBEngineRendererShaderPtr shader = 
            (IFBEngineRendererShaderPtr)shader_allocator_ref.stack_ptr;

    return(shader);
}

