#pragma once

#include "ifb-engine-assets-new.hpp"
#include "ifb-engine-assets-internal.hpp"

internal const IFBEngineAssetsFile* 
ifb_engine_assets::file_open(
    const char* file_path) {

    //make sure we have a valid file
    const handle file_handle = ifb_engine_platform_file_open(file_path,false);
    ifb_assert(file_handle);

    //get the file size
    const size_t file_size = ifb_engine_platform_file_size(file_handle,false);
    ifb_assert(file_size > 0);

    //get a file arena
    const IFBEngineMemoryArena file_arena = ifb_engine_assets::memory_arena_file_commit();
    ifb_assert(file_arena);

    //sizes
    const asset_file_size = sizeof(IFBEngineAssetsFile);

    //allocate the asset file struct
    IFBEngineAssetsFile* asset_file = (IFBEngineAssetsFile*)ifb_engine_memory::arena_push_bytes(asset_file_size);    

    //get the index count
    char header_buffer[7] = {0};

    ifb_engine_platform_file_read(
        file_handle,
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
    const size_t index_array_size = sizeof(IFBEngineAssetsFileIndex) * index_count;

    //allocate space for the index array
    IFBEngineAssetsFileIndex* index_array = 
        (IFBEngineAssetsFileIndex*)ifb_engine_memory::arena_push_bytes(
            file_arena,
            index_array_size);
    ifb_assert(index_array);

    //this is a temporary arena for reading the index data
    IFBEngineMemoryArena data_arena = ifb_engine_assets::memory_arena_data_commit();

    //we don't need the data arena anymore
    ifb_engine_assets::memory_arena_decommit(data_arena);

    //initialize the struct and return
    asset_file->arena       = file_arena;
    asset_file->path        = file_path;
    asset_file->handle      = file_handle;
    asset_file->size        = file_size;
    asset_file->index_count = index_count;
    asset_file->index_array = index_array;

    return(asset_file);
}

internal void                       
ifb_engine_assets::file_close(
    const IFBEngineAssetsFile* asset_file) {

}


// internal void file_table_create(
//     void) {
    
//     IFBEngineAssetsContext& context = ifb_engine_assets::context_get();
// }

// internal void file_open(
//     const IFBEngineAssetsType type) {

//     IFBEngineAssetsContext&   context    = ifb_engine_assets::context_get();
//     IFBEngineAssetsFileTable& file_table = context.file_table;


//     IFBEngineAssetsFile& file = file_table.array[type];
//     const char* file_path = IFB_ENGINE_ASSETS_FILE_PATHS[type];

//     //open the file
//     file.type = type;
//     file.file_handle = 
//         ifb_engine_platform_file_open(
//             file_path,
//             false);
//     ifb_assert(file.file_handle);

//     //get the index count
//     char header_buffer[7] = {0};

//     ifb_engine_platform_file_read(
//         asset_file_handle,
//         0,
//         7,
//         (memory)header_buffer,
//         false);

//     ifb_assert(
//         header_buffer[0] == 'I' &&
//         header_buffer[1] == 'F' &&
//         header_buffer[2] == 'B'
//     );

//     const size_t index_count      = *(u32*)&header_buffer[3];
//     const size_t index_table_size = sizeof(IFBEngineAssetsFileIndex) * index_count;

//     //allocate the index table
//     file.index_table.block = 
//         (IFBEngineAssetsFileIndex*)
//             ifb_engine_assets::memory_block_commit(
//                 index_table_size);

//     ifb_assert(file.index_table.block);


// }

// internal void file_close(
//     const IFBEngineAssetsType type) {

// }