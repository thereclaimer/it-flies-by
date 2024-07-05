#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRenderer ifb_engine_renderer;

internal IFBEngineRendererPtr
ifb_engine_renderer_create_and_initialize() {

    ifb_engine_renderer = {0};

    ifb_engine_renderer.memory = ifb_engine_renderer_memory_create_and_initialize();

    return(&ifb_engine_renderer);
}

internal void
ifb_engine_renderer_update() {

    //update the shaders
    ifb_engine_renderer_shader_store_update();
}
