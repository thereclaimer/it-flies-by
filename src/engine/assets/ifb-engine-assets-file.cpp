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
    const size_t asset_file_size = sizeof(IFBEngineAssetsFile);

    //allocate the asset file struct
    IFBEngineAssetsFile* asset_file = 
        (IFBEngineAssetsFile*)ifb_engine_memory::arena_push_bytes(
            file_arena,
            asset_file_size);    

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

    //allocate space for the index buffer and read it
    const size_t index_data_size   = (32 + (sizeof(u32) * 3));
    const size_t index_buffer_size = index_data_size * index_count;
    memory index_buffer = ifb_engine_memory::arena_push_bytes(data_arena,index_buffer_size);

    ifb_engine_platform_file_read(
        file_handle,
        7,
        index_buffer_size,
        index_buffer,
        false);

    //initialize the index array
    for (
        size_t index = 0;
        index < index_count;
        ++index) {

        IFBEngineAssetsFileIndex* file_index = &index_array[index]; 
        
        size_t index_data_offset = index_data_size * index;
        memory index_data        = index_buffer + index_data_offset;
    
        memmove(
            file_index->tag,
            index_data,
            32);
        file_index->tag[31] = 0;

        file_index->file_size       = *(u32*)&index_data[32]; 
        file_index->allocation_size = *(u32*)&index_data[32 + sizeof(u32)]; 
        file_index->offset          = *(u32*)&index_data[32 + (sizeof(u32) * 2)]; 
    }

    //we don't need the data arena anymore
    ifb_engine_assets::memory_arena_decommit(data_arena);

    //initialize the struct and return
    const size_t file_path_length = strlen(file_path);
    
    asset_file->arena       = file_arena;
    asset_file->file_handle = file_handle;
    asset_file->size        = file_size;
    asset_file->index_count = index_count;
    asset_file->index_array = index_array;
    asset_file->path        = (char*)ifb_engine_memory::arena_push_bytes(file_arena,file_path_length);
    strcpy(
        asset_file->path,
        file_path);

    return(asset_file);
}

internal void                       
ifb_engine_assets::file_close(
    const IFBEngineAssetsFile* asset_file) {

    ifb_engine_platform_file_close(asset_file->file_handle);

    ifb_engine_assets::memory_arena_decommit(asset_file->arena);
}


internal IFBEngineAssetsDataBlock_Impl* 
ifb_engine_assets::file_read(
    const IFBEngineAssetsFile* asset_file,
    const size_t               asset_index) {

    IFBEngineAssetsFileIndex* index = asset_file->index_array[asset_index];

    IFBEngineAssetsDataBlock_Impl* data_block = 
        ifb_engine_assets::data_block_commit(index->allocation_size);
    ifb_assert(data_block);

    ifb_engine_platform_file_read(
        asset_file->file_handle,
        index->offset,
        index->allocation_size,
        data_block->start,
        false);

    data_block->index = index;
    data_block->file  = asset_file;

    return(data_block);
}