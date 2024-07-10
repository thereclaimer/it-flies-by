#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRendererShaderHandle _solid_quad_shader_handle;

internal IFBEngineRendererShaderHandle
ifb_engine_renderer_shader_solid_quad_create() {

    //create the handle
    _solid_quad_shader_handle =
        ifb_engine_renderer_shader_create(
            IFBEngineAssetsShader_SolidQuadVertexShader,
            IFBEngineAssetsShader_SolidQuadFragmentShader,
            "SOLID QUAD");

    //set the uniforms
    const char* uniform_names[] = {
        "solid_quad_update",
        "model",
        "color"
    };

    u32 uniform_count = sizeof(uniform_names) / sizeof(char*);

    // ifb_engine_renderer_shader_uniform_push(
    //     _solid_quad_shader_handle,
    //     uniform_count,
    //     uniform_names);

    //compile the shader
    ifb_engine_renderer_shader_compile(_solid_quad_shader_handle);

    return(_solid_quad_shader_handle);
}

