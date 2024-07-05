#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRendererShaderStore ifb_engine_renderer_shader_store;
global IFBEngineRendererShaders ifb_engine_renderer_shaders;


internal IFBEngineRendererShaderStorePtr
ifb_engine_renderer_shader_store_create_and_initialize() {

    ifb_engine_renderer_shader_store = {0};

    return(&ifb_engine_renderer_shader_store);
}

internal IFBEngineRendererShaderPtr
ifb_engine_renderer_shader_create(
    const IFBEngineAssetsShaderId                   stage_id_vertex,
    const IFBEngineAssetsShaderId                   stage_id_fragment,
    const u32                                       uniform_count,
    const char**                                    uniform_names,
    const funcptr_ifb_engine_renderer_shader_update update_callback) {

    ifb_assert(update_callback);

    //load the asset
    IFBEngineAssetShader shader_asset = {0};
    ifb_engine_assets_data_shader_load(
        stage_id_vertex,
        stage_id_fragment,
        shader_asset);

    //compile the shader
    GLuint gl_id_shader_stage_vertex   = ifb_engine_renderer_opengl_shader_stage_vertex_compile((const char*)shader_asset.shader_buffer_vertex);
    GLuint gl_id_shader_stage_fragment = ifb_engine_renderer_opengl_shader_stage_fragment_compile((const char*)shader_asset.shader_buffer_fragment);
    
    ifb_assert(
        gl_id_shader_stage_vertex   > 0 &&
        gl_id_shader_stage_fragment > 0);

    GLuint gl_id_shader_program = 
        ifb_engine_renderer_opengl_program_create_and_link(
            gl_id_shader_stage_vertex,
            gl_id_shader_stage_fragment);

    ifb_assert(gl_id_shader_program);

    //get a new arena for the shader
    IFBEngineMemoryArenaPtr shader_arena = 
        ifb_engine_renderer_memory_arena_reserve(IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_8KB);
    ifb_assert(shader_arena);

    //allocate and initialize the uniforms
    IFBEngineRendererShaderUniformPtr uniforms = 
        ifb_engine_renderer_memory_uniforms_push(uniform_count);
    ifb_assert(uniforms);

    IFBEngineRendererShaderUniformPtr uniform_ptr;
    const char*                       uniform_name;

    for (
        u32 uniform_index = 0;
        uniform_index < uniform_count;
        ++uniform_index) {

        uniform_ptr  = &uniforms[uniform_index];
        uniform_name = uniform_names[uniform_index];
        
        uniform_ptr->name = ifb_tag(uniform_name);
        uniform_ptr->id   = glGetUniformLocation(gl_id_shader_program,uniform_name); 
    }

    //allocate and initialize the structure
    IFBEngineRendererShaderPtr shader_ptr = ifb_engine_renderer_memory_allocator_shader_push();
    ifb_assert(shader_ptr);

    //update the count, and make this the list if its the first one
    ++ifb_engine_renderer_shader_store.shaders_count;
    if (!ifb_engine_renderer_shader_store.shaders) {
        ifb_engine_renderer_shader_store.shaders = shader_ptr;
    }

    shader_ptr->arena_8k                      = shader_arena;
    shader_ptr->asset                         = shader_asset;
    shader_ptr->stages.gl_program_id          = gl_id_shader_program;
    shader_ptr->stages.gl_shader_stage_vertex = gl_id_shader_stage_vertex;
    shader_ptr->stages.gl_shader_stage_vertex = gl_id_shader_stage_fragment;
    shader_ptr->uniform_count                 = uniform_count;
    shader_ptr->uniforms                      = uniform_ptr;
    shader_ptr->update_callback               = update_callback;

    return(shader_ptr);
}

internal void
ifb_engine_renderer_shader_store_update() {

    IFBEngineRendererShaderPtr shader_ptr = NULL;

    for (
        u32 shader_index = 0;
        shader_index < ifb_engine_renderer_shader_store.shaders_count;
        ++shader_index) {

        shader_ptr = &ifb_engine_renderer_shader_store.shaders[shader_index];
        
        shader_ptr->update_callback(shader_ptr->shader_impl);
    }
}


