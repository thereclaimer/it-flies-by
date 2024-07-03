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