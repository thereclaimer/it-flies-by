#pragma once

#include "ifb-engine-assets.hpp"

global IFBEngineAssets ifb_engine_assets = {0};

internal u32
ifb_engine_assets_file_indexes_count(
    const handle asset_handle) {
    
    char header_buffer[7] = {0};

    ifb_engine_platform_file_read(
        asset_handle,
        0,
        7,
        (memory)header_buffer,
        false
    );

    ifb_assert(
        header_buffer[0] == 'I' &&
        header_buffer[1] == 'F' &&
        header_buffer[2] == 'B'
    );

    u16 indexes_count = *(u32*)&header_buffer[3];

    return(indexes_count);
}

internal void
ifb_engine_assets_file_load_all() {

    for (
        u32 asset_index = 0;
        asset_index < IFBEngineAssetFileId_count;
        ++asset_index) {


        //get the stuff we need
        const char*             asset_file_path  = ITFLIESBY_ENGINE_ASSETS_FILE_PATHS[asset_index];
        IFBEngineMemoryArenaPtr asset_file_arena = ifb_engine_assets.memory.array[asset_index];
        IFBEngineAssetFile&     asset_file       = ifb_engine_assets.files.array[asset_index]; 

        //make sure our arena is initialized
        ifb_assert(asset_file_arena);

        //open the file
        asset_file.file_handle = 
            ifb_engine_platform_file_open(
                asset_file_path,
                false);

        ifb_assert(asset_file.file_handle);

        //get the number of indexes
        asset_file.indexes_count = ifb_engine_assets_file_indexes_count(asset_file.file_handle); 
        ifb_assert(asset_file.indexes_count > 0);
    
        //allocate space for the indexes
        u64 index_allocation_size = ifb_engine_asset_indexes_allocation_size(asset_file.indexes_count);

        memory index_memory = 
            ifb_engine_memory_arena_bytes_push(
                asset_file_arena,
                index_allocation_size);
    
        ifb_assert(index_memory);

        //finally read the data
        ifb_engine_platform_file_read(
            asset_file.file_handle,
            7,
            index_allocation_size,
            index_memory,
            false);

        asset_file.indexes = (IFBEngineAssetIndexPtr)index_memory;
    }
}

internal IFBEngineAssetsPtr
ifb_engine_assets_create_and_initialize() {

    IFBEngineAssetMemory& asset_memory = ifb_engine_assets.memory;
    
    asset_memory.arena_shader_64kb = ifb_engine_memory_arena_reserve_64kb(IFB_ENGINE_ASSET_ARENA_TAG_SHADER);
    asset_memory.arena_image_64mb  = ifb_engine_memory_arena_reserve_64mb(IFB_ENGINE_ASSET_ARENA_TAG_IMAGE);

    ifb_assert(asset_memory.arena_shader_64kb);
    ifb_assert(asset_memory.arena_image_64mb);

    return(&ifb_engine_assets);
}