#pragma once

#include "ifb-engine-assets.hpp"

global IFBEngineAssetsMemory ifb_engine_asset_memory;

IFBEngineAssetMemoryArenaAllocator
ifb_engine_assets_memory_allocator_arena_create() {

    IFBEngineAssetMemoryArenaAllocator arena_allocator = {0}; 

    arena_allocator.arena_8KB = 
        ifb_engine_memory_arena_allocator_create(
            ifb_engine_asset_memory.region, // region
            "ASSET BLOCK 8KB",              // tag
            IFB_MATH_KILOBYTES(8),          // arena size
            IFB_MATH_MEGABYTES(8));         // allocator size
    ifb_assert(arena_allocator.arena_8KB);
    
    arena_allocator.arena_64KB = 
        ifb_engine_memory_arena_allocator_create(
            ifb_engine_asset_memory.region, // region
            "ASSET BLOCK 64KB",             // tag
            IFB_MATH_KILOBYTES(64),         // arena size
            IFB_MATH_MEGABYTES(8));         // allocator size
    ifb_assert(arena_allocator.arena_64KB);
    
    arena_allocator.arena_128KB = 
        ifb_engine_memory_arena_allocator_create(
            ifb_engine_asset_memory.region, // region
            "ASSET BLOCK 128KB",            // tag
            IFB_MATH_KILOBYTES(128),        // arena size
            IFB_MATH_MEGABYTES(8));         // allocator size
    ifb_assert(arena_allocator.arena_128KB);
    
    arena_allocator.arena_1MB = 
        ifb_engine_memory_arena_allocator_create(
            ifb_engine_asset_memory.region, // region
            "ASSET BLOCK 1MB",              // tag
            IFB_MATH_MEGABYTES(1),          // arena size
            IFB_MATH_MEGABYTES(8));         // allocator size
    ifb_assert(arena_allocator.arena_1MB);
    
    arena_allocator.arena_4MB = 
        ifb_engine_memory_arena_allocator_create(
            ifb_engine_asset_memory.region, // region
            "ASSET BLOCK 4MB",              // tag
            IFB_MATH_MEGABYTES(4),          // arena size
            IFB_MATH_MEGABYTES(16));        // allocator size
    ifb_assert(arena_allocator.arena_4MB);

    return(arena_allocator);
}

IFBEngineAssetMemoryBlockAllocator
ifb_engine_assets_memory_allocator_block_create() {

    IFBEngineAssetMemoryBlockAllocator block_allocator = {0};


    return(block_allocator);
}

IFBEngineAssetMemoryIndexAllocator
ifb_engine_assets_memory_allocator_index_create() {

    IFBEngineAssetMemoryIndexAllocator index_allocator = {0};

    index_allocator.arena = 
        ifb_engine_memory_arena_reserve(
            ifb_engine_asset_memory.allocators.arena.arena_8KB);

    ifb_assert(index_allocator.arena);

    index_allocator.stack_ptr = NULL;

    return(index_allocator);
}

IFBEngineAssetsMemoryPtr
ifb_engine_assets_memory_create_and_initialize() {

    ifb_engine_asset_memory = {0};

    //create the region
    ifb_engine_asset_memory.region = 
        ifb_engine_memory_region_create(
            IFB_ENGINE_ASSET_MEMORY_REGION_SIZE,
            "ASSET REGION");

    ifb_assert(ifb_engine_asset_memory.region);

    //create the allocators
    ifb_engine_asset_memory.allocators.arena = ifb_engine_assets_memory_allocator_arena_create();
    ifb_engine_asset_memory.allocators.block = ifb_engine_assets_memory_allocator_block_create();
    ifb_engine_asset_memory.allocators.index = ifb_engine_assets_memory_allocator_index_create();

    return(&ifb_engine_asset_memory);
}

