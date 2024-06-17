#pragma once

#include "itfliesby-renderer.hpp"

const f32 SIMPLE_QUAD_VERTEX_BUFFER[] = {
    -0.5f,  0.5f,  // Top-left
    -0.5f, -0.5f,  // Bottom-left
     0.5f, -0.5f,  // Bottom-right

    -0.5f,  0.5f,  // Top-left
     0.5f,  0.5f,  // Top-right
     0.5f, -0.5f   // Bottom-right
};
const f32 SIMPLE_QUAD_UV_BUFFER[] = {
     0.0f,  1.0f,  // Top-left
     0.0f,  0.0f,  // Bottom-left
     1.0f,  0.0f,  // Bottom-right

     0.0f,  1.0f,  // Top-left
     1.0f,  1.0f,  // Top-right
     1.0f,  0.0f   // Bottom-right
};

internal ItfliesbyRendererShaderBuffersSimpleQuad
itfliesby_renderer_shader_simple_quad_buffers_create() {

    ItfliesbyRendererShaderBuffersSimpleQuad simple_quad_buffers = {0};

    //create and bind the VAO
    itfliesby_renderer_opengl_vertex_array_create(
        &simple_quad_buffers.gl_vao);

    //create the buffers
    GLuint buffers[2];
    glGenBuffers(2,buffers);
    simple_quad_buffers.gl_vbo_quad_indices = buffers[0];
    simple_quad_buffers.gl_vbo_quad_uv      = buffers[1];

    u32 buffer_size_bytes = sizeof(f32) * 12;

    //vertex buffer
    itfliesby_renderer_opengl_array_buffer(
        simple_quad_buffers.gl_vbo_quad_indices,
        buffer_size_bytes,
        (f32*)SIMPLE_QUAD_VERTEX_BUFFER);

    //uv buffer
    itfliesby_renderer_opengl_array_buffer(
        simple_quad_buffers.gl_vbo_quad_uv,
        buffer_size_bytes,
        (f32*)SIMPLE_QUAD_UV_BUFFER);

    //unbind the vao
    glBindVertexArray(0);

    return(simple_quad_buffers);
}

external u32
itfliesby_renderer_simple_quad_push(
          ItfliesbyRenderer*          renderer,
    const ItfliesbyRendererSimpleQuad simple_quad) {

    ItfliesbyRendererBatchSimpleQuad* simple_quad_batch = &renderer->batches.simple_quad;

    if (simple_quad_batch->count == ITFLIESBY_RENDERER_TEST_BATCH_COUNT_MAX) {
        //TODO: we should have proper error codes
        return(-1);
    }

    u32 new_quad_index = simple_quad_batch->count;

    simple_quad_batch->color[new_quad_index]     = simple_quad.color;
    simple_quad_batch->transform[new_quad_index] = simple_quad.transform;
    simple_quad_batch->texture[new_quad_index]   = simple_quad.texture;

    ++simple_quad_batch->count;

    return(new_quad_index);
}

external void
itfliesby_renderer_simple_quad_push_batch(
    ItfliesbyRenderer*                 renderer,
    const size_t                       simple_quad_count,
    const ItfliesbyRendererSimpleQuad* simple_quad,
          u32*                         simple_quad_indices) {

    ItfliesbyRendererBatchSimpleQuad* simple_quad_batch = &renderer->batches.simple_quad;

    //TODO: we could push ones that actually fit
    if (simple_quad_batch->count + simple_quad_count >= ITFLIESBY_RENDERER_TEST_BATCH_COUNT_MAX) {
        for (
            size_t index = 0;
            index < simple_quad_count;
            ++index) {
                simple_quad_indices[index] = -1;
        }
    }

    u32 starting_quad_index = simple_quad_batch->count;
    ItfliesbyRendererSimpleQuad i_simple_quad = {0};
    size_t simple_quad_index = 0;

    for (
        size_t index = 0;
        index < simple_quad_count;
        ++index) {

        simple_quad_index = index + starting_quad_index;

        i_simple_quad = simple_quad[simple_quad_index];

        simple_quad_batch->color[simple_quad_index]     = i_simple_quad.color;
        simple_quad_batch->transform[simple_quad_index] = i_simple_quad.transform;
        simple_quad_batch->texture[simple_quad_index]   = i_simple_quad.texture;

        simple_quad_indices[index] = simple_quad_index;
    }
}

internal void
itfliesby_renderer_simple_quad_render(
    ItfliesbyRendererShaderUniformsSimpleQuad* uniforms,
    ItfliesbyRendererShaderBuffersSimpleQuad*  buffers,
    ItfliesbyRendererBatchSimpleQuad*          batch,
    GLuint                                     gl_program) {

    IFBMathMat3*                batch_transforms = batch->transform;
    ItfliesbyRendererColorNormalized* batch_colors     = batch->color; 
    ItfliesbyRendererTextureId*       batch_textures   = batch->texture; 
    u32                               batch_count      = batch->count;

    IFBMathMat3                current_transform = {0};
    ItfliesbyRendererColorNormalized current_color     = {0};
    ItfliesbyRendererTextureId       current_texture   = 0;

    //use program and vao
    itfliesby_renderer_opengl_use_program_and_bind_vertex_array(
        gl_program,
        buffers->gl_vao);

    //vertex attribute
    itfliesby_renderer_opengl_bind_and_enable_array_buffer_vec2(
        ITFLIESBY_RENDERER_SHADER_SIMPLE_QUAD_VAO_INDEX_VERTICES,
        buffers->gl_vbo_quad_indices);

    //uv attribute
    itfliesby_renderer_opengl_bind_and_enable_array_buffer_vec2(
        ITFLIESBY_RENDERER_SHADER_SIMPLE_QUAD_VAO_INDEX_UV,
        buffers->gl_vbo_quad_uv);

    //set the texture sampler
    itfliesby_renderer_opengl_set_texture_slot_and_sampler(
        GL_TEXTURE0,
        uniforms->index_texture_sampler);

    for (
        u32 index = 0;
        index < batch_count;
        ++index) {

        current_color     = batch_colors[index];
        current_transform = batch_transforms[index];
        current_texture   = batch_textures[index];

        //bind the texture
        itfliesby_renderer_opengl_bind_texture_2d(
            current_texture
        );

        //update the color
        itfliesby_renderer_opengl_uniform_data_vec4(
            uniforms->index_color,
            current_color.data);

        //update the transform
        itfliesby_renderer_opengl_uniform_data_mat3(
            uniforms->index_transform,
            current_transform.m);

        //draw the arrays
        itfliesby_renderer_opengl_draw_quad_vertices();
    }

    //unbind vao and program
    itfliesby_renderer_opengl_unbind_program_and_vertex_array();
    
    //reset
    batch->count = 0;
}