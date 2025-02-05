#ifndef IFB_GL_HPP
#define IFB_GL_HPP

#include "ifb-dependencies.hpp"
#include "ifb-graphics.hpp"
#include "ifb-types.hpp"

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

#define ifb_gl_macro_check_error(b8_no_error,gl_error) b8_no_error &= ((gl_error = glGetError()) == GL_NO_ERROR) 

struct IFBGLViewport;

struct IFBGLID { GLuint gl_id; };

struct IFBGLIDShaderStage   : IFBGLID { };
struct IFBGLIDShaderProgram : IFBGLID { };
struct IFBGLIDUniform       : IFBGLID { };
struct IFBGLIDVertex        : IFBGLID { };
struct IFBGLIDBuffer        : IFBGLID { };

struct IFBGLIDShaderStageVertex   : IFBGLIDShaderStage { };   
struct IFBGLIDShaderStageFragment : IFBGLIDShaderStage { }; 

struct IFBGLIDUniformU32  : IFBGLIDUniform { };
struct IFBGLIDUniformF32  : IFBGLIDUniform { };
struct IFBGLIDUniformVec2 : IFBGLIDUniform { };
struct IFBGLIDUniformVec3 : IFBGLIDUniform { };
struct IFBGLIDUniformMat3 : IFBGLIDUniform { };
struct IFBGLIDUniformMat4 : IFBGLIDUniform { };

struct IFBGLIDBufferVertex  : IFBGLIDBuffer { };
struct IFBGLIDBufferElement : IFBGLIDBuffer { };

#define IFB_GL_INVALID_UNIFORM_LOCATION -1

typedef ifb_enum IFBGLVertexAttributeType;

struct IFBGLShaderContext {
    IFBGLIDShaderProgram gl_shader_program;
    IFBGLIDVertex        gl_vertex_object;
};

/**********************************************************************************/
/* VIEWPORT                                                                       */
/**********************************************************************************/

struct IFBGLViewport {
    IFBPosition          position;
    IFBDimensions        dimensions;
    IFBColor clear_color;
};

namespace ifb_gl{

    ifb_void viewport_initialize                  (IFBGLViewport* viewport);
    ifb_void viewport_clear                       (IFBGLViewport* viewport);
    ifb_void viewport_set_clear_color             (IFBGLViewport* viewport);
    ifb_void viewport_set_position_and_dimensions (IFBGLViewport* viewport);
};