internal void
ifb_engine_renderer_shader_simple_quad_update(
    void* shader_impl) {
    
    ifb_assert(shader_impl);
    IFBEngineRendererShaderSimpleQuad* simple_quad_shader = (IFBEngineRendererShaderSimpleQuad*)shader_impl;
}

internal void
ifb_engine_renderer_shader_solid_quad_update(
    void* shader_impl) {

    ifb_assert(shader_impl);
    IFBEngineRendererShaderSolidQuad* solid_quad_shader = (IFBEngineRendererShaderSolidQuad*)shader_impl;
}

internal void
ifb_engine_renderer_shader_textured_quad_update(
    void* shader_impl) {

    ifb_assert(shader_impl);
    IFBEngineRendererShaderTexturedQuad* textured_quad_shader = (IFBEngineRendererShaderTexturedQuad*)shader_impl;
}

internal void 
ifb_engine_renderer_shaders_create() {

    ifb_engine_renderer_shaders = {0};

    IFBEngineRendererShaderPtr shader        = NULL;
    memory                     shader_memory = NULL;

    //----------------------------------
    // SIMPLE QUAD SHADER
    //----------------------------------

    const char* simple_quad_shader_uniforms[] = {
        "transform",
        "color"
    };

    shader =
        ifb_engine_renderer_shader_create(
            IFBEngineAssetsShader_SimpleQuadVertexShader,
            IFBEngineAssetsShader_SimpleQuadFragmentShader,
            2,
            simple_quad_shader_uniforms,            
            ifb_engine_renderer_shader_simple_quad_update);

    ifb_assert(shader);

    shader_memory = 
        ifb_engine_memory_arena_bytes_push(
            shader->arena_8k,
            sizeof(IFBEngineRendererShaderSimpleQuad)); 
    
    shader->shader_impl = shader_memory;

    ifb_assert(shader_memory);

    ifb_engine_renderer_shaders.shader_simple_quad = (IFBEngineRendererShaderSimpleQuad*)shader_memory;

    //----------------------------------
    // SOLID QUAD SHADER
    //----------------------------------

    const char* solid_quad_shader_uniforms[] = {
        "solid_quad_update",
        "model",
        "color"
    };

    shader =
        ifb_engine_renderer_shader_create(
            IFBEngineAssetsShader_SolidQuadVertexShader,
            IFBEngineAssetsShader_SolidQuadFragmentShader,
            2,
            solid_quad_shader_uniforms,            
            ifb_engine_renderer_shader_solid_quad_update);

    ifb_assert(shader);

    shader_memory = 
        ifb_engine_memory_arena_bytes_push(
            shader->arena_8k,
            sizeof(IFBEngineRendererShaderSimpleQuad)); 
    
    ifb_assert(shader_memory);

    ifb_engine_renderer_shaders.shader_solid_quad = (IFBEngineRendererShaderSolidQuad*)shader_memory;

    //----------------------------------
    // TEXTURED QUAD SHADER
    //----------------------------------

    const char* textured_quad_shader_uniforms[] = {
        "solid_quad_update",
        "model",
        "color"
    };

    shader =
        ifb_engine_renderer_shader_create(
            IFBEngineAssetsShader_TexturedQuadVertexShader,
            IFBEngineAssetsShader_TexturedQuadFragmentShader,
            2,
            textured_quad_shader_uniforms,            
            ifb_engine_renderer_shader_textured_quad_update);

    ifb_assert(shader);

    shader_memory = 
        ifb_engine_memory_arena_bytes_push(
            shader->arena_8k,
            sizeof(IFBEngineRendererShaderTexturedQuad)); 
    
    ifb_assert(shader_memory);

    ifb_engine_renderer_shaders.shader_textured_quad = (IFBEngineRendererShaderTexturedQuad*)shader_memory;
    
}