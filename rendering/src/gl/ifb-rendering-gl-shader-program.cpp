#pragma once

#include "ifb-rendering-gl.hpp"


inline const ifb::b8
ifb::gl::shader_program_create(
    const ifb::u32                     shader_program_count,
    const gl_id_shader_stage_vertex_t*   shader_stage_array_vertex,
    const gl_id_shader_stage_fragment_t* shader_stage_array_fragment,
          gl_id_shader_program_t*       shader_program_array) {

    //sanity check
    ifb_macro_assert(shader_program_count        != 0);
    ifb_macro_assert(shader_stage_array_vertex   != NULL);
    ifb_macro_assert(shader_stage_array_fragment != NULL);
    ifb_macro_assert(shader_program_array        != NULL);

    ifb::b8 result = true;

    GLint shader_program_link_status = 0;

    //loop through the shader programs and create them
    for (
        ifb::u32 shader_program_index = 0;
        shader_program_index < shader_program_count; 
        ++shader_program_index) {
    
        //get the current shader program reference and stage ids
        gl_id_shader_program_t& shader_program_ref       = shader_program_array       [shader_program_index]; 
        const GLuint          shader_stage_id_vertex   = shader_stage_array_vertex  [shader_program_index].gl_id;
        const GLuint          shader_stage_id_fragment = shader_stage_array_fragment[shader_program_index].gl_id;

        //create the program
        shader_program_ref.gl_id = glCreateProgram();
        
        //attach the shaders
        glAttachShader(shader_program_ref.gl_id, shader_stage_id_vertex);
        glAttachShader(shader_program_ref.gl_id, shader_stage_id_fragment);
    
        //link the program
        glLinkProgram(shader_program_ref.gl_id);

        //detach the shaders
        glDetachShader(shader_program_ref.gl_id, shader_stage_id_vertex);
        glDetachShader(shader_program_ref.gl_id, shader_stage_id_fragment);

        //check our program status
        const GLenum error = glGetError();
        glGetProgramiv(shader_program_ref.gl_id,GL_LINK_STATUS,&shader_program_link_status);
        
        //update the result with our shader program status
        result &= shader_program_ref.gl_id   != GL_ZERO;     // shader program should be non-zero
        result &= error                      == GL_NO_ERROR; // OpenGL should be error free
        result &= shader_program_link_status == GL_TRUE;     // shader program should be linked
    }

    //we're done
    return(result);
}

inline const ifb::b8
ifb::gl::shader_program_delete(
    const ifb::u32               shader_program_count,
    const gl_id_shader_program_t* shader_program_array) {

    //sanity check
    ifb_macro_assert(shader_program_count != 0);
    ifb_macro_assert(shader_program_array != NULL);

    //forward declarations
    ifb::b8 result = true;    
    
    //loop through the shader programs and delete them
    for (
        ifb::u32 shader_program_index = 0;
        shader_program_index < shader_program_count; 
        ++shader_program_index) {
    
        const GLuint shader_program_id = shader_program_array[shader_program_index].gl_id;

        glDeleteProgram(shader_program_id);

        const GLenum error = glGetError();
        result &= error == GL_NO_ERROR;
    }

    return(result);
}

inline const ifb::b8 
ifb::gl::shader_program_set_active(
    const gl_id_shader_program_t shader_program) {

    glUseProgram(shader_program.gl_id);

    const GLenum error = glGetError();

    return(error == GL_NO_ERROR);
}