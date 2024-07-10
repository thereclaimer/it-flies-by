#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRendererShaderSolidQuad _solid_quad_shader;

internal IFBEngineRendererShaderHandle
ifb_engine_renderer_shader_solid_quad_handle() {

    IFBEngineRendererShaderHandle shader_handle = 
        ifb_engine_renderer_shader_create(
            IFBEngineAssetsShader_SolidQuadVertexShader,
            IFBEngineAssetsShader_SolidQuadFragmentShader,
            "SOLID QUAD");

    return(shader_handle);
}

internal void
ifb_engine_renderer_shader_solid_quad_uniforms() {

    const char* uniform_names[] = {
        "transform"
    };

    u32 uniform_count = sizeof(uniform_names) / sizeof(char*);

    ifb_engine_renderer_shader_uniform_push(
        _solid_quad_shader.shader_handle,
        uniform_count,
        uniform_names);
}

internal void
ifb_engine_renderer_shader_solid_quad_vertex_attributes() {

    const u32 attribute_count = 2;
    IFBEngineRendererShaderVertexArrayAttribute attributes[attribute_count];
    
    attributes[0].gl_type       = GL_FLOAT;
    attributes[0].gl_normalized = GL_FALSE;
    attributes[0].element_size  = sizeof(f32);
    attributes[0].element_count = 2;
    attributes[0].tag           = ifb_tag("POSITION");
    
    attributes[1].gl_type       = GL_FLOAT;
    attributes[1].gl_normalized = GL_FALSE;
    attributes[1].element_size  = sizeof(f32);
    attributes[1].element_count = 4;
    attributes[1].tag           = ifb_tag("COLOR");

    ifb_engine_renderer_shader_vertex_array_attributes_push(
        _solid_quad_shader.shader_handle,
        attribute_count,
        attributes);
}

internal IFBEngineRendererShaderSolidQuad*
ifb_engine_renderer_shader_solid_quad_create() {

    _solid_quad_shader = {0};

    //create the handle
    _solid_quad_shader.shader_handle = ifb_engine_renderer_shader_solid_quad_handle(); 
    
    //add the unforms and vertex attributes
    ifb_engine_renderer_shader_solid_quad_uniforms();
    ifb_engine_renderer_shader_solid_quad_vertex_attributes();

    //compile the shader
    ifb_engine_renderer_shader_compile(_solid_quad_shader.shader_handle);

    return(&_solid_quad_shader);
}

internal void
ifb_engine_renderer_shader_solid_quad_push(
    IFBEngineRendererSolidQuad* solid_quads,
    u32                         solid_quads_count) {

    ifb_assert(
        solid_quads &&
        solid_quads_count > 0);

    //allocate space on the draw buffer
    IFBEngineRendererSolidQuadVertex* quad_vertex_buffer = 
        (IFBEngineRendererSolidQuadVertex*)ifb_engine_renderer_shader_push_vertex(
            _solid_quad_shader.shader_handle,
            solid_quads_count); 

    ifb_assert(quad_vertex_buffer);

    for (
        u32 quad_index = 0;
        quad_index < solid_quads_count;
        ++quad_index) {
        
        memmove(
            (void*)quad_vertex_buffer[quad_index].transform,
            (void*)solid_quads[quad_index].transform.m,
            sizeof(f32) * 9);

        memmove(
            (void*)quad_vertex_buffer[quad_index].color,
            (void*)solid_quads[quad_index].color.rgba,
            sizeof(f32) * 4);
    }

}