#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRendererShaderTexturedQuad _textured_quad_shader;

internal IFBEngineRendererShaderHandle
ifb_engine_renderer_shader_textured_quad_handle() {

    IFBEngineRendererShaderHandle shader_handle = 
        ifb_engine_renderer_shader_create(
            IFBEngineAssetsShader_TexturedQuadVertexShader,
            IFBEngineAssetsShader_TexturedQuadFragmentShader,
            "SOLID QUAD");

    return(shader_handle);
}

internal void
ifb_engine_renderer_shader_textured_quad_uniforms() {

    const char* uniform_names[] = {
        "transform"
    };

    u32 uniform_count = sizeof(uniform_names) / sizeof(char*);

    ifb_engine_renderer_shader_uniform_push(
        _textured_quad_shader.shader_handle,
        uniform_count,
        uniform_names);
}

internal void
ifb_engine_renderer_shader_textured_quad_vertex_attributes() {

    const u32 attribute_count = 3;
    IFBEngineRendererShaderVertexArrayAttribute attributes[attribute_count];
    
    attributes[0].gl_type       = GL_FLOAT;
    attributes[0].gl_normalized = GL_FALSE;
    attributes[0].element_size  = sizeof(f32);
    attributes[0].element_count = 2;
    attributes[0].tag           = ifb_tag("POSITION");
    
    attributes[1].gl_type       = GL_FLOAT;
    attributes[1].gl_normalized = GL_FALSE;
    attributes[1].element_size  = sizeof(f32);
    attributes[1].element_count = 2;
    attributes[1].tag           = ifb_tag("UV");

    attributes[2].gl_type       = GL_FLOAT;
    attributes[2].gl_normalized = GL_FALSE;
    attributes[2].element_size  = sizeof(f32);
    attributes[2].element_count = 4;
    attributes[2].tag           = ifb_tag("COLOR");
}

internal IFBEngineRendererShaderTexturedQuad*
ifb_engine_renderer_shader_textured_quad_create() {

    _textured_quad_shader = {0};

    //create the handle
    _textured_quad_shader.shader_handle = ifb_engine_renderer_shader_textured_quad_handle();

    //set the uniforms and vertex attributes
    ifb_engine_renderer_shader_textured_quad_uniforms();
    ifb_engine_renderer_shader_textured_quad_vertex_attributes();

    //compile the shader
    ifb_engine_renderer_shader_compile(_textured_quad_shader.shader_handle);

    return(&_textured_quad_shader);
}