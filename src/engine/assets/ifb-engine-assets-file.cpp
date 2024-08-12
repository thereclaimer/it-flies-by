#pragma once

#include "ifb-engine-assets-new.hpp"
#include "ifb-engine-assets-internal.hpp"

internal void file_table_create(
    void) {
    
    IFBEngineAssetsContext& context = ifb_engine_assets::context_get();
}

internal void file_open(
    const IFBEngineAssetsType type) {

    IFBEngineAssetsContext&   context    = ifb_engine_assets::context_get();
    IFBEngineAssetsFileTable& file_table = context.file_table;


    IFBEngineAssetsFile& file = file_table.array[type];
    const char* file_path = IFB_ENGINE_ASSETS_FILE_PATHS[type];

    //open the file
    file.type = type;
    file.file_handle = 
        ifb_engine_platform_file_open(
            file_path,
            false);
    ifb_assert(file.file_handle);

    //get the index count
    char header_buffer[7] = {0};

    ifb_engine_platform_file_read(
        asset_file_handle,
        0,
        7,
        (memory)header_buffer,
        false);

    ifb_assert(
        header_buffer[0] == 'I' &&
        header_buffer[1] == 'F' &&
        header_buffer[2] == 'B'
    );

    const size_t index_count      = *(u32*)&header_buffer[3];
    const size_t index_table_size = sizeof(IFBEngineAssetsFileIndex) * index_count;

    //allocate the index table
    file.index_table.block = 
        (IFBEngineAssetsFileIndex*)
            ifb_engine_assets::memory_block_commit(
                index_table_size);

    ifb_assert(file.index_table.block);


}

internal void file_close(
    const IFBEngineAssetsType type) {

}