inline ifb_void 
ifb_gl::viewport_initialize(
    IFBGLViewport* viewport) {

    //initialize glew
    ifb_macro_assert(glewInit() == GLEW_OK); 
 
    //set viewport position, dimensions, and clear color
    ifb_gl::viewport_set_clear_color(viewport);
    ifb_gl::viewport_set_position_and_dimensions(viewport);    

    //other gl configurations
	glHint     (GL_SAMPLES, 4);
	glEnable   (GL_MULTISAMPLE);
	glEnable   (GL_BLEND);
	glEnable   (GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

inline ifb_void 
ifb_gl::viewport_clear(
    IFBGLViewport* viewport) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline ifb_void 
ifb_gl::viewport_set_clear_color(
    IFBGLViewport* viewport) {

    //set the clear color
    glClearColor(
        viewport->clear_color.red,
        viewport->clear_color.green,
        viewport->clear_color.blue,
        viewport->clear_color.alpha);
}

inline ifb_void 
ifb_gl::viewport_set_position_and_dimensions(
    IFBGLViewport* viewport) {

    //set viewport position and dimensions
    glViewport(
        viewport->position.x,
        viewport->position.y,
        viewport->dimensions.width,
        viewport->dimensions.height);
}

/**********************************************************************************/
/* SHADER STAGE                                                                   */
/**********************************************************************************/

namespace ifb_gl {

    const ifb_b8
    shader_stage_compile(
        const GLuint              shader_stage_type,
        const ifb_u32             shader_stage_count,
        const ifb_char**          shader_stage_buffers,
              IFBGLIDShaderStage* shader_stage_array); 
    
    const ifb_b8 shader_stage_compile_vertex   (const ifb_u32 shader_stage_vertex_count,   const ifb_char** shader_stage_vertex_buffers,   IFBGLIDShaderStageVertex*   shader_stage_vertex_array); 
    const ifb_b8 shader_stage_compile_fragment (const ifb_u32 shader_stage_fragment_count, const ifb_char** shader_stage_fragment_buffers, IFBGLIDShaderStageFragment* shader_stage_fragment_array); 
    
    const ifb_b8
    shader_stage_delete(
        const ifb_u32           shader_stage_count,
        const IFBGLIDShaderStage* shader_stage_array);
};

inline const ifb_b8
ifb_gl::shader_stage_compile(
    const GLuint              shader_stage_type,
    const ifb_u32             shader_stage_count,
    const ifb_char**          shader_stage_buffers,
          IFBGLIDShaderStage* shader_stage_array) {

    //sanity check
    ifb_macro_assert(shader_stage_count   != 0);
    ifb_macro_assert(shader_stage_buffers != NULL);
    ifb_macro_assert(shader_stage_array   != NULL);

    //forward declarations
    ifb_b8 result                = true;
    GLint  shader_stage_compiled = 0;

    //iterate over the shader stage buffers and compile each one
    for (
        ifb_u32 shader_stage_index = 0;
        shader_stage_index < shader_stage_count;
        ++shader_stage_index) {

        //get the current shader stage and buffer
        IFBGLIDShaderStage& shader_stage_ref    = shader_stage_array[shader_stage_index];
        const ifb_char*     shader_stage_buffer = shader_stage_buffers[shader_stage_index];
        
        //compile the shader stage
        shader_stage_ref.gl_id = glCreateShader(shader_stage_type); 

        //check the shader stage status
        const GLenum last_error = glGetError();
        glGetShaderiv(shader_stage_ref.gl_id,GL_COMPILE_STATUS,&shader_stage_compiled);

        //update the result
        result &= shader_stage_ref.gl_id != GL_ZERO;     // the shader stage id should be non-zero
        result &= last_error             == GL_NO_ERROR; // OpenGL should have no errors
        result &= shader_stage_compiled  == GL_TRUE;     // the shader stage should be compiled 
    }

    //we're done
    return(result);
}

inline const ifb_b8
ifb_gl::shader_stage_compile_vertex(
    const ifb_u32                   shader_stage_vertex_count,
    const ifb_char**                shader_stage_vertex_buffers,
          IFBGLIDShaderStageVertex* shader_stage_vertex_array) {

    const ifb_b8 result = ifb_gl::shader_stage_compile(
        GL_VERTEX_SHADER,
        shader_stage_vertex_count,
        shader_stage_vertex_buffers,
        shader_stage_vertex_array);

    return(result);
}

inline const ifb_b8
ifb_gl::shader_stage_compile_fragment(
    const ifb_u32                     shader_stage_fragment_count,
    const ifb_char**                  shader_stage_fragment_buffers,
          IFBGLIDShaderStageFragment* shader_stage_fragment_array) {

    const ifb_b8 result = ifb_gl::shader_stage_compile(
        GL_FRAGMENT_SHADER,
        shader_stage_fragment_count,
        shader_stage_fragment_buffers,
        shader_stage_fragment_array);

    return(result);
}

inline const ifb_b8
ifb_gl::shader_stage_delete(
    const ifb_u32             shader_stage_count,
    const IFBGLIDShaderStage* shader_stage_array) {

    ifb_macro_assert(shader_stage_count != 0);
    ifb_macro_assert(shader_stage_array != NULL);

    ifb_b8 result = true;

    for (
        ifb_u32 shader_stage_index = 0;
        shader_stage_index < shader_stage_count;
        ++shader_stage_index) {

        const GLuint gl_shader_id = shader_stage_array[shader_stage_index].gl_id;

        glDeleteShader(gl_shader_id);

        const GLenum last_error = glGetError();

        result &= last_error == GL_NO_ERROR;
    }

    return(result);
}

/**********************************************************************************/
/* SHADER PROGRAM                                                                 */
/**********************************************************************************/

namespace ifb_gl {

    const ifb_b8
    shader_program_create(
        const ifb_u32                     shader_program_count,
        const IFBGLIDShaderStageVertex*   shader_stage_array_vertex,
        const IFBGLIDShaderStageFragment* shader_stage_array_fragment,
              IFBGLIDShaderProgram*       shader_program_array);

    const ifb_b8
    shader_program_delete(
        const ifb_u32               shader_program_count,
        const IFBGLIDShaderProgram* shader_program_array);

    const ifb_b8 
    shader_program_set_active(const IFBGLIDShaderProgram shader_program);
};

inline const ifb_b8
ifb_gl::shader_program_create(
    const ifb_u32                     shader_program_count,
    const IFBGLIDShaderStageVertex*   shader_stage_array_vertex,
    const IFBGLIDShaderStageFragment* shader_stage_array_fragment,
          IFBGLIDShaderProgram*       shader_program_array) {

    //sanity check
    ifb_macro_assert(shader_program_count        != 0);
    ifb_macro_assert(shader_stage_array_vertex   != NULL);
    ifb_macro_assert(shader_stage_array_fragment != NULL);
    ifb_macro_assert(shader_program_array        != NULL);

    ifb_b8 result = true;

    GLint shader_program_link_status = 0;

    //loop through the shader programs and create them
    for (
        ifb_u32 shader_program_index = 0;
        shader_program_index < shader_program_count; 
        ++shader_program_index) {
    
        //get the current shader program reference and stage ids
        IFBGLIDShaderProgram& shader_program_ref       = shader_program_array       [shader_program_index]; 
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

inline const ifb_b8
ifb_gl::shader_program_delete(
    const ifb_u32               shader_program_count,
    const IFBGLIDShaderProgram* shader_program_array) {

    //sanity check
    ifb_macro_assert(shader_program_count != 0);
    ifb_macro_assert(shader_program_array != NULL);

    //forward declarations
    ifb_b8 result = true;    
    
    //loop through the shader programs and delete them
    for (
        ifb_u32 shader_program_index = 0;
        shader_program_index < shader_program_count; 
        ++shader_program_index) {
    
        const GLuint shader_program_id = shader_program_array[shader_program_index].gl_id;

        glDeleteProgram(shader_program_id);

        const GLenum error = glGetError();
        result &= error == GL_NO_ERROR;
    }

    return(result);
}

inline const ifb_b8 
ifb_gl::shader_program_set_active(
    const IFBGLIDShaderProgram shader_program) {

    glUseProgram(shader_program.gl_id);

    const GLenum error = glGetError();

    return(error == GL_NO_ERROR);
}

/**********************************************************************************/
/* UNIFORMS                                                                       */
/**********************************************************************************/

namespace ifb_gl {

    const ifb_b8 
    uniform_get_locations_in_shader_program(
        const IFBGLIDShaderProgram program,
        const ifb_u32              uniform_count,
        const ifb_char**           uniform_names,
              IFBGLIDUniform*      uniform_array);

    const ifb_b8 uniform_update_u32  (const IFBGLIDUniformU32  uniform, const ifb_u32 count, const ifb_u32* u32);
    const ifb_b8 uniform_update_f32  (const IFBGLIDUniformF32  uniform, const ifb_u32 count, const ifb_f32* f32);
    const ifb_b8 uniform_update_vec2 (const IFBGLIDUniformVec2 uniform, const ifb_u32 count, const IFBVec2* vec2);
    const ifb_b8 uniform_update_vec3 (const IFBGLIDUniformVec3 uniform, const ifb_u32 count, const IFBVec3* vec3);
    const ifb_b8 uniform_update_mat3 (const IFBGLIDUniformMat3 uniform, const ifb_u32 count, const IFBMat3* mat3);
};

inline const ifb_b8 
ifb_gl::uniform_get_locations_in_shader_program(
    const IFBGLIDShaderProgram program,
    const ifb_u32              uniform_count,
    const ifb_char**           uniform_names,
          IFBGLIDUniform*      uniform_array) {

    //sanity check
    ifb_macro_assert(program.gl_id != GL_ZERO);
    ifb_macro_assert(uniform_count != 0);
    ifb_macro_assert(uniform_names != NULL);
    ifb_macro_assert(uniform_array != NULL);

    //forward declarations
    ifb_b8       result        = true;
    const GLuint gl_program_id = program.gl_id;

    //loop throught the uniform names and query the program for their location
    for (
        ifb_u32 uniform_index = 0;
        uniform_index < uniform_count;
        ++uniform_index) {

        //get the current uniform reference and name
        IFBGLIDUniform& uniform_ref  = uniform_array[uniform_index];  
        const ifb_char* uniform_name = uniform_names[uniform_index];

        //query for the uniform location
        uniform_ref.gl_id = glGetUniformLocation(gl_program_id,uniform_name);

        //check for errors        
        const GLenum error = glGetError();
        
        //update the result
        result &= uniform_ref.gl_id != IFB_GL_INVALID_UNIFORM_LOCATION; // the location should be valid (not -1)
        result &= error             == GL_NO_ERROR;                     // no opengl errors
    }

    //we're done
    return(result);
}

inline const ifb_b8 
ifb_gl::uniform_update_u32(
    const IFBGLIDUniformU32 uniform,     
    const ifb_u32           count,
    const ifb_u32*          u32) {

    glUniform1uiv(
        uniform.gl_id,
        count,
        u32);

    const GLenum error = glGetError();

    return(error == GL_NO_ERROR);
}

inline const ifb_b8 
ifb_gl::uniform_update_f32(
    const IFBGLIDUniformF32 uniform,     
    const ifb_u32           count,
    const ifb_f32*          f32) {

    glUniform1fv(
        uniform.gl_id,
        count,
        f32);

    const GLenum error = glGetError();

    return(error == GL_NO_ERROR);
}

inline const ifb_b8 
ifb_gl::uniform_update_vec2(
    const IFBGLIDUniformVec2 uniform,
    const ifb_u32            count,
    const IFBVec2*           vec2) {

    glUniform2fv(
        uniform.gl_id,
        count,
        (ifb_f32*)vec2);

    const GLenum error = glGetError();

    return(error == GL_NO_ERROR);
}

inline const ifb_b8 
ifb_gl::uniform_update_vec3(
    const IFBGLIDUniformVec3 uniform,
    const ifb_u32            count,
    const IFBVec3*           vec3) {

    glUniform3fv(
        uniform.gl_id,
        count,
        (ifb_f32*)vec3);

    const GLenum error = glGetError();

    return(error == GL_NO_ERROR);
}

inline const ifb_b8 
ifb_gl::uniform_update_mat3(
    const IFBGLIDUniformMat3 uniform,
    const ifb_u32            count,
    const IFBMat3*           mat3) {

    glUniformMatrix3fv(
        uniform.gl_id,
        count,
        GL_FALSE, //no transpose
        (ifb_f32*)mat3);

    const GLenum error = glGetError();

    return(error == GL_NO_ERROR);
}

/**********************************************************************************/
/* BUFFERS                                                                        */
/**********************************************************************************/

namespace ifb_gl {

    const ifb_b8 buffer_create(const ifb_u32 buffer_count, IFBGLIDBuffer* buffer_array);

    const ifb_b8 buffer_create_vertex  (const ifb_u32 vertex_buffer_count,  IFBGLIDBufferVertex*  vertex_buffer_array);
    const ifb_b8 buffer_create_element (const ifb_u32 element_buffer_count, IFBGLIDBufferElement* element_buffer_array);

    const ifb_b8
    buffer_data_upload_vertex(
        const ifb_u32              vertex_buffer_count,
        const IFBGLIDBufferVertex* vertex_buffer_array,
        const ifb_u32*             vertex_buffer_size_array,
        const ifb_void*            vertex_buffer_data);

    const ifb_b8
    buffer_data_upload_element(
        const ifb_u32               element_buffer_count,
        const IFBGLIDBufferElement* element_buffer_array,
        const ifb_u32*              element_buffer_size_array,
        const ifb_void*             element_buffer_data);
};

inline const ifb_b8 
ifb_gl::buffer_create(
    const ifb_u32        buffer_count, 
          IFBGLIDBuffer* buffer_array) {

    //sanity check
    ifb_macro_assert(buffer_count != 0);
    ifb_macro_assert(buffer_array != NULL);

    //forward declarations
    ifb_b8 result   = true;
    GLenum gl_error = 0;

    //generate the buffers
    glGenBuffers(buffer_count,(GLuint*)buffer_array);

    //check errors
    ifb_gl_macro_check_error(result,gl_error);

    //we're done
    return(result);
}

inline const ifb_b8 
ifb_gl::buffer_create_vertex(
    const ifb_u32              vertex_buffer_count,
          IFBGLIDBufferVertex* vertex_buffer_array) {

    const ifb_b8 result = ifb_gl::buffer_create(
        vertex_buffer_count,
        vertex_buffer_array);

    return(result);
}

inline const ifb_b8 
ifb_gl::buffer_create_element(
    const ifb_u32               element_buffer_count,
          IFBGLIDBufferElement* element_buffer_array) {
    
    const ifb_b8 result = ifb_gl::buffer_create(
        element_buffer_count,
        element_buffer_array);

    return(result);
}

inline const ifb_b8
ifb_gl::buffer_data_upload_vertex(
    const ifb_u32              vertex_buffer_count,
    const IFBGLIDBufferVertex* vertex_buffer_array,
    const ifb_u32*             vertex_buffer_size_array,
    const ifb_void*            vertex_buffer_data) {

    //sanity check
    ifb_macro_assert(vertex_buffer_count      != 0);
    ifb_macro_assert(vertex_buffer_array      != NULL);
    ifb_macro_assert(vertex_buffer_size_array != NULL);
    ifb_macro_assert(vertex_buffer_data       != NULL);

    //forward declarations
    ifb_u32 buffer_offset = 0;
    ifb_b8  result        = true;
    GLenum  gl_error      = 0;

    for (
        ifb_u32 vertex_buffer_index = 0;
        vertex_buffer_index < vertex_buffer_count;
        ++vertex_buffer_index) {

        //get the buffer id, size, and data
        const GLuint    buffer_id   = vertex_buffer_array     [vertex_buffer_index].gl_id;
        const ifb_u32   buffer_size = vertex_buffer_size_array[vertex_buffer_index];
        const ifb_void* buffer_data = (ifb_byte*)vertex_buffer_data + buffer_offset;

        //bind the buffer
        glBindBuffer(GL_ARRAY_BUFFER,buffer_id);

        //set the data
        glBufferData(
            GL_ARRAY_BUFFER,
            buffer_size,
            buffer_data,
            GL_STATIC_DRAW);

        //check errors
        ifb_gl_macro_check_error(result,gl_error);

        //update the offset
        buffer_offset += buffer_size;
    }

    //we're done
    return(result);
}

inline const ifb_b8
ifb_gl::buffer_data_upload_element(
    const ifb_u32               element_buffer_count,
    const IFBGLIDBufferElement* element_buffer_array,
    const ifb_u32*              element_buffer_size_array,
    const ifb_void*             element_buffer_data) {

    //sanity check
    ifb_macro_assert(element_buffer_count      != 0);
    ifb_macro_assert(element_buffer_array      != NULL);
    ifb_macro_assert(element_buffer_size_array != NULL);
    ifb_macro_assert(element_buffer_data       != NULL);

    //forward declarations
    ifb_u32 buffer_offset = 0;
    ifb_b8  result        = true;
    GLenum  gl_error      = 0;

    for (
        ifb_u32 element_buffer_index = 0;
        element_buffer_index < element_buffer_index;
        ++element_buffer_index) {

        //get the buffer id, size, and data
        const GLuint    buffer_id   = element_buffer_array     [element_buffer_index].gl_id;
        const ifb_u32   buffer_size = element_buffer_size_array[element_buffer_index];
        const ifb_void* buffer_data = (ifb_byte*)element_buffer_data + buffer_offset;

        //bind the buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer_id);

        //set the data
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            buffer_size,
            buffer_data,
            GL_STATIC_DRAW);

        //check errors
        ifb_gl_macro_check_error(result,gl_error);

        //update the offset
        buffer_offset += buffer_size;
    }

    //we're done
    return(result);
}

/**********************************************************************************/
/* VERTEX                                                                         */
/**********************************************************************************/

enum IFBGLVertexAttributeType_ {
    IFBGLVertexAttributeType_S8  = 0,
    IFBGLVertexAttributeType_U8  = 1,
    IFBGLVertexAttributeType_S16 = 2,
    IFBGLVertexAttributeType_U16 = 3,
    IFBGLVertexAttributeType_S32 = 4,
    IFBGLVertexAttributeType_U32 = 5,
    IFBGLVertexAttributeType_F32 = 6
};

ifb_global const ifb_u32 IFB_GL_VERTEX_ATTRIBUTE_GL_TYPES[] = {
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_FLOAT
};

ifb_global const ifb_u32 IFB_GL_VERTEX_ATTRIBUTE_SIZES[] = {
    sizeof(ifb_s8),  // GL_BYTE
    sizeof(ifb_u8),  // GL_UNSIGNED_BYTE
    sizeof(ifb_s16), // GL_SHORT
    sizeof(ifb_u16), // GL_UNSIGNED_SHORT
    sizeof(ifb_s32), // GL_INT
    sizeof(ifb_u32), // GL_UNSIGNED_INT
    sizeof(ifb_f32)  // GL_FLOAT
};

namespace ifb_gl {

    const ifb_b8
    vertex_create(
        const ifb_u32        vertex_count,
              IFBGLIDVertex* vertex_array);

    const ifb_b8
    vertex_enable_attributes(
        const IFBGLIDVertex             vertex_ref,
        const ifb_u32                   vertex_size,
        const IFBGLIDBuffer             vertex_buffer,
        const ifb_address               vertex_buffer_offset,
        const ifb_u32                   vertex_attribute_count,
        const IFBGLVertexAttributeType* vertex_attribute_types_array,
        const ifb_u32*                  vertex_attribute_base_offset_array);

    const GLenum  vertex_attribute_type_get_gl_type (const IFBGLVertexAttributeType vertex_attribute_type);
    const ifb_u32 vertex_attribute_type_get_size    (const IFBGLVertexAttributeType vertex_attribute_type);
};

inline const ifb_b8
ifb_gl::vertex_create(
    const ifb_u32        vertex_count,
          IFBGLIDVertex* vertex_array) {

    //forward declarations
    ifb_b8 result   = true;
    GLenum gl_error = 0;  
    
    //sanity check
    ifb_macro_assert(vertex_count != 0);
    ifb_macro_assert(vertex_array != NULL);    

    //create the vertex arrays
    glCreateVertexArrays(vertex_count,(GLuint*)vertex_array);

    //check errors
    ifb_gl_macro_check_error(result,gl_error);

    //unbind any vertex arrays
    glBindVertexArray(0);
    
    //final error check
    ifb_gl_macro_check_error(result,gl_error);

    //we're done
    return(result);
}

inline const ifb_b8
ifb_gl::vertex_enable_attributes(
    const IFBGLIDVertex             vertex,
    const ifb_u32                   vertex_size,
    const IFBGLIDBuffer             vertex_buffer,
    const ifb_address               vertex_buffer_offset,
    const ifb_u32                   vertex_attribute_count,
    const IFBGLVertexAttributeType* vertex_attribute_types_array,
    const ifb_u32*                  vertex_attribute_base_offset_array) {

    //sanity check
    ifb_macro_assert(vertex_size                        != 0);
    ifb_macro_assert(vertex_attribute_count             != 0);
    ifb_macro_assert(vertex_attribute_types_array       != NULL);
    ifb_macro_assert(vertex_attribute_base_offset_array != NULL);

    //forward declarations
    ifb_b8 result   = true;
    GLenum gl_error = 0;

    //bind the vertex array
    glBindVertexArray(vertex.gl_id);
    ifb_gl_macro_check_error(result,gl_error);

    //bind the vertex buffer
    glBindVertexBuffer(
        0,
        vertex_buffer.gl_id,
        vertex_buffer_offset,
        vertex_size);

    //loop through the attributes and enable them
    for (
        ifb_u32 vertex_attribute_index = 0;
        vertex_attribute_index < vertex_attribute_count;
        ++vertex_attribute_index) {

        //get the current attribute
        const IFBGLVertexAttributeType vertex_attribute_type = vertex_attribute_types_array[vertex_attribute_index];

        //get the gl type, attribute size, and offset
        const GLenum    vertex_attribute_gl_type = ifb_gl::vertex_attribute_type_get_gl_type (vertex_attribute_type);
        const ifb_u32   vertex_attribute_size    = ifb_gl::vertex_attribute_type_get_size    (vertex_attribute_type);
        const ifb_u32   vertex_attribute_offset  = vertex_attribute_base_offset_array[vertex_attribute_index];
        const ifb_void* vertex_attribute_ptr     = (ifb_void*)(vertex_buffer_offset + vertex_attribute_offset); 

        //enable the attribute
        glEnableVertexAttribArray(vertex_attribute_index);
        glVertexAttribPointer(
            vertex_attribute_index,
            vertex_attribute_size,
            vertex_attribute_gl_type,
            GL_FALSE,
            vertex_size,
            vertex_attribute_ptr);

        //check errors
        ifb_gl_macro_check_error(result,gl_error);
    }

    //unbind the vertex array
    glBindVertexArray(0);

    //last error check
    ifb_gl_macro_check_error(result,gl_error);

    //we're done
    return(result);
}

inline const GLenum 
ifb_gl::vertex_attribute_type_get_gl_type(
    const IFBGLVertexAttributeType vertex_attribute_type) {

    return(IFB_GL_VERTEX_ATTRIBUTE_GL_TYPES[vertex_attribute_type]);
}

inline const ifb_u32 
ifb_gl::vertex_attribute_type_get_size(
    const IFBGLVertexAttributeType vertex_attribute_type) {

    return(IFB_GL_VERTEX_ATTRIBUTE_SIZES[vertex_attribute_type]);
}

#endif //IFB_GL_HPP