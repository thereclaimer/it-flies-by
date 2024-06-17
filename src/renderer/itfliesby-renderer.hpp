#ifndef ITFLIESBY_RENDERER_HPP
#define ITFLIESBY_RENDERER_HPP


#include <ifb.hpp>
#include <common/itfliesby-platform-api.hpp>
#include <common/itfliesby-memory.hpp>

#include "itfliesby-renderer-opengl.hpp"
#include "itfliesby-renderer-shader.hpp"
#include "itfliesby-renderer-quad.hpp"
#include "itfliesby-renderer-memory.hpp"
#include "itfliesby-renderer-texture.hpp"

global IFBPlatformApi platform;

struct ItfliesbyRendererBatches{
    ItfliesbyRendererBatchSimpleQuad simple_quad;
};

struct ItfliesbyRendererBuffers {
    ItfliesbyRendererShaderBuffersSimpleQuad simple_quad;
};

struct ItfliesbyRendererPerspective {
    ItfliesbyMathMat3 transform;
    f32               width_pixels;
    f32               height_pixels;
};

typedef ItfliesbyMathVec2 ItfliesbyRendererScaleFactor;

struct ItfliesbyRenderer {
    handle                        gl_context;
    ItfliesbyRendererShaderStore  shader_store;
    ItfliesbyRendererQuadManager  quad_manager;
    ItfliesbyRendererBatches      batches;
    ItfliesbyRendererBuffers      buffers;
    ItfliesbyRendererTextureStore textures;
    ItfliesbyRendererPerspective  perspective;
    f32 screen_width;
    f32 screen_height;
    ItfliesbyRendererScaleFactor scale_factor;
};

inline void
itfliesby_renderer_apply_perspective_to_transforms(
    const ItfliesbyMathMat3* perspective,
    const ItfliesbyMathMat3* in_transforms,
    const size_t             transforms_count,
          ItfliesbyMathMat3* out_transforms) {

    itfliesby_math_mat3_multiply(
        perspective,
        in_transforms,
        transforms_count,
        out_transforms);
}

#endif //ITFLIESBY_RENDERER_HPP