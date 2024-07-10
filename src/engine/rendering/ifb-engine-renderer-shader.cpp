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

    //vertex array object
    glGenVertexArrays(
        1,
        &new_shader_ref.gl_ids.vertex_array_object);

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

    //allocate space for the tags and locations
    memory uniform_tag_memory = 
        ifb_engine_memory_arena_bytes_push(
            shader_ref.memory.uniform.arena_1kb,
            sizeof(IFBTag) * uniform_count);
    ifb_assert(uniform_tag_memory);
    shader_ref.uniform_table.uniform_name_tags = (IFBTag*)uniform_tag_memory; 

    memory uniform_location_memory = 
        ifb_engine_memory_arena_bytes_push(
            shader_ref.memory.uniform.arena_1kb,
            sizeof(GLint)  * uniform_count);
    ifb_assert(uniform_location_memory);
    shader_ref.uniform_table.uniform_locations = (GLint*)uniform_location_memory; 

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
ifb_engine_renderer_shader_vertex_array_attributes_push(
    const IFBEngineRendererShaderHandle                shader_handle,
    const u32                                          attribute_count,
    const IFBEngineRendererShaderVertexArrayAttribute* attributes) {

    //get the shader reference
    IFBEngineRendererShaderRef shader_ref = 
        ifb_engine_renderer_shader_manager.shaders[shader_handle];

    //allocate space for the attributes
    u64 row_size = 
        sizeof(GLenum)    + 
        sizeof(GLboolean) + 
        sizeof(GLsizei)   + 
        sizeof(void*)     + 
        sizeof(IFBTag);

    u64 table_size = row_size * attribute_count;  

    memory table_memory = 
        ifb_engine_memory_arena_bytes_push(
            shader_ref.memory.vertex_array_object.arena_1kb,
            table_size);

    ifb_assert(table_memory);

    shader_ref.memory.vertex_array_object.vertex_attribute_table_memory = table_memory;
    shader_ref.memory.vertex_array_object.vertex_attribute_table_size   = table_size;

    //set the table pointers
    IFBEngineRendererShaderVertexArrayAttributeTable& attribute_table = 
        shader_ref.vertex_array_object.attribute_table;
    
    u64 offset = 0;
    attribute_table.gl_types       = (GLenum*)table_memory;
    
    offset += sizeof(GLenum)       * attribute_count;
    attribute_table.gl_normalizeds = (GLboolean*)&table_memory[offset]; 

    offset += sizeof(GLboolean)    * attribute_count;
    attribute_table.element_sizes  = (u32*)&table_memory[offset];

    offset += sizeof(u32)          * attribute_count;
    attribute_table.element_counts = (u32*)&table_memory[offset];

    offset += sizeof(u32)          * attribute_count;
    attribute_table.tags           = (IFBTag*)&table_memory[offset];
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

internal memory
ifb_engine_renderer_shader_push_vertex(
    const IFBEngineRendererShaderHandle shader_handle,
    const u32                           vertex_count) {

    //get the shader reference
    IFBEngineRendererShaderRef shader_ref = 
        ifb_engine_renderer_shader_manager.shaders[shader_handle];

    u64 allocation_size = 0;
    u64 element_size    = 0;
    u64 element_count   = 0;

    for (
        u32 attribute_index = 0;
        attribute_index < shader_ref.vertex_array_object.attribute_table.count;
        ++attribute_index) {

        element_size  = shader_ref.vertex_array_object.attribute_table.element_sizes[attribute_index];
        element_count = shader_ref.vertex_array_object.attribute_table.element_counts[attribute_index];

        allocation_size += element_size * element_count;
    }

    ifb_assert(allocation_size > 0);

    memory vertex_buffer =
        ifb_engine_memory_arena_bytes_push(
            shader_ref.memory.draw_buffer.arena_16kb,
            allocation_size);

    ifb_assert(vertex_buffer);

    shader_ref.memory.draw_buffer.draw_buffer_size += allocation_size; 

    return(vertex_buffer);
}

internal void
ifb_engine_renderer_shader_enable_vertex_array(
    IFBEngineRendererShaderHandle shader_handle) {

    //get the shader reference
    IFBEngineRendererShaderRef shader_ref = 
        ifb_engine_renderer_shader_manager.shaders[shader_handle];

    glBindVertexArray(shader_ref.gl_ids.vertex_array_object);

    IFBEngineRendererShaderVertexArrayAttributeTable& vertex_attribute_table = 
        shader_ref.vertex_array_object.attribute_table;

    u64 offset = 0;

    u32       element_count    = 0;
    u32       element_size     = 0;
    GLenum    gl_type          = 0;
    GLboolean gl_normalized    = 0;
    u32       attribute_stride = 0;

    for (
        u32 vertex_attribute_index = 0;
        vertex_attribute_index < vertex_attribute_table.count;
        ++vertex_attribute_index) {

        //enable the attribute index
        glEnableVertexAttribArray(vertex_attribute_index);

        //get our vertex attribute data
        element_count = vertex_attribute_table.element_counts[vertex_attribute_index];
        element_size  = vertex_attribute_table.element_sizes[vertex_attribute_index];
        gl_type       = vertex_attribute_table.gl_types[vertex_attribute_index];
        gl_normalized = vertex_attribute_table.gl_normalizeds[vertex_attribute_index];
        
        //calculate the stride
        attribute_stride = element_count * element_size;

        //set the pointer to this attribute
        glVertexAttribPointer(
            vertex_attribute_index,
            element_count,
            gl_type,
            gl_normalized,
            attribute_stride,
            (void*)offset);

        //update the offset
        offset += attribute_stride;
    }
}