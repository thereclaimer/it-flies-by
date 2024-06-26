#pragma once

#include "itfliesby-renderer.hpp"

internal void
itfliesby_renderer_quad_index_buffer_create(
    GLuint vbo,
    const u32 quad_vertices[6]) {
    
    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        vbo);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(u32) * 6, 
        quad_vertices, 
        GL_STATIC_DRAW);

}

internal void
itfliesby_renderer_quad_vertex_buffer_create(
    GLuint vbo,
    const f32 quad_vertices[8]) {
    
    glBindBuffer(
        GL_ARRAY_BUFFER,
        vbo);

    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(f32) * 8, 
        quad_vertices, 
        GL_STATIC_DRAW);

}

internal ItfliesbyRendererQuadBuffers
itfliesby_renderer_quad_buffers_init() {

    ItfliesbyRendererQuadBuffers quad_buffers = {0};

    glGenBuffers(
        ITFLIESBY_RENDERER_QUAD_BUFFERS_COUNT,
        quad_buffers.array
    );

    itfliesby_renderer_quad_index_buffer_create(
        quad_buffers.instances.indices,
        ITFLIESBY_RENDERER_INDICES_QUAD
    );

    itfliesby_renderer_quad_vertex_buffer_create(
        quad_buffers.instances.square_quad_vertices,
        ITFLIESBY_RENDERER_VERTICES_SQUARE_QUAD
    );

    return(quad_buffers);
};

internal ItfliesbyRendererQuadManager
itfliesby_renderer_quad_manager_init() {

    ItfliesbyRendererQuadManager quad_manager = {0};

    //create and bind the VAO
    glGenVertexArrays(1,&quad_manager.gl_quad_vao);
    glBindVertexArray(quad_manager.gl_quad_vao);

    //generate the indices and elements used for the quads
    quad_manager.quad_buffers = itfliesby_renderer_quad_buffers_init(); 

    //specify the layout of the vertices
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(f32),
        (void*)0);
    glEnableVertexAttribArray(0);

    //unbind the vao
    glBindVertexArray(0);

    return(quad_manager);
}

external void
itfliesby_renderer_quad_solid_quads_batch_update(
    ItfliesbyRenderer*                     renderer,
    ItfliesbyRendererSolidQuadUpdateBatch* solid_quad_update_batch) {

    renderer->quad_manager.solid_quad_batch = {0}; 
    renderer->quad_manager.solid_quad_batch.count = solid_quad_update_batch->count;
    
    for (
        u32 index = 0;
        index < solid_quad_update_batch->count;
        ++index) {

        renderer->quad_manager.solid_quad_batch.batch[index] = solid_quad_update_batch->batch[index];
    }
}

internal void
itfliesby_renderer_quad_solid_quads_render(
    ItfliesbyRendererShader*                  solid_quad_shader,
    ItfliesbyRendererQuadManager*             quad_manager,
    ItfliesbyRendererShaderUniformsSolidQuad* solid_quad_uniforms,
    ItfliesbyRendererSolidQuadUpdateBatch*    solid_quad_batch) {


    //set the uniform buffer data
    u32 data_size = sizeof(ItfliesbyRendererSolidQuadUpdate) * solid_quad_batch->count; 
    u32 uniform_index_offset = 0;
    u32 uniform_offset_model = 0;
    u32 uniform_offset_color = 0;
    ItfliesbyRendererSolidQuadUpdate current_quad = {0};

    for (
        u32 uniform_index = 0;
        uniform_index < solid_quad_batch->count;
        ++uniform_index
    ) {
        current_quad = solid_quad_batch->batch[uniform_index];

        uniform_index_offset = uniform_index * solid_quad_uniforms->block_data_size;
        uniform_offset_model = uniform_index_offset + solid_quad_uniforms->offset_model;
        uniform_offset_color = uniform_index_offset + solid_quad_uniforms->offset_color;

        //copy matrix
        for (
            u32 matrix_index = 0;
            matrix_index < 9;
            ++matrix_index) {

            solid_quad_uniforms->gl_uniform_buffer_memory[uniform_offset_model + matrix_index] =
                current_quad.transform[matrix_index]; 
        }

        //copy color
        solid_quad_uniforms->gl_uniform_buffer_memory[uniform_offset_color]     = current_quad.color.r;
        solid_quad_uniforms->gl_uniform_buffer_memory[uniform_offset_color + 1] = current_quad.color.g;
        solid_quad_uniforms->gl_uniform_buffer_memory[uniform_offset_color + 2] = current_quad.color.b;
        solid_quad_uniforms->gl_uniform_buffer_memory[uniform_offset_color + 3] = current_quad.color.a;
    }

    //bind the program
    glUseProgram(solid_quad_shader->gl_program_id);
    glBindVertexArray(quad_manager->gl_quad_vao);

    //bind the uniform buffer
    glBindBuffer(
        GL_UNIFORM_BUFFER,
        solid_quad_uniforms->gl_solid_quad_update_ubo);

    glBufferData(
        GL_UNIFORM_BUFFER,
        solid_quad_uniforms->block_data_size,
        solid_quad_uniforms->gl_uniform_buffer_memory,
        GL_STATIC_DRAW);

    glBindBufferBase(
        GL_UNIFORM_BUFFER,
        ITFLIESBY_RENDERER_SHADER_UNIFORM_SOLID_QUAD_BINDING_POINT,
        solid_quad_uniforms->gl_solid_quad_update_ubo
    );

    //bind the element buffer
    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        quad_manager->quad_buffers.instances.indices);

    glDrawElements(
        GL_TRIANGLES,
        ITFLIESBY_RENDERER_INDICES_QUAD_COUNT,
        GL_UNSIGNED_INT,
        NULL);

    glBindVertexArray(0);
}