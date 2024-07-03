#pragma once

#include "ifb-engine.hpp"

global IFBEngine ifb_engine;

internal IFBEngineSystems 
ifb_engine_systems_initialize() {

    IFBEngineSystems engine_systems = {0};

    //memory
    engine_systems.memory = ifb_engine_memory_create_and_initialize();
    ifb_assert(engine_systems.memory);

    //assets
    engine_systems.assets = ifb_engine_assets_create_and_initialize();
    ifb_assert(engine_systems.assets);

    return(engine_systems);
}

external IFBEnginePtr
ifb_engine_create_and_initialize(
    IFBPlatformApi platform) {

    ifb_engine = {0};

    //validate the platform api
    ifb_engine_platform_validate(platform);

    //initialize engine systems
    ifb_engine.systems = ifb_engine_systems_initialize();

    //initialize engine core
    ifb_engine.memory = ifb_engine_core_memory_create_and_initialize();

    return(&ifb_engine);
}

external void
ifb_engine_render_frame() {

    //reset the frame allocator
    ifb_engine_core_memory_frame_reset();

    //test the asset loading
    IFBEngineAssetImage image_asset = {0}; 
        ifb_engine_assets_image_load(
            IFBEngineAssetsImage_ConnorCalibaration,
            image_asset);

    IFBEngineAssetShader shader_asset = {0};
    ifb_engine_assets_data_shader_load(
        IFBEngineAssetsShader_SolidQuadVertexShader,
        IFBEngineAssetsShader_SolidQuadFragmentShader,
        shader_asset);

    ifb_nop();
}