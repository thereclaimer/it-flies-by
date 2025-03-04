#ifndef IFB_RENDERING_HPP
#define IFB_RENDERING_HPP

#include "ifb-types.hpp"
#include "ifb-memory.hpp"

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

//-------------------
// i-ds
//--------------------

// struct IFBIDShader             : IFBID { };
// struct IFBIDBufferVertex       : IFBID { };
// struct IFBIDBufferElement      : IFBID { };
// struct IFBIDUniformU32         : IFBID { };
// struct IFBIDUniformF32         : IFBID { };
// struct IFBIDUniformVec2        : IFBID { };
// struct IFBIDUniformVec3        : IFBID { };
// struct IFBIDUniformMat3        : IFBID { };
// struct IFBIDUniformColor       : IFBID { };
// struct IFBIDUniformSpriteAtlas : IFBID { };

// //--------------------
// // structs
// //--------------------

// struct IFBShader;
// struct IFBBufferVertex;
// struct IFBBufferElement;
// struct IFBUniformHeader;
// struct IFBUniformU32;
// struct IFBUniformF32;
// struct IFBUniformVec2;
// struct IFBUniformVec3;
// struct IFBUniformMat3;
// struct IFBUniformColor;
// struct IFBTransform;

/**********************************************************************************/
/* SHADERS                                                                        */
/**********************************************************************************/

// struct IFBShader : IFBIDShader {
//     IFBIDArena            arena_id;              
//     IFBGLIDShaderProgram  gl_shader_program;
//     IFBGLIDVertex         gl_vertex;
//     IFBAddr           vertex_buffer_start;
//     IFBU32               vertex_buffer_size;
//     IFBU32               vertex_buffer_position;
// };

// /**********************************************************************************/
// /* BUFFERS                                                                        */
// /**********************************************************************************/

// struct IFBBufferVertex : IFBIDBufferVertex {
//     IFBIDTag            tag;
//     IFBGLIDBufferVertex gl_vertex_buffer;
//     IFBU32             size;
//     IFBU32             stride;
//     IFBHND              data_handle;
// };

// struct IFBBufferElement : IFBIDBufferElement {
//     IFBU32              count;
//     IFBU32*             data;
//     IFBIDTag             tag;
//     IFBGLIDBufferElement gl_element_buffer; 
// };

// /**********************************************************************************/
// /* UNIFORMS                                                                       */
// /**********************************************************************************/

// struct IFBUniformU32 : IFBIDUniformU32 {
//     IFBIDShader    shader;
//     IFBIDTag       tag;
//     IFBGLIDUniform gl_uniform;
//     IFBU32        count;
// };

// struct IFBUniformF32 : IFBIDUniformF32 {
//     IFBIDShader    shader;
//     IFBIDTag       tag;
//     IFBGLIDUniform gl_uniform;
//     IFBU32        count;
// };

// struct IFBUniformVec2 : IFBIDUniformVec2 {
//     IFBIDShader    shader;
//     IFBIDTag       tag;
//     IFBGLIDUniform gl_uniform;
//     IFBU32        count;
// };

// struct IFBUniformVec3 : IFBIDUniformVec3 {
//     IFBIDShader    shader;
//     IFBIDTag       tag;
//     IFBGLIDUniform gl_uniform;
//     IFBU32        count;
// };

// struct IFBUniformMat3 : IFBIDUniformMat3 {
//     IFBIDShader    shader;
//     IFBIDTag       tag;
//     IFBGLIDUniform gl_uniform;
//     IFBU32        count;
// };

// struct IFBUniformColor : IFBIDUniformColor {
//     IFBIDShader    shader;
//     IFBIDTag       tag;
//     IFBGLIDUniform gl_uniform;
//     IFBU32        count;
// };

// struct IFBUniformSpriteAtlas : IFBIDUniformSpriteAtlas {
//     IFBIDShader    shader;
//     IFBIDTag       tag;
//     IFBGLIDUniform gl_uniform;
//     IFBU32        count;
// };

#endif //IFB_RENDERING_HPP