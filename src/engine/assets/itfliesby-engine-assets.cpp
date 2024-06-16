#pragma once

#include "itfliesby-engine.hpp"

internal u32
itfliesby_engine_assets_file_header_num_indexes(
    const handle asset_handle) {
    
    char header_buffer[7] = {0};

    platform_api.file_read(
        asset_handle,
        0,
        7,
        (memory)header_buffer,
        false
    );

    ITFLIESBY_ASSERT(
        header_buffer[0] == 'I' &&
        header_buffer[1] == 'F' &&
        header_buffer[2] == 'B'
    );

    u16 num_indexes = *(u32*)&header_buffer[3];

    return(num_indexes);
}

internal memory
itfliesby_engine_assets_file_header_allocate_and_read(
    const handle                             asset_handle,
    const u32                                asset_header_num_indexes) {

    //calculate the index buffer suze
    u64 asset_data_size = ITFLIESBY_ASSET_FILE_INDEX_SIZE_TOTAL(asset_header_num_indexes);

    //allocate space for the index buffer
    memory asset_index_data = itfliesby_engine_memory_assets_file_header_allocate(asset_data_size);

    //read the indexes into the buffer
    platform_api.file_read(
        asset_handle,
        7,
        asset_data_size,
        asset_index_data,
        false
    );

    return(asset_index_data);
}

internal void
itfliesby_engine_asset_indexes_load(
    ItfliesbyEngineAssetsFileindex*          file_indexes,
    handle                                   file_handle,
    u32                                      file_indexes_count) {

    //make sure the indexes in the file match what we expect
    u32 file_indexes_count_actual = itfliesby_engine_assets_file_header_num_indexes(file_handle);
    ITFLIESBY_ASSERT(file_indexes_count_actual == file_indexes_count);

    //read our index buffer
    memory file_index_memory = 
        itfliesby_engine_assets_file_header_allocate_and_read(
            file_handle,
            file_indexes_count);

    ItfliesbyEngineAssetsFileindex* current_file_index;

    for (
        u32 index = 0;
        index < file_indexes_count;
        ++index) {

        //current index
        ItfliesbyEngineAssetsFileindex* current_index = &file_indexes[index];

        //calculate the offset
        u64 offset = ITFLIESBY_ASSET_FILE_INDEX_SIZE * index;

        //tag
        memmove(
            (void*)current_index->tag,
            (void*)&file_index_memory[offset],
            32
        );

        //rest of the shit
        memmove(
            (void*)((memory)current_index + 32),
            (void*)&file_index_memory[offset + 32],
            ITFLIESBY_ASSET_FILE_INDEX_SIZE - 32
        );

    }

    //destroy the index buffer and reset the allocator
    itfliesby_engine_memory_assets_file_header_reset(file_index_memory);
}

internal void
itfliesby_engine_asset_indexes_load_all(
    ItfliesbyEngineAssetsFileHandles*        asset_file_handles,
    ItfliesbyEngineAssetsFileIndexStore*     asset_index_store) {

    handle* asset_file_handle_array = asset_file_handles->array;
    handle  asset_file_handle;

    //load shaders
    itfliesby_engine_asset_indexes_load(
        asset_index_store->shader_indexes,
        asset_file_handles->shader_asset_file,
        ITFLIESBY_ENGINE_ASSETS_SHADER_COUNT
    );

    //load images
    itfliesby_engine_asset_indexes_load(
        asset_index_store->image_indexes,
        asset_file_handles->image_asset_file,
        ITFLIESBY_ENGINE_ASSETS_IMAGE_COUNT
    );
}

internal void 
itfliesby_engine_assets_init(
    ItfliesbyEngineAssets* assets) {

    itfliesby_engine_memory_assets_create();

    *assets = {0};
    ItfliesbyEngineAssetsFileHandles* file_handles = &assets->file_handles;

    itfliesby_engine_assets_file_handles_load(file_handles);

    itfliesby_engine_asset_indexes_load_all(
        file_handles,
        &assets->file_index_store
    );

    ITFLIESBY_NOP();
}

