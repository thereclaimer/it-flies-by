#ifndef IFB_ENGINE_OPENGL_HPP
#define IFB_ENGINE_OPENGL_HPP

#include "ifb.hpp"

struct IFBEngineOpenglShaderProgram {
    GLuint program;
    GLuint stage_vertex;
    GLuint stage_fragment;
};

struct IFBEngineOpenglShaderUniform {
    IFBTag name_tag;
    GLint  location;
    GLuint program;
};

void
ifb_engine_opengl_program_compile(
    const u32                            shaders_count,
    const char**                         vertex_stage_buffer,
    const char**                         fragment_stage_buffer,
          IFBEngineOpenglShaderProgram*  shader_programs) ;

void
ifb_engine_opengl_uniform_locations(
    const u32                           uniform_count,
    const char**                        uniform_names,
    const GLuint*                       gl_shader_programs,
          IFBEngineOpenglShaderUniform* uniforms);

#endif //IFB_ENGINE_OPENGL_HPP