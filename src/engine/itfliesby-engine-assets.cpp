#pragma once

#include "itfliesby-engine.hpp"

internal u32
itfliesby_engine_assets_file_header_read(
    const handle asset_handle,
    const memory asset_header_memory) {

    //TODO: there's no overflow protection here

    char header_buffer[8] = {0};

    platform_api.file_read(
        asset_handle,
        0,
        8,
        (memory)header_buffer
    );

    ITFLIESBY_ASSERT(
        header_buffer[0] == 'I' &&
        header_buffer[1] == 'F' &&
        header_buffer[2] == 'B'
    );

    u16 num_indexes = *(u16*)&header_buffer[3];

    u32 index_data_size = sizeof(ItfliesbyEngineAssetsFileindex) * num_indexes;

    platform_api.file_read(
        asset_handle,
        8,
        index_data_size,
        asset_header_memory
    );

    return(num_indexes);
}


internal void
itfliesby_engine_asset_indexes_load(
    ItfliesbyEngineAssetsFileHandles*       asset_file_handles,
    ItfliesbyEngineAssetsFileIndexStore*    asset_index_store,
    itfliesby_engine_assets_allocator_index index_allocator) {

    handle*                                   asset_file_handle_array = asset_file_handles->handles.array;
    ItfliesbyEngineAssetsFileIndexCollection* index_collection;
    handle                                    asset_file_handle;

    for (
        s32 file_index = 0;
        file_index < ITFLIESBY_ASSETS_FILE_ID_COUNT;
        ++file_index) {

        //initialize our stuff
        asset_file_handle = asset_file_handle_array[file_index]; 
        index_collection = &asset_index_store->array[file_index];
        
        //if there is no memory, we have indexes to load
        if (index_collection->index_memory) {
            continue;
        }

        //allocate index memory
        index_collection->index_memory = itfliesby_memory_allocator_block_allocate(index_allocator);
        ITFLIESBY_ASSERT(index_collection->index_memory);

        //load the indexes
        index_collection->index_count =
            itfliesby_engine_assets_file_header_read(
                asset_file_handle,
                index_collection->index_memory
        );
    }
}

internal void 
itfliesby_engine_assets_init(
    ItfliesbyEngineAssets* assets,
    itfliesby_memory_arena arena) {

    *assets = {0};
    ItfliesbyEngineAssetsFileHandles* file_handles = &assets->file_handles;

    //the assets are starting out in the unloaded state
    for (
        s32 file_index = 0;
        file_index < ITFLIESBY_ASSETS_FILE_ID_COUNT;
        ++file_index) {

        file_handles->unloaded_files[file_index] = (ItfliesbyEngineAssetsFileId)file_index;
        ++file_handles->unloaded_files_count;
    }

    //initialize memory
    assets->memory.partition = itfliesby_memory_partition_create(arena,"ENGINE ASSETS PRTN",  ITFLIESBY_ENGINE_ASSETS_MEMORY_PARTITION_SIZE);
    ITFLIESBY_ASSERT(assets->memory.partition);    

    //allocators
    assets->memory.index_allocator      = itfliesby_memory_allocator_block_create(assets->memory.partition,"ASSET INDEX ALCTR",ITFLIESBY_ENGINE_ASSETS_MEMORY_BLOCK_SIZE_INDEX,     ITFLIESBY_ASSETS_FILE_ID_COUNT);
    assets->memory.asset_data_allocator = itfliesby_memory_allocator_block_create(assets->memory.partition,"ASSET DATA ALCTR", ITFLIESBY_ENGINE_ASSETS_MEMORY_BLOCK_SIZE_ASSET_DATA,ITFLIESBY_ASSETS_FILE_ID_COUNT);
    ITFLIESBY_ASSERT(assets->memory.index_allocator);    
    ITFLIESBY_ASSERT(assets->memory.asset_data_allocator);    
    
}

internal void
itfliesby_engine_assets_update(
    ItfliesbyEngineAssets* assets) {

    ItfliesbyEngineAssetsFileHandles*    file_handles = &assets->file_handles;
    ItfliesbyEngineAssetsFileIndexStore* index_store  = &assets->file_index_store;
    ItfliesbyEngineAssetsMemory*         asset_memory = &assets->memory;

    itfliesby_engine_assets_file_handles_load(file_handles);

    itfliesby_engine_asset_indexes_load(
        file_handles,
        index_store,
        asset_memory->index_allocator
    );
}

internal void
itfliesby_engine_assets_file_handles_load(
    ItfliesbyEngineAssetsFileHandles* file_handles) {

    //cache the stuff we'll need
    ItfliesbyEngineAssetsFileId* unloaded_files = file_handles->unloaded_files;
    ItfliesbyEngineAssetsFileId* missing_files  = file_handles->missing_files;
    handle* file_handle_array                   = file_handles->handles.array;
    u32 unloaded_files_count                    = file_handles->unloaded_files_count;
    u32 missing_files_count                     = 0;

    ItfliesbyEngineAssetsFileId asset_id_to_load;
    for (
        u32 unloaded_file_index = 0;
        unloaded_file_index < unloaded_files_count;
        ++unloaded_file_index) {

        //get the next asset id
        asset_id_to_load = unloaded_files[unloaded_file_index];

        //get the file handle
        file_handle_array[asset_id_to_load] = 
            platform_api.file_open(
                (str)ITFLIESBY_ENGINE_ASSETS_FILE_PATHS[asset_id_to_load],
                false
            );
        
        //if the file failed to load, add it to the missing files table
        if (!file_handle_array[asset_id_to_load]) {
            missing_files[missing_files_count];
            ++missing_files_count;
        }
    }

    file_handles->unloaded_files_count = 0;
    file_handles->missing_files_count  = missing_files_count;

    if (missing_files_count > 0) {
        //TODO: error message
        ITFLIESBY_PANIC();
    }
}