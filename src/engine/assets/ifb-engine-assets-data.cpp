#pragma once

#include "ifb-engine-assets.hpp"
#include "ifb-engine-assets-internal.hpp"

internal IFBEngineAssetsDataBlock_Impl* 
ifb_engine_assets::data_block_commit(
    const size_t data_size) {

    //get a data arena
    const IFBEngineMemoryArena data_arena =
        ifb_engine_assets::memory_arena_data_commit();
    ifb_assert(data_arena);

    //allocate space for the data block
    IFBEngineAssetsDataBlock_Impl* data_block = 
        (IFBEngineAssetsDataBlock_Impl*)ifb_engine_memory::arena_push_bytes(
            data_arena,
            sizeof(IFBEngineAssetsDataBlock_Impl));
    ifb_assert(data_block);
    
    memory data_start = 
        ifb_engine_memory::arena_push_bytes(
            data_arena,
            data_size);
    ifb_assert(data_start);

    //initialize the struct and return
    data_block->arena    = data_arena;
    data_block->file     = NULL;
    data_block->next     = NULL;
    data_block->previous = NULL;
    data_block->size     = data_size;
    data_block->index    = 0;
    data_block->start    = data_start;

    return(data_block);
}

internal void 
ifb_engine_assets::data_block_decommit(
    IFBEngineAssetsDataBlock_Impl* data_block) {

    ifb_engine_memory::arena_decommit(data_block->arena);
    data_block = NULL;
}