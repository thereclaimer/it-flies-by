#ifndef ITFLIESBY_RENDERER_QUAD_HPP
#define ITFLIESBY_RENDERER_QUAD_HPP

#include <ifb.hpp>
#include <common/itfliesby-platform-api.hpp>
#include <common/itfliesby-memory.hpp>
#include <math/itfliesby-math.hpp>

#include "itfliesby-renderer-opengl.hpp"
#include "itfliesby-renderer-types.hpp"

struct  ItfliesbyRendererQuadBuffers {
    union {
    
        struct {
            GLuint indices; 
            GLuint square_quad_vertices; 
        } instances;
        
        GLuint array[sizeof(instances) / sizeof(GLuint)];
    };
};

#define ITFLIESBY_RENDERER_QUAD_BUFFERS_COUNT (sizeof(ItfliesbyRendererQuadBuffers::instances) / sizeof(GLuint))
#define ITFLIESBY_RENDERER_SOLID_QUADS_MAX 128

const f32 ITFLIESBY_RENDERER_VERTICES_SQUARE_QUAD[] = {
    -0.5f,  0.5f,
     0.5f, -0.5f,
    -0.5f, -0.5f,
     0.5f,  0.5f
};

const f32 ITFLIESBY_RENDERER_UV_SQUARE_QUAD[] = {
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 1.0f
};

const u32 ITFLIESBY_RENDERER_INDICES_QUAD[] = {
    0, 1, 2, 
    0, 3, 1  
};

#define ITFLIESBY_RENDERER_INDICES_QUAD_COUNT  6
#define ITFLIESBY_RENDERER_VERTICES_QUAD_COUNT 8

struct ItfliesbyRendererSolidQuadUpdate {
    f32                       transform[9];
    ItfliesbyRendererColorHex color;
};

#define ITFLIESBY_RENDERER_SOLID_QUAD_UPDATE_SIZE() sizeof(ItfliesbyRendererSolidQuadUpdate)

struct ItfliesbyRendererSolidQuadUpdateBatch {
    ItfliesbyRendererSolidQuadUpdate batch[ITFLIESBY_RENDERER_SOLID_QUADS_MAX];
    u8                               count;
};


struct ItfliesbyRendererQuadManager {
    GLuint                                gl_quad_vao;
    ItfliesbyRendererQuadBuffers          quad_buffers;
    ItfliesbyRendererSolidQuadUpdateBatch solid_quad_batch;
};

#endif //ITFLIESBY_RENDERER_QUAD_HPP