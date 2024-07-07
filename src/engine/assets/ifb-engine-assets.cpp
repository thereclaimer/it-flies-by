#pragma once

#include "ifb-engine-assets.hpp"

global IFBEngineAssets ifb_engine_assets = {0};

internal IFBEngineAssetsPtr
ifb_engine_assets_create_and_initialize() {

    ifb_engine_assets = {0};

    ifb_engine_assets.memory     = ifb_engine_assets_memory_create_and_initialize();
    ifb_engine_assets.files      = ifb_engine_asset_files_create_and_initialize();
    ifb_engine_assets.data_table = ifb_engine_assets_data_table_create_and_initialize();

    return(&ifb_engine_assets);
}

internal void
ifb_engine_assets_data_shader_load(
    const IFBEngineAssetsShaderId shader_id_vertex,
    const IFBEngineAssetsShaderId shader_id_fragment,
          IFBEngineAssetShader&   shader_asset_ref) {

    IFBEngineAssetData shader_data_vertex = 
        ifb_engine_assets_data_table_asset_load(
            IFBEngineAssetsType_Shader,
            shader_id_vertex);

    IFBEngineAssetData shader_data_fragment = 
        ifb_engine_assets_data_table_asset_load(
            IFBEngineAssetsType_Shader,
            shader_id_fragment);

    //initialize result
    shader_asset_ref = {0};
    shader_asset_ref.shader_data_vertex     = shader_data_vertex;
    shader_asset_ref.shader_data_fragment   = shader_data_fragment;
    shader_asset_ref.shader_buffer_vertex   = (char*)shader_data_vertex.buffer;
    shader_asset_ref.shader_buffer_fragment = (char*)shader_data_fragment.buffer;
}

internal void
ifb_engine_assets_image_load(
    const IFBEngineAssetsImageId image_id,
          IFBEngineAssetImage&   image_asset_ref) {

    //load the image data
    IFBEngineAssetData image_data = 
        ifb_engine_assets_data_table_asset_load(
            IFBEngineAssetsType_Image,
            image_id);

    //set the dimensions and pixel information
    const u32 image_dimensions_size_bytes = sizeof(u32) * 2;
    const u32 image_pixels_size_bytes     = image_data.buffer_size_bytes - image_dimensions_size_bytes;

    u32*   dimensions    = (u32*)image_data.buffer;
    u32    pixels_width  = dimensions[0]; 
    u32    pixels_height = dimensions[1];
    memory pixels        = image_data.buffer + image_dimensions_size_bytes;

    //initialize result
    image_asset_ref = {0};
    image_asset_ref.data               = image_data;
    image_asset_ref.pixels_rgba        = pixels;
    image_asset_ref.pixels_rgba_width  = pixels_width;
    image_asset_ref.pixels_rgba_height = pixels_height;
}

internal void
ifb_engine_assets_data_unload(
    IFBEngineAssetData& asset_data_ref) {

    ifb_engine_assets_data_table_asset_unload(
        asset_data_ref.type,
        asset_data_ref.index
    );
}

internal void
ifb_engine_assets_data_shader_unload(
    const IFBEngineAssetShader& shader_asset_ref) {

    ifb_engine_assets_data_table_asset_unload(
        IFBEngineAssetsType_Shader,
        shader_asset_ref.shader_data_vertex.index);

    ifb_engine_assets_data_table_asset_unload(
        IFBEngineAssetsType_Shader,
        shader_asset_ref.shader_data_fragment.index);
}


