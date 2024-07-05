#ifndef IFB_ENGINE_RENDERER_OPENGL_HPP
#define IFB_ENGINE_RENDERER_OPENGL_HPP

#include "ifb-engine-renderer.hpp"

inline GLuint
ifb_engine_renderer_opengl_shader_stage_vertex_compile(
    const char* shader_stage_buffer) {

    //create the shader
    GLuint gl_shader_stage_id_vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        gl_shader_stage_id_vertex,
        1,
        &shader_stage_buffer,
        NULL);

    //compile the shader
    glCompileShader(gl_shader_stage_id_vertex);

    GLint compile_status = 0;
    glGetShaderiv(gl_shader_stage_id_vertex, GL_COMPILE_STATUS, &compile_status);

    //opengl sanity check
    u32 gl_error = glGetError();
    ifb_assert(gl_error == GL_NO_ERROR); 

    return(gl_shader_stage_id_vertex);
}

inline GLuint
ifb_engine_renderer_opengl_shader_stage_fragment_compile(
    const char* shader_stage_buffer) {

    //create the shader
    GLuint gl_shader_stage_id_fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        gl_shader_stage_id_fragment,
        1,
        &shader_stage_buffer,
        NULL);

    //compile the shader
    glCompileShader(gl_shader_stage_id_fragment);

    GLint compile_status = 0;
    glGetShaderiv(gl_shader_stage_id_fragment, GL_COMPILE_STATUS, &compile_status);

    //opengl sanity check
    u32 gl_error = glGetError();
    ifb_assert(gl_error == GL_NO_ERROR); 

    return(gl_shader_stage_id_fragment);
}

inline GLuint
ifb_engine_renderer_opengl_program_create_and_link(
    GLuint gl_shader_stage_id_vertex,
    GLuint gl_shader_stage_id_fragment) {

    //create the program
    GLuint gl_shader_program_id = glCreateProgram();
    
    //attach the shaders
    glAttachShader(gl_shader_program_id,gl_shader_stage_id_vertex);
    glAttachShader(gl_shader_program_id,gl_shader_stage_id_fragment);
    
    //link the program
    glLinkProgram(gl_shader_program_id);
    
    GLint link_status;
    glGetProgramiv(gl_shader_program_id,GL_LINK_STATUS,&link_status);
    ifb_assert(link_status = GL_TRUE);

    //opengl sanity check
    u32 gl_error = glGetError();
    ifb_assert(gl_error == GL_NO_ERROR); 

    return(gl_shader_program_id);
}

#endif //IFB_ENGINE_RENDERING_OPENGL_HPP