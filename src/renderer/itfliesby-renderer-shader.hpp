#ifndef ITFLIESBY_RENDERER_SHADER_HPP
#define ITFLIESBY_RENDERER_SHADER_HPP

#include <ifb.hpp>
#include <common/itfliesby-platform-api.hpp>
#include <common/itfliesby-memory.hpp>
#include <math/itfliesby-math.hpp>
#include "itfliesby-renderer-opengl.hpp"
#include "itfliesby-renderer-types.hpp"
#include "itfliesby-renderer-texture.hpp"

struct ItfliesbyRendererShader {
    GLuint gl_program_id;
    GLuint gl_stage_id_vertex;
    GLuint gl_stage_id_fragment;
};


typedef s8 ItfliesbyRendererShaderIndex;

enum ItfliesbyRendererShaderError {
    ITFLIESBY_RENDERER_SHADER_ERROR_OKAY                             = 0x01,
    ITFLIESBY_RENDERER_SHADER_ERROR                                  = 0x80,
    ITFLIESBY_RENDERER_SHADER_ERROR_FAILED_TO_COMPILE                = 0x81,
    ITFLIESBY_RENDERER_SHADER_ERROR_FAILED_TO_COMPILE_STAGE_VERTEX   = 0x82,
    ITFLIESBY_RENDERER_SHADER_ERROR_FAILED_TO_COMPILE_STAGE_FRAGMENT = 0x83,
    ITFLIESBY_RENDERER_SHADER_ERROR_FAILED_TO_LINK                   = 0x84,
    ITFLIESBY_RENDERER_SHADER_ERROR_MAX_SHADERS                      = 0x85
};

enum ItfliesbyRendererShaderType : s32 {
    ITFLIESBY_RENDERER_SHADER_TYPE_INVALID       = -1,
    ITFLIESBY_RENDERER_SHADER_TYPE_TEXTURED_QUAD =  0,
    ITFLIESBY_RENDERER_SHADER_TYPE_SOLID_QUAD    =  1,
    ITFLIESBY_RENDERER_SHADER_TYPE_TEST          =  2,
    ITFLIESBY_RENDERER_SHADER_TYPE_COUNT         =  3,
};

struct ItfliesbyRendererShaderStageBuffer {
    memory                       shader_stage_data;
    ItfliesbyRendererShaderError result;
};

#define ITFLIESBY_RENDERER_SHADER_UNIFORM_SOLID_QUAD_UPDATE        "solid_quad_update"
#define ITFLIESBY_RENDERER_SHADER_UNIFORM_SOLID_QUAD_UPDATE_MODEL  "model"
#define ITFLIESBY_RENDERER_SHADER_UNIFORM_SOLID_QUAD_UPDATE_COLOR  "color"
#define ITFLIESBY_RENDERER_SHADER_UNIFORM_SOLID_QUAD_BINDING_POINT 0

struct ItfliesbyRendererShaderUniformsSolidQuad {
    union {
        struct {
            GLuint index_model;
            GLuint index_color;
        };
        GLuint indices[2];
    };
    
    union {
        struct {
            GLint offset_model;
            GLint offset_color;
        };
        GLint offsets[2];
    };
    
    GLint   block_data_size;
    GLuint  gl_block_index_solid_quad_update;
    GLuint  gl_solid_quad_update_ubo;
    GLbyte* gl_uniform_buffer_memory;
};

#define ITFLIESBY_RENDERER_SHADER_SIMPLE_QUAD_VAO_INDEX_VERTICES       0
#define ITFLIESBY_RENDERER_SHADER_SIMPLE_QUAD_VAO_INDEX_UV             1
#define ITFLIESBY_RENDERER_SHADER_SIMPLE_QUAD_UNIFORM_TRANSFORM        "transform"
#define ITFLIESBY_RENDERER_SHADER_SIMPLE_QUAD_UNIFORM_COLOR            "color"
#define ITFLIESBY_RENDERER_SHADER_SIMPLE_QUAD_UNIFORM_TEXTURE_SAMPLER  "texture_sampler"


struct ItfliesbyRendererShaderUniformsSimpleQuad{
    
    union {
        struct {
            GLuint index_transform;
            GLuint index_color;
            GLuint index_texture_sampler;
        };
        GLuint indices[3];
    };
};

struct ItfliesbyRendererShaderBuffersSimpleQuad {
    GLuint gl_vao;
    GLuint gl_vbo_quad_indices;
    GLuint gl_vbo_quad_uv;
};


#define ITFLIESBY_RENDERER_TEST_BATCH_COUNT_MAX 32

struct ItfliesbyRendererSimpleQuad {
    IFBMathMat3                 transform;
    ItfliesbyRendererColorNormalized  color;
    ItfliesbyRendererTextureId        texture;
};

struct ItfliesbyRendererBatchSimpleQuad {
    IFBMathMat3                transform[ITFLIESBY_RENDERER_TEST_BATCH_COUNT_MAX];
    ItfliesbyRendererColorNormalized color[ITFLIESBY_RENDERER_TEST_BATCH_COUNT_MAX];
    ItfliesbyRendererTextureId       texture[ITFLIESBY_RENDERER_TEST_BATCH_COUNT_MAX];
    size_t                           count;
};

struct ItfliesbyRendererShaderUniforms {
    ItfliesbyRendererShaderUniformsSolidQuad  solid_quad_uniforms;
    ItfliesbyRendererShaderUniformsSimpleQuad simple_quad;
};

struct ItfliesbyRendererShaderStore {

    union {
        struct {
            ItfliesbyRendererShader textured_quad;
            ItfliesbyRendererShader solid_quad;
            ItfliesbyRendererShader simple_quad;
        } types;

        ItfliesbyRendererShader shaders[ITFLIESBY_RENDERER_SHADER_TYPE_COUNT];
    };

    ItfliesbyRendererShaderUniforms uniforms;
};

#endif //ITFLIESBY_RENDERER_SHADER_HPP