internal void
itfliesby_engine_assets_update(
    ItfliesbyEngineAssets* assets) {

    ItfliesbyEngineAssetsFileHandles*    file_handles = &assets->file_handles;
    ItfliesbyEngineAssetsFileIndexStore* index_store  = &assets->file_index_store;


}

internal void
itfliesby_engine_assets_file_handles_load(
    ItfliesbyEngineAssetsFileHandles* file_handles) {

    //cache our stuff
    handle* file_handle_array   = file_handles->array;
    handle  current_file_handle;
    u32     missing_files_count = 0;
    s32     missing_files_array[ITFLIESBY_ASSETS_FILE_ID_COUNT];
    str     file_path;
    
    for (
        u32 file_handle_index = 0;
        file_handle_index < ITFLIESBY_ASSETS_FILE_ID_COUNT;
        ++file_handle_index) {

        //get the file path
        file_path = (str)ITFLIESBY_ENGINE_ASSETS_FILE_PATHS[file_handle_index];

        //open the file
        current_file_handle = platform_api.file_open(file_path,false);
    
        //if we have a file, we're good
        if (current_file_handle) {
            file_handle_array[file_handle_index] = current_file_handle;
            continue;
        }

        //otherwise, add this to the missing file list
        missing_files_array[missing_files_count] = file_handle_index;
        ++missing_files_count;
    }

    if (missing_files_count > 0) {
        //TODO: error message
        ITFLIESBY_PANIC();
    }
}

internal u64
itfliesby_engine_assets_index_allocation_size(
    ItfliesbyEngineAssetsFileindex* indexes,
    s32*                            index_ids,
    s32                             index_ids_count) {

    ITFLIESBY_ASSERT(
        indexes    && 
        index_ids  && 
        index_ids_count > 0);

    ItfliesbyEngineAssetsFileindex current_index;
    u64 index_memory_size = 0;
    u32 current_index_id;

    for (
        u32 index = 0;
        index < index_ids_count;
        ++index) {

        //get the index size
        current_index_id = index_ids[index];
        current_index     = indexes[current_index_id];
        index_memory_size += current_index.allocation_size;
    }

    return(index_memory_size);
}

internal u64
itfliesby_engine_assets_shader_allocation_size(
    ItfliesbyEngineAssets*       assets,
    ItfliesbyEngineAssetsShader* shader_index_ids,
    s32                          shader_index_ids_count) {

    ITFLIESBY_ASSERT(
        assets                     &&
        shader_index_ids           &&
        shader_index_ids_count > 0 &&
        shader_index_ids_count < ITFLIESBY_ENGINE_ASSETS_SHADER_COUNT
    );

    ItfliesbyEngineAssetsFileindex* shader_file_indexes = assets->file_index_store.shader_indexes;

    ItfliesbyEngineAssetsFileindex current_index;
    u64 shader_allocation_size = 0;
    u32 current_index_id       = 0;

    for (
        u32 index = 0;
        index < shader_index_ids_count;
        ++index) {

        //get the index size
        current_index_id        = shader_index_ids[index];
        current_index           = shader_file_indexes[current_index_id];
        shader_allocation_size += current_index.allocation_size;
    }

    return(shader_allocation_size);
}

internal void
itfliesby_engine_assets_load_asset_from_index(
    ItfliesbyEngineAssetsFileindex asset_file_index,
    handle                         asset_file_handle,
    memory                         asset_file_memory,
    u32                            asset_file_memory_offset,
    u32                            asset_file_memory_size) {

    platform_api.file_read(
        asset_file_handle,
        asset_file_index.offset + asset_file_memory_offset,
        asset_file_memory_size,
        asset_file_memory,
        false
    );
}

