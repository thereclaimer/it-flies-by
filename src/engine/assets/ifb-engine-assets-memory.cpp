#include "ifb-engine-assets-new.hpp"
#include "ifb-engine-assets-internal.hpp"

internal void
ifb_engine_assets::memory_reserve(
    void) {

    //get the memory from the context
    IFBEngineAssetsContext& context      = context_get();
    IFBEngineAssetsMemory&  asset_memory = context.memory;

    const size_t asset_memory_size    = ifb_engine_memory_megabytes(64);
    const size_t asset_allocator_size = ifb_engine_memory_megabytes(64);
    const size_t asset_arena_size     = ifb_engine_memory_kilobytes(64);

    //make the reservation
    asset_memory.reservation = 
        ifb_engine_memory::reserve_memory(
            "ASSETS",
            asset_memory_size,
            IFBEngineMemoryPageType_Small);
    ifb_assert(asset_memory.reservation);

    //create the region for the allocator
    asset_memory.block_allocator.block_size = asset_arena_size;
    asset_memory.block_allocator.blocks     = NULL;
    asset_memory.block_allocator.total_size =  asset_allocator_size;
    asset_memory.block_allocator.region     = 
        ifb_engine_memory::region_create(
            asset_memory.reservation,
            "ASSET BLOCK ALLOCATOR",
            asset_allocator_size,
            asset_arena_size);
    ifb_assert(asset_memory.block_allocator.region);
}

internal void
ifb_engine_assets::memory_release(
    void) {

    IFBEngineAssetsContext& context = context_get();
    ifb_engine_memory::release_memory(context.memory.reservation);
}

  
internal const IFBEngineAssetsMemoryBlock* 
ifb_engine_assets::memory_block_commit(
    const size_t size) {

    IFBEngineAssetsContext& context                       = context_get();
    IFBEngineAssetsMemoryBlockAllocator&  block_allocator = context.memory.block_allocator;

    ifb_assert(size <= block_allocator.block_size);

    //get an arena
    IFBEngineMemoryArena arena = ifb_engine_memory::arena_commit(block_allocator.region);
    ifb_assert(arena);

    //reserve the block
    IFBEngineAssetsMemoryBlock_Impl* memory_block = 
        (IFBEngineAssetsMemoryBlock_Impl*)ifb_engine_memory::arena_push_bytes(
            sizeof(IFBEngineAssetsMemoryBlock));
    
    //reserve space for the asset
    memory asset_memory = ifb_engine_memory::arena_push_bytes(size);

    //initialize the block and return
    memory_block->arena    = arena;
    memory_block->size     = size;
    memory_block->next     = block_allocator.blocks;
    memory_block->previous = NULL;
    block_allocator.blocks->previous = memory_block;
    block_allocator.blocks = memory_block;

    return(memory_block);    
}

internal void 
ifb_engine_assets::memory_block_decommit (
    const IFBEngineAssetsMemoryBlock* block) {

    IFBEngineAssetsMemoryBlock_Impl* block_impl = (IFBEngineAssetsMemoryBlock_Impl*)block;

    ifb_assert(block_impl && block_impl->arena);

    //update the list
    IFBEngineAssetsMemoryBlock_Impl* previous = block_impl->previous;
    IFBEngineAssetsMemoryBlock_Impl* next     = block_impl->next;

    if (previous) {
        previous->next = next;
    }
    if (next) {
        next->previous = previous;
    }

    //decommit the arena
    ifb_engine_memory::arena_decommit(block_impl->arena);
} 