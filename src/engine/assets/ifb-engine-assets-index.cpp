#pragma once

#include "ifb-engine-assets.hpp"
#include "ifb-engine-assets-internal.hpp"

internal IFBEngineAssetsIndexTable* 
ifb_engine_assets::index_table_commit(
    const size_t index_count) {

    //get the next arena
    IFBEngineMemoryArena index_arena = ifb_engine_assets::memory_arena_index_commit();
    ifb_assert(index_arena);

    //calculate sizes
    const size_t index_table_size = sizeof(IFBEngineAssetsIndexTable);
    const size_t index_array_size = sizeof(IFBEngineAssetsIndex) * index_count;

    //allocate memory    
    IFBEngineAssetsIndexTable* index_table = ifb_engine_memory::arena_push_bytes(index_table_size);
    IFBEngineAssetsIndex*      index_array = ifb_engine_memory::arena_push_bytes(index_array_size);

    //initialize struct and return    
    index_table->arena       = index_arena;
    index_table->index_count = index_count;
    index_table->index_array = index_array;

    return(index_table);
}

internal void                       
ifb_engine_assets::index_table_decommit(
    IFBEngineAssetsIndexTable* index_table) {

    ifb_assert(index_table);

    ifb_engine_memory::arena_decommit(index_table->arena);
}