internal void
itfliesby_engine_assets_load_shaders(
    ItfliesbyEngineAssets*       assets,
    ItfliesbyEngineAssetsShader* shader_index_ids,
    memory                       shader_memory,
    u64*                         shader_offsets,
    u32                          shader_count) {

    ITFLIESBY_ASSERT(
        assets && 
        shader_memory &&
        shader_count > 0);

    //NOTE: we are assuming here you have allocated the memory
    //fuck around and find out

    ItfliesbyEngineAssetsFileindex* shader_file_indexes       = assets->file_index_store.shader_indexes;
    handle                          shader_file_handle        = assets->file_handles.shader_asset_file;
    ItfliesbyEngineAssetsFileindex  current_shader_file_index = {0};
    memory                          current_shader_memory     = NULL;
    u64                             current_shader_offset     = 0;
    ItfliesbyEngineAssetsShader     current_shader_index_id   = ITFLIESBY_ENGINE_ASSETS_SHADER_INVALID;
    for (
        u32 shader_index = 0;
        shader_index < shader_count;
        ++shader_index) {

        //get info for the current shader
        current_shader_index_id   = shader_index_ids[shader_index];
        current_shader_memory     = &shader_memory[current_shader_offset];
        current_shader_file_index = shader_file_indexes[current_shader_index_id];

        //load the shader
        itfliesby_engine_assets_load_asset_from_index(
            current_shader_file_index,
            shader_file_handle,
            current_shader_memory,
            0,
            current_shader_file_index.allocation_size
        );
    
        //update the offsets
        shader_offsets[shader_index] = current_shader_offset;
        current_shader_offset       += current_shader_file_index.allocation_size;
    }
}

internal u64
itfliesby_engine_assets_image_allocation_size(
    ItfliesbyEngineAssets*     assets,
    ItfliesbyEngineAssetsImage image) {

    ItfliesbyEngineAssetsFileindex* shader_file_indexes = 
        assets->file_index_store.image_indexes;

    ItfliesbyEngineAssetsFileindex file_index = 
        shader_file_indexes[image];

    u64 allocation_size = file_index.allocation_size;

    return(allocation_size);
}

internal ItfliesbyEngineAssetsImageData*
itfliesby_engine_assets_image_load(
    ItfliesbyEngineAssets*          assets,
    ItfliesbyEngineAssetsImage      image) {

    //get our image index
    ItfliesbyEngineAssetsFileindex* image_file_indexes = 
        assets->file_index_store.image_indexes;

    ItfliesbyEngineAssetsFileindex file_index = 
        image_file_indexes[image];
    
    //allocate space for our image
    ItfliesbyEngineAssetsImageData* image_data = 
        itfliesby_engine_memory_assets_image_allocate(
            &file_index);
    
    //load the image data
    handle file_handle = assets->file_handles.image_asset_file;
    const u32 image_dimensions_size_bytes = sizeof(u32) * 2;
    const u32 image_pixels_size_bytes     = file_index.allocation_size - image_dimensions_size_bytes; 

    //image dimensions
    itfliesby_engine_assets_load_asset_from_index(
        file_index,
        file_handle,
        (memory)image_data,
        0,
        image_dimensions_size_bytes);

    //TODO: I was originally passing the image_data->pixels directly into the function,
    //but it was still not initialized. So why the FUCK did this work!?
    memory image_pixels = image_data->pixels;

    //image pixels
    itfliesby_engine_assets_load_asset_from_index(
        file_index,
        file_handle,
        image_pixels,
        image_dimensions_size_bytes,
        image_pixels_size_bytes);

    image_data->pixels = image_pixels;

    //return our image data
    return(image_data);
}

internal void
itfliesby_engine_assets_image_unload(
    ItfliesbyEngineAssets*           assets,
    ItfliesbyEngineAssetsImageData** image_data) {

    //TODO: ideally, we should be able to allocate a batch of images
    //currently, the linear allocator is used to allocate one at a time

    *image_data = NULL;
    itfliesby_engine_memory_assets_image_reset();
}