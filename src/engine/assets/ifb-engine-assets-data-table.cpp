#pragma once

#include "ifb-engine-assets.hpp"

global IFBEngineAssetDataTable ifb_engine_asset_data_table;

internal IFBEngineAssetDataTablePtr
ifb_engine_assets_data_table_create_and_initialize() {

    ifb_engine_asset_data_table = {0};

    return(&ifb_engine_asset_data_table);
}

internal IFBEngineAssetData
ifb_engine_assets_data_table_asset_load(
    IFBEngineAssetsType asset_type,
    u16                 asset_id) {

    //get the data table
    IFBEngineAssetMemoryBlockPtr* asset_memory_blocks = NULL;

    switch (asset_type) {
        case IFBEngineAssetsType_Shader: asset_memory_blocks = ifb_engine_asset_data_table.shader; break;
        case IFBEngineAssetsType_Image:  asset_memory_blocks = ifb_engine_asset_data_table.image;  break;
        default: break;
    }

    ifb_assert(asset_memory_blocks);

    //get the asset block pointer
    IFBEngineAssetMemoryBlockPtr asset_memory_block = asset_memory_blocks[asset_id];

    //if its null, load it
    if (!asset_memory_block) {
        
        asset_memory_block = 
            ifb_engine_assets_files_asset_allocate_and_read(
                asset_type,
                asset_id);

        asset_memory_blocks[asset_id] = asset_memory_block; 
    }

    //we should ALWAYS have asset data at this point
    ifb_assert(asset_memory_block);

    //initialize the result
    IFBEngineAssetData asset_data = {0};
    asset_data.buffer            = asset_memory_block->memory;
    asset_data.buffer_size_bytes = asset_memory_block->size;
    asset_data.index             = asset_id;
    asset_data.type              = asset_type;

    return(asset_data);
}

internal void
ifb_engine_assets_data_table_asset_unload(
    IFBEngineAssetsType asset_type,
    u16                 asset_id) {

    //get the data table
    IFBEngineAssetMemoryBlockPtr* asset_memory_blocks = NULL;

    switch (asset_type) {
        case IFBEngineAssetsType_Shader: asset_memory_blocks = ifb_engine_asset_data_table.shader; break;
        case IFBEngineAssetsType_Image:  asset_memory_blocks = ifb_engine_asset_data_table.image;  break;
        default: break;
    }

    ifb_assert(asset_memory_blocks);

    //get the asset block pointer
    IFBEngineAssetMemoryBlockPtr asset_memory_block = asset_memory_blocks[asset_id];

    //sanity check, we shouldnt be freeing something occupied
    ifb_assert(asset_memory_block);
    
    //free the block
    ifb_engine_assets_files_asset_free(asset_memory_block);
}