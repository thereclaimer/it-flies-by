#pragma once

#include "ifb-engine-assets.hpp"

global IFBEngineAssetDataTable ifb_engine_asset_data_table;

internal IFBEngineAssetDataTablePtr
ifb_engine_assets_data_table_create_and_initialize() {

    ifb_engine_asset_data_table = {0};

    return(&ifb_engine_asset_data_table);
}