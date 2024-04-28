#pragma once

#include "itfliesby-renderer.hpp"

internal b8
itfliesby_renderer_shader_is_valid(
    const ItfliesbyRendererShader* shader) {

    ITFLIESBY_ASSERT(shader);

    b8 shader_valid = (
        shader->gl_program_id        > 0 && 
        shader->gl_stage_id_vertex   > 0 && 
        shader->gl_stage_id_fragment > 0);

    return(shader_valid);
}

external void
itfliesby_renderer_shader_destroy(
    ItfliesbyRenderer*           renderer,
    ItfliesbyRendererShaderType  shader_type) {

    ITFLIESBY_ASSERT(renderer);
    ITFLIESBY_ASSERT(shader_type >= 0 && shader_type < ITFLIESBY_RENDERER_SHADER_TYPE_COUNT);

    ItfliesbyRendererShaderStore* shader_store      = &renderer->shader_store;
    ItfliesbyRendererShader*      shaders           = shader_store->shaders;
    ItfliesbyRendererShader*      shader_to_destroy = &shaders[shader_type];

    glDeleteShader(shader_to_destroy->gl_stage_id_vertex);
    glDeleteShader(shader_to_destroy->gl_stage_id_fragment);
    glDeleteProgram(shader_to_destroy->gl_program_id);

    shader_to_destroy->gl_stage_id_vertex   = 0;
    shader_to_destroy->gl_stage_id_fragment = 0;
    shader_to_destroy->gl_program_id        = 0;
}

external ItfliesbyRendererShaderIndex
itfliesby_renderer_shader_compile_and_link(
    ItfliesbyRenderer*                  renderer,
    ItfliesbyRendererShaderType         shader_type,
    ItfliesbyRendererShaderStageBuffer* shader_stage_buffer_vertex,
    ItfliesbyRendererShaderStageBuffer* shader_stage_buffer_fragment) {

    ITFLIESBY_ASSERT(
        renderer         &&
        shader_stage_buffer_vertex &&
        shader_stage_buffer_fragment
    );

    ItfliesbyRendererShaderStore* shader_stage_store = &renderer->shader_store;
    ItfliesbyRendererShader* shader = &shader_stage_store->shaders[shader_type];

    //create the ids for our shader
    GLuint gl_shader_program_id        = glCreateProgram(); 
    GLuint gl_shader_stage_id_vertex   = glCreateShader(GL_VERTEX_SHADER);
    GLuint gl_shader_stage_id_fragment = glCreateShader(GL_FRAGMENT_SHADER);

    //upload the shader source
    glShaderSource(gl_shader_stage_id_vertex,   1, (const char**)&shader_stage_buffer_vertex->shader_stage_data,   NULL);
    glShaderSource(gl_shader_stage_id_fragment, 1, (const char**)&shader_stage_buffer_fragment->shader_stage_data, NULL);

    //compile the shaders
    glCompileShader(gl_shader_stage_id_vertex);
    glCompileShader(gl_shader_stage_id_fragment);

    //check the compile status
    GLint stage_status_vertex   = 0;
    GLint stage_status_fragment = 0;
    
    glGetShaderiv(gl_shader_stage_id_vertex,   GL_COMPILE_STATUS, &stage_status_vertex);
    glGetShaderiv(gl_shader_stage_id_fragment, GL_COMPILE_STATUS, &stage_status_fragment);
    
    b8 shader_compiled_vertex    = (stage_status_vertex   == GL_TRUE);
    b8 shader_compiled_fragement = (stage_status_fragment == GL_TRUE);

    shader_stage_buffer_vertex->result   = shader_compiled_vertex    ? ITFLIESBY_RENDERER_SHADER_ERROR_OKAY : ITFLIESBY_RENDERER_SHADER_ERROR_FAILED_TO_COMPILE_STAGE_VERTEX; 
    shader_stage_buffer_fragment->result = shader_compiled_fragement ? ITFLIESBY_RENDERER_SHADER_ERROR_OKAY : ITFLIESBY_RENDERER_SHADER_ERROR_FAILED_TO_COMPILE_STAGE_FRAGMENT; 
    
    //if we failed to compile, return    
    if (!shader_compiled_vertex || !shader_compiled_fragement) {
        itfliesby_renderer_shader_destroy(renderer,shader_type);
        return(ITFLIESBY_RENDERER_SHADER_ERROR_FAILED_TO_COMPILE);
    }    

    //now, link the program
    glAttachShader(gl_shader_program_id,gl_shader_stage_id_vertex);
    glAttachShader(gl_shader_program_id,gl_shader_stage_id_fragment);
    glLinkProgram(gl_shader_program_id);
    
    //check the link status
    GLint program_status;
    glGetProgramiv(gl_shader_program_id,GL_LINK_STATUS,(GLint*)&program_status);
    b8 program_linked = (program_status == GL_TRUE);

    //if we failed to link, return
    if (!program_linked) {
        itfliesby_renderer_shader_destroy(renderer,shader_type);
        return(ITFLIESBY_RENDERER_SHADER_ERROR_FAILED_TO_LINK);
    }    

    //sanity check
    ITFLIESBY_ASSERT(
        shader_compiled_vertex    &&
        shader_compiled_fragement &&
        program_linked
    );
    
    //detach shaders
    glDetachShader(gl_shader_program_id,gl_shader_stage_id_vertex);
    glDetachShader(gl_shader_program_id,gl_shader_stage_id_fragment);
    
    //write our shader ids back to the store
    shader->gl_program_id        = gl_shader_program_id;
    shader->gl_stage_id_vertex   = gl_shader_stage_id_vertex;
    shader->gl_stage_id_fragment = gl_shader_stage_id_fragment;

    return(shader_type);
}

