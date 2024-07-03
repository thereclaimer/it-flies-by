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

internal IFBEngineAssetData
ifb_engine_assets_data_shader_load(
    IFBEngineAssetsShader shader) {

    IFBEngineAssetData shader_data = 
        ifb_engine_assets_data_table_asset_load(
            IFBEngineAssetsType_Shader,
            shader);

    return(shader_data);
}

internal IFBEngineAssetData
ifb_engine_assets_data_image_load(
    IFBEngineAssetsImage image) {
    
    IFBEngineAssetData image_data = 
        ifb_engine_assets_data_table_asset_load(
            IFBEngineAssetsType_Image,
            image);

    return(image_data);
}

internal void
ifb_engine_assets_data_unload(
    IFBEngineAssetData& asset_data_ref) {

    ifb_engine_assets_data_table_asset_unload(
        asset_data_ref.type,
        asset_data_ref.index
    );
}