#pragma once

#include "ifb-engine-opengl.hpp"

global u32 _gl_error;

internal void
ifb_engine_opengl_programs_compile(
    const u32                            shaders_count,
    const char**                         vertex_stage_buffer,
    const char**                         fragment_stage_buffer,
          IFBEngineOpenglShaderProgram*  shader_programs) {
    
    IFBEngineOpenglShaderProgram* current_shader_program;
    char*                         current_vertex_stage_buffer;
    char*                         current_fragment_stage_buffer;
    GLint                         current_vertex_compile_status;
    GLint                         current_fragment_compile_status;
    GLint                         current_shader_link_status;

    for (
        u32 shader_index = 0;
        shader_index < shaders_count;
        ++shader_index) {

        //get the current shader info
        current_shader_program        = &shader_programs[shader_index];
        current_vertex_stage_buffer   = &vertex_stage_buffer[shader_index];
        current_fragment_stage_buffer = &fragment_stage_buffer[shader_index];

        //greate the gl ids
        shader_program->program        = glCreateProgram();
        shader_program->stage_vertex   = glCreateShader(GL_VERTEX_SHADER); 
        shader_program->stage_fragment = glCreateShader(GL_FRAGMENT_SHADER); 

        //compile the shaders
        glShaderSource(shader_program->stage_vertex,   1, &current_vertex_stage_buffer,   NULL);
        glShaderSource(shader_program->stage_fragment, 1, &current_fragment_stage_buffer, NULL);

        //sanity check, ensure the shader stages are compiled
        glGetShaderiv(shader_program->stage_vertex,   GL_COMPILE_STATUS, &current_vertex_compile_status);
        glGetShaderiv(shader_program->stage_fragment, GL_COMPILE_STATUS, &current_fragment_compile_status);
        ifb_assert(current_vertex_compile_status   == GL_TRUE);
        ifb_assert(current_fragment_compile_status == GL_TRUE);

        //attach stages and link shader program
        glAttachShader(shader_program->program,shader_program->stage_vertex);
        glAttachShader(shader_program->program,shader_program->stage_fragment);
        glLinkProgram(shader_program->program);

        //sanity check, ensure the shader program has been linked
        glGetProgramiv(shader_program->program,GL_LINK_STATUS,&current_shader_link_status);
        ifb_assert(current_shader_link_status == GL_TRUE);
    }
}

internal void
ifb_engine_opengl_uniform_locations(
    const u32                           uniform_count,
    const char**                        uniform_names,
    const GLuint*                       gl_shader_programs,
          IFBEngineOpenglShaderUniform* uniforms) {

    IFBEngineOpenglShaderUniform* current_uniform;
    const char*                   current_uniform_name;
    GLuint                        current_gl_shader_program;

    for (
        u32 uniform_index = 0;
        uniform_index < uniform_count;
        ++uniform_index) {

        //get the current uniform information
        current_uniform           = &uniforms[uniform_index];
        current_uniform_name      = uniform_names[uniform_index];
        current_gl_shader_program = gl_shader_programs[uniform_index];

        //create the tag and location
        current_uniform->name_tag = ifb_tag(current_uniform_name);
        current_uniform->location = glGetUniformLocation(current_gl_shader_program, current_uniform->name_tag.value);
        current_uniform->program  = current_gl_shader_program;

        //sanity check, make sure we have a valid uniform location
        ifb_assert(current_uniform->location >= 0);
    }
}