internal void
ifliesby_renderer_shader_uniforms_solid_quad(
    ItfliesbyRendererShaderUniformsSolidQuad* solid_quad_uniforms,
    GLuint                                    solid_quad_program_id,
    GLuint                                    solid_quad_update_ubo) {

    solid_quad_uniforms->gl_block_index_solid_quad_update = 
        glGetUniformBlockIndex(
            solid_quad_program_id,
            ITFLIESBY_RENDERER_SHADER_UNIFORM_SOLID_QUAD_UPDATE);

    glUniformBlockBinding(
        solid_quad_program_id,
        solid_quad_uniforms->gl_block_index_solid_quad_update,
        0);

    u32 data_size = ITFLIESBY_RENDERER_SOLID_QUAD_UPDATE_SIZE();

    solid_quad_uniforms->gl_solid_quad_update_ubo = solid_quad_update_ubo;
    
    glBindBuffer(GL_UNIFORM_BUFFER,solid_quad_uniforms->gl_solid_quad_update_ubo);
    glBufferData(GL_UNIFORM_BUFFER,data_size,NULL,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    glBindBufferRange(
        GL_UNIFORM_BUFFER,
        0,
        solid_quad_uniforms->gl_solid_quad_update_ubo,
        0,
        data_size);    
}

external b8
itfliesby_renderer_ready(
    ItfliesbyRenderer* renderer) {
    
    //make sure the renderer has everything it needs
    b8 ready = true;

    ItfliesbyRendererShaderStore*    shader_store = &renderer->shader_store;
    ItfliesbyRendererShader*         shader_array = shader_store->shaders;
    ItfliesbyRendererShader          shader;
    ItfliesbyRendererShaderUniforms* shader_uniforms = &renderer->shader_store.uniforms;

    //make sure the shader ids are initialized
    for (
        u8 shader_index = 0;
        shader_index < ITFLIESBY_RENDERER_SHADER_TYPE_COUNT;
        ++shader_index) {

        shader = shader_array[shader_index];

        ready &= itfliesby_renderer_shader_is_valid(&shader);
    }

    if (!ready) {
        return(false);
    }

    //generate the ubos we will need
    const u32 num_ubos = 1;
    GLuint gl_ubos[num_ubos] = {0};
    glGenBuffers(num_ubos,gl_ubos);

    //solid quad uniform data
    ifliesby_renderer_shader_uniforms_solid_quad(
        &shader_uniforms->solid_quad_uniforms,
        shader_store->types.solid_quad.gl_program_id,
        gl_ubos[0]
    );

    return(ready);
}