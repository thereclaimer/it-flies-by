#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRenderer ifb_engine_renderer;

internal IFBEngineRendererPtr
ifb_engine_renderer_create_and_initialize() {

    ifb_engine_renderer = {0};

    ifb_engine_renderer.memory = ifb_engine_renderer_memory_create_and_initialize();

    //create the shaders
    ifb_engine_renderer.shader_manager               = ifb_engine_renderer_shader_manager_create_and_initialize();
    ifb_engine_renderer.shader_handles.solid_quad    = ifb_engine_renderer_shader_solid_quad_create();
    ifb_engine_renderer.shader_handles.textured_quad = ifb_engine_renderer_shader_textured_quad_create();

    return(&ifb_engine_renderer);
}

internal void
ifb_engine_renderer_update() {

}
