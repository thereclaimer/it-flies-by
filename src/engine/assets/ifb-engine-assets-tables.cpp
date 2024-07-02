#pragma once

#include "ifb-engine-assets.hpp"

global IFBEngineAssetTables ifb_engine_asset_tables;

internal IFBEngineAssetTable
ifb_engine_assets_tables_table_create(ItfliesbyEngineAssetsType asset_type) {

    IFBEngineAssetTable asset_table = {0};

    //get the file name
    ifb_assert(
        asset_type > ItfliesbyEngineAssetsType_Invalid &&
        asset_type < ItfliesbyEngineAssetsType_Count);

    const char* file_name = IFB_ENGINE_ASSETS_FILE_PATHS[asset_type];

    //open the file
    handle asset_file_handle = ifb_engine_platform_file_open(file_name,false);
    ifb_assert(asset_file_handle);

    //get the index count
    char header_buffer[7] = {0};

    ifb_platform_api_file_read(
        asset_file_handle,
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

    u16 index_count = *(u32*)&header_buffer[3];

    //allocate space for the indexes
    IFBEngineAssetTableIndexPtr   index_array       = ifb_engine_assets_memory_index_array_push(index_count);
    IFBEngineAssetMemoryBlockPtr* index_block_array = ifb_engine_assets_memory_index_block_array_push(index_count);

    //sanity check
    ifb_assert(index_array);
    ifb_assert(index_block_array);

    //make sure everything is set everything to 0
    for (
        u32 index = 0;
        index < index_count;
        ++index) {

        index_array[index]       = {0};
        index_block_array[index] = NULL;
    }

    //initialize the table
    asset_table.asset_file_handle = asset_file_handle;
    asset_table.index_array       = index_array;
    asset_table.index_block_array = index_block_array;
    asset_table.index_count       = index_count;
    asset_table.type              = asset_type;

    return(asset_table);
}


internal IFBEngineAssetTablesPtr 
ifb_engine_assets_tables_create_and_initialize() {

    ifb_engine_asset_tables = {0};

    for (
        IFBEngineAssetType asset_type = 0;
        asset_type < ItfliesbyEngineAssetsType_Count;
        ++asset_type) {

        //get a reference to the current table
        IFBEngineAssetTable& asset_table_ref = ifb_engine_asset_tables.array[asset_type];
        
        //create the table based on the type
        asset_table_ref = ifb_engine_assets_tables_table_create(asset_type);        
    }

    return(&ifb_engine_asset_tables);
}