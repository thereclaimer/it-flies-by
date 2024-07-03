#pragma once

#include "ifb-engine-assets.hpp"

global IFBEngineAssetFiles ifb_engine_asset_files;

internal IFBEngineAssetFileIndexBuffer
ifb_engine_assets_file_index_buffer_allocate_and_read(
    handle asset_file_handle,
    u32    asset_file_index_count) {

    //get the total size of the indexes
    u32 index_buffer_size_bytes = ifb_engine_assets_files_index_size_byes(asset_file_index_count);

    //allocate space for the indexes
    memory asset_file_buffer =  ifb_engine_assets_memory_scratch_push(index_buffer_size_bytes);

    //read the indexes
    ifb_engine_platform_file_read(
        asset_file_handle,
        7,
        index_buffer_size_bytes,
        asset_file_buffer,
        false);

    //allocate space for our index buffer
    IFBEngineAssetsFileIndexPtr asset_file_index_buffer = 
        ifb_engine_assets_memory_index_buffer_push(asset_file_index_count);

    ifb_assert(asset_file_index_buffer);

    u32 asset_file_buffer_offset = 0; 

    for (
        u32 index = 0;
        index < asset_file_index_count;
        ++index) {

        //get the current index data
        memory asset_index_data = asset_file_buffer + asset_file_buffer_offset;

        //get the current index
        IFBEngineAssetsFileIndexPtr current_asset_index_ptr = &asset_file_index_buffer[index];  

        //tag
        memmove(
            (void*)current_asset_index_ptr->tag,
            (void*)&asset_index_data[asset_file_buffer_offset],
            32
        );

        //rest of the data
        memmove(
            (void*)((memory)current_asset_index_ptr + 32),
            (void*)&asset_index_data[asset_file_buffer_offset + 32],
            IFB_ENGINE_ASSETS_FILE_INDEX_SIZE_BYTES - 32
        );

        //update the offset
        asset_file_buffer_offset += IFB_ENGINE_ASSETS_FILE_INDEX_SIZE_BYTES;
    }

    //we don't need the file buffer anymore
    (void)ifb_engine_assets_memory_scratch_pop(index_buffer_size_bytes);

    //initialize the index buffer
    IFBEngineAssetFileIndexBuffer asset_index_buffer = {0};
    asset_index_buffer.indexes = asset_file_index_buffer;
    asset_index_buffer.count   = asset_file_index_count;

    return(asset_index_buffer);
}

internal u32
ifb_engine_assets_file_index_count(
    handle asset_file_handle) {

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

    return(index_count);
}

internal IFBEngineAssetFilesPtr
ifb_engine_asset_files_create_and_initialize() {

    ifb_engine_asset_files = {0};


    for (
        IFBEngineAssetType asset_file_type = 0;
        asset_file_type < IFBEngineAssetsType_Count;
        ++asset_file_type) {

        IFBEngineAssetFile& asset_file_ref = ifb_engine_asset_files.array[asset_file_type];
        
        //open the file
        const char* file_path = IFB_ENGINE_ASSETS_FILE_PATHS[asset_file_type];
        handle asset_file_handle = 
            ifb_platform_api_file_open(file_path,false);
        ifb_assert(asset_file_handle);

        //get the file size
        u64 asset_file_size_bytes = 
            ifb_platform_api_file_size(
                asset_file_handle,
                true);

        //get the information about the indexes
        u32 asset_file_index_count = ifb_engine_assets_file_index_count(asset_file_handle); 

        //the data starts after the header
        u64 asset_data_start = ifb_engine_assets_files_header_size_bytes(asset_file_index_count);

        //get the index buffer
        IFBEngineAssetFileIndexBuffer asset_index_buffer = 
            ifb_engine_assets_file_index_buffer_allocate_and_read(
                asset_file_handle,
                asset_file_index_count);

        //initialize the asset file
        asset_file_ref.file_handle      = asset_file_handle; 
        asset_file_ref.file_size_bytes  = asset_file_size_bytes; 
        asset_file_ref.file_type        = asset_file_type; 
        asset_file_ref.asset_data_start = asset_data_start; 
        asset_file_ref.file_data_buffer = NULL; 
        asset_file_ref.index_buffer     = asset_index_buffer;             
    }

    return(&ifb_engine_asset_files);
}


internal void
ifb_engine_asset_files_read(
    handle asset_file_handle,
    u64    asset_file_data_start,
    u64    asset_buffer_offset,
    u64    asset_buffer_size,
    memory asset_buffer) {

    ifb_assert(
        asset_file_handle && 
        asset_buffer      && 
        asset_buffer_size > 0);

    u64 file_offset = asset_file_data_start + asset_buffer_offset;

    ifb_platform_api_file_read(
        asset_file_handle,
        file_offset,
        asset_buffer_size,
        asset_buffer,
        false);
}

internal IFBEngineAssetMemoryBlockPtr 
ifb_engine_assets_files_asset_allocate_and_read(
    IFBEngineAssetsType asset_type,
    u16                 asset_id) {

    //get the file and index
    IFBEngineAssetFile&       asset_file_ref       = ifb_engine_asset_files.array[asset_type];
    IFBEngineAssetsFileIndex& asset_file_index_ref = asset_file_ref.index_buffer.indexes[asset_id];

    //allocate a block for the asset
    IFBEngineAssetMemoryBlockPtr asset_memory_block_ptr = 
        ifb_engine_assets_memory_block_allocate(asset_file_index_ref.allocation_size);    

    ifb_assert(asset_memory_block_ptr);

    //read the asset data from the file
    ifb_engine_asset_files_read(
        asset_file_ref.file_handle,
        asset_file_ref.asset_data_start,
        asset_file_index_ref.offset,
        asset_memory_block_ptr->size,
        asset_memory_block_ptr->memory);

    //and we're done    
    return(asset_memory_block_ptr);
}

internal void
ifb_engine_assets_files_asset_free(
    IFBEngineAssetMemoryBlockPtr asset_memory_block) {

    ifb_assert(asset_memory_block);
    ifb_engine_assets_memory_block_free(asset_memory_block);
}