internal IFBEngineAssetMemoryBlockPtr
ifb_engine_assets_memory_block_allocate(
    const u64 size) {

    IFBEngineAssetMemoryArenaAllocator& arena_allocator_ref = ifb_engine_asset_memory.allocators.arena;
    IFBEngineAssetMemoryBlockAllocator& block_allocator_ref = ifb_engine_asset_memory.allocators.block;

    IFBEngineMemoryArenaAllocatorPtr allocator  = NULL;
    IFBEngineAssetMemoryBlockPtr     block_list = NULL; 

    u64 size_actual = sizeof(IFBEngineAssetMemoryBlock) + size;

    if ((size_actual < IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_8KB)) {
        allocator = arena_allocator_ref.arena_8KB;
        block_list = block_allocator_ref.blocks_8KB;
    }

    else if ((size_actual >= IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_8KB) && (size_actual < IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_64KB)) {
        allocator = arena_allocator_ref.arena_64KB;
        block_list = block_allocator_ref.blocks_64KB;
    }

    else if ((size_actual >=IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_64KB) && (size_actual < IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_128KB)) {
        allocator = arena_allocator_ref.arena_128KB;
        block_list = block_allocator_ref.blocks_128KB;
    }

    else if ((size_actual >=IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_128KB) && (size_actual < IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_1MB)) {
        allocator = arena_allocator_ref.arena_1MB;
        block_list = block_allocator_ref.blocks_1MB;
    }

    else if ((size_actual >=IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_1MB) && (size_actual < IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_4MB)) {
        allocator = arena_allocator_ref.arena_4MB;
        block_list = block_allocator_ref.blocks_4MB;
    }

    else if (size_actual >=IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_4MB) {
        allocator  = NULL; 
        block_list = NULL;
    }

    //make sure we have an allocator
    ifb_assert(allocator);

    //reserve an arena
    IFBEngineMemoryArenaPtr asset_arena = ifb_engine_memory_arena_reserve(allocator);
    ifb_assert(asset_arena);

    //allocate space for the block
    memory asset_memory = 
        ifb_engine_memory_arena_bytes_push(
            asset_arena,
            size_actual);

    ifb_assert(asset_memory);

    IFBEngineAssetMemoryBlockPtr asset_block = (IFBEngineAssetMemoryBlockPtr)asset_memory;

    //push this new block to the front of this allocators blocks    
    if (block_list) {
        block_list->previous = asset_block;
    }

    asset_block->arena    = asset_arena; 
    asset_block->memory   = asset_memory + size;
    asset_block->size     = size;
    asset_block->next     = block_list;
    asset_block->previous = NULL;

    block_list = asset_block;

    return(asset_block);
}

internal void
ifb_engine_assets_memory_block_free(
    IFBEngineAssetMemoryBlockPtr block) {

    if (!block) {
        return;
    }
    
    IFBEngineAssetMemoryBlockPtr block_previous = block->previous;
    IFBEngineAssetMemoryBlockPtr block_next     = block->next;

    if (block_previous) {
        block_previous->next = block_next;
    }

    if (block_next) {
        block_next->previous = block_previous;
    }

    ifb_engine_memory_arena_release(block->arena);

    memset(
        block,
        0,
        sizeof(IFBEngineAssetMemoryBlock) + block->size);

    block = NULL;
}

internal memory
ifb_engine_assets_memory_block_bytes_push(
    IFBEngineAssetMemoryBlockPtr block,
    u32                          size) {

    memory asset_block_memory = 
        ifb_engine_memory_arena_bytes_push(
            block->arena,
            size);

    ifb_assert(asset_block_memory);

    return(asset_block_memory);
}

internal void
ifb_engine_assets_memory_block_bytes_pop(
    IFBEngineAssetMemoryBlockPtr block,
    u32                          size) {

    ifb_engine_memory_arena_bytes_pop(
        block->arena,
        size
    );
}

IFBEngineAssetTableIndexPtr 
ifb_engine_assets_memory_index_array_push(
    u32 index_count) {

    u32 allocation_size = sizeof(IFBEngineAssetTableIndex) * index_count;

    IFBEngineAssetMemoryIndexAllocator& index_allocator = ifb_engine_asset_memory.allocators.index; 

    index_allocator.stack_ptr = 
        ifb_engine_memory_arena_bytes_push(
            index_allocator.arena,
            allocation_size);

    ifb_assert(index_allocator.stack_ptr);

    IFBEngineAssetTableIndexPtr index_arrray = 
        (IFBEngineAssetTableIndexPtr)index_allocator.stack_ptr;

    return(index_arrray);
}

IFBEngineAssetMemoryBlockPtr*
ifb_engine_assets_memory_index_block_array_push(
    u32 index_count) {

    u32 allocation_size = sizeof(IFBEngineAssetMemoryBlockPtr) * index_count;

    IFBEngineAssetMemoryIndexAllocator& index_allocator = ifb_engine_asset_memory.allocators.index; 

    index_allocator.stack_ptr = 
        ifb_engine_memory_arena_bytes_push(
            index_allocator.arena,
            allocation_size);

    ifb_assert(index_allocator.stack_ptr);

    IFBEngineAssetMemoryBlockPtr* index_block_array = 
        (IFBEngineAssetMemoryBlockPtr*)index_allocator.stack_ptr;

    return(index_block_array);
}