#pragma once

#include "ifb-engine-renderer.hpp"

global IFBEngineRendererShaderManager ifb_engine_renderer_shader_manager;

internal IFBEngineRendererShaderManagerPtr
ifb_engine_renderer_shader_manager_create_and_initialize() {

    ifb_engine_renderer_shader_manager               = {0};
    ifb_engine_renderer_shader_manager.shaders_count = 0;
    ifb_engine_renderer_shader_manager.arena_16kb    = 
        ifb_engine_renderer_memory_arena_reserve(IFBEngineRendererMemoryArenaSize_16KB);    

    ifb_engine_renderer_shader_manager.shaders = 
        (IFBEngineRendererShaderPtr)ifb_engine_renderer_shader_manager.arena_16kb->block.memory;

    ifb_assert(ifb_engine_renderer_shader_manager.arena_16kb);

    return(&ifb_engine_renderer_shader_manager);
}

internal IFBEngineRendererShaderHandle
ifb_engine_renderer_shader_create(
    const IFBEngineAssetsShaderId asset_stage_vertex,
    const IFBEngineAssetsShaderId asset_stage_fragment,
    const char*                   shader_tag) {

    ifb_assert(ifb_engine_renderer_shader_manager.arena_16kb);

    IFBEngineRendererShaderHandle new_shader_handle = 
        ifb_engine_renderer_shader_manager.shaders_count;

    //allocation size
    u64 new_shader_size = sizeof(IFBEngineRendererShader);  

    //allocate a new shader
    memory new_shader_memory = 
        ifb_engine_memory_arena_bytes_push(
            ifb_engine_renderer_shader_manager.arena_16kb,            
            new_shader_size); 

    ifb_assert(new_shader_memory);

    u32 new_shader_index = ifb_engine_renderer_shader_manager.shaders_count; 

    ++ifb_engine_renderer_shader_manager.shaders_count;

    //get the reference to the  new shader
    IFBEngineRendererShaderRef new_shader_ref = ifb_engine_renderer_shader_manager.shaders[new_shader_index]; 
    new_shader_ref = {0};

    //get the shader assets
    ifb_engine_assets_data_shader_load(
        asset_stage_vertex,
        asset_stage_fragment,        
        new_shader_ref.assets);

    //allocate the arenas
    new_shader_ref.memory.vertex_array_object.arena_1kb = ifb_engine_renderer_memory_arena_reserve(IFBEngineRendererMemoryArenaSize_1KB);
    new_shader_ref.memory.uniform.arena_1kb             = ifb_engine_renderer_memory_arena_reserve(IFBEngineRendererMemoryArenaSize_1KB);
    new_shader_ref.memory.draw_buffer.arena_16kb        = ifb_engine_renderer_memory_arena_reserve(IFBEngineRendererMemoryArenaSize_16KB);

    ifb_assert(new_shader_ref.memory.vertex_array_object.arena_1kb);
    ifb_assert(new_shader_ref.memory.uniform.arena_1kb);
    ifb_assert(new_shader_ref.memory.draw_buffer.arena_16kb);

    new_shader_ref.memory.vertex_array_object.vertex_attribute_table_memory = new_shader_ref.memory.vertex_array_object.arena_1kb->block.memory; 
    new_shader_ref.memory.uniform.uniform_table_memory                      = new_shader_ref.memory.uniform.arena_1kb->block.memory; 
    new_shader_ref.memory.draw_buffer.draw_buffer_memory                    = new_shader_ref.memory.draw_buffer.arena_16kb->block.memory;

    //tag
    new_shader_ref.tag = ifb_tag(shader_tag);

    return(new_shader_handle);
}

internal void
ifb_engine_renderer_shader_uniform_push(
    const IFBEngineRendererShaderHandle shader_handle,
    const u32                           uniform_count,              
    const char**                        uniform_name) {

    //get the shader reference
    IFBEngineRendererShaderRef shader_ref = 
        ifb_engine_renderer_shader_manager.shaders[shader_handle];        

    //allocate space for the tag
    memory uniform_tag_memory = 
        ifb_engine_memory_arena_bytes_push(
            shader_ref.memory.uniform.arena_1kb,
            sizeof(IFBTag) * uniform_count);

    ifb_assert(uniform_tag_memory);

    //set the uniform array if its not already
    if (!shader_ref.uniform_table.uniform_name_tags) {
        shader_ref.uniform_table.uniform_name_tags = (IFBTag*)uniform_tag_memory; 
    }

    //set the tag values
    for (
        u32 uniform_name_index = 0;
        uniform_name_index < uniform_count;
        ++uniform_name_index) {

        shader_ref.uniform_table.uniform_name_tags[
            shader_ref.uniform_table.count +
            uniform_name_index] = ifb_tag(uniform_name[uniform_name_index]);
    }

    //update the uniform count
    shader_ref.uniform_table.count += uniform_count;
}

internal void
ifb_engine_renderer_shader_compile(
    const IFBEngineRendererShaderHandle shader_handle) {

    //get the shader reference
    IFBEngineRendererShaderRef shader_ref = 
        ifb_engine_renderer_shader_manager.shaders[shader_handle];

    //compile the shader stages
    shader_ref.gl_ids.stage_vertex   = ifb_engine_renderer_opengl_shader_stage_vertex_compile(shader_ref.assets.shader_buffer_vertex);
    shader_ref.gl_ids.stage_fragment = ifb_engine_renderer_opengl_shader_stage_fragment_compile(shader_ref.assets.shader_buffer_fragment);

    ifb_assert(
        shader_ref.gl_ids.stage_vertex   > 0 &&
        shader_ref.gl_ids.stage_fragment > 0);

    //create and link the shader program    
    shader_ref.gl_ids.program  = 
        ifb_engine_renderer_opengl_program_create_and_link(
            shader_ref.gl_ids.stage_vertex,
            shader_ref.gl_ids.stage_fragment);

    ifb_assert(shader_ref.gl_ids.program > 0);

    //we don't need the shader data anymore
    ifb_engine_assets_data_shader_unload(shader_ref.assets);

    //get the uniform locations
    IFBTag current_uniform_tag;
    GLint  current_uniform_location;
    for (
        u32 uniform_index = 0;
        uniform_index < shader_ref.uniform_table.count;
        ++uniform_index) {

        current_uniform_tag      = shader_ref.uniform_table.uniform_name_tags[uniform_index]; 
        current_uniform_location = 
            ifb_engine_renderer_opengl_uniform_location(
                shader_ref.gl_ids.program,
                current_uniform_tag);
        
        ifb_assert(current_uniform_location >= 0);

        shader_ref.uniform_table.uniform_locations[uniform_index] = current_uniform_location; 
    }

}