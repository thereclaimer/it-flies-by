#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRendererShaderHandle _textured_quad_shader_handle;

internal IFBEngineRendererShaderHandle
ifb_engine_renderer_shader_textured_quad_create() {

    //create the handle
    _textured_quad_shader_handle =
        ifb_engine_renderer_shader_create(
            IFBEngineAssetsShader_TexturedQuadVertexShader,
            IFBEngineAssetsShader_TexturedQuadFragmentShader,
            "SOLID QUAD");

    //set the uniforms
    const char* uniform_names[] = {
        "transform"
    };

    u32 uniform_count = sizeof(uniform_names) / sizeof(char*);

    ifb_engine_renderer_shader_uniform_push(
        _textured_quad_shader_handle,
        uniform_count,
        uniform_names);

    //compile the shader
    ifb_engine_renderer_shader_compile(_textured_quad_shader_handle);

    return(_textured_quad_shader_handle);
}

