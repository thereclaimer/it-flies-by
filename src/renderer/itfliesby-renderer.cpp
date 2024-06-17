#pragma once

#include "itfliesby-renderer.hpp"
#include "itfliesby-renderer-shader.cpp"
#include "itfliesby-renderer-memory.cpp"
#include "itfliesby-renderer-quad.cpp"
#include "itfliesby-renderer-simple-quad.cpp"
#include "itfliesby-renderer-texture.cpp"

external ItfliesbyRendererPerspective
itfliesby_renderer_perspective(
    f32 width_pixels,
    f32 height_pixels) {

    IFBMathMat3 transform = {0};

    f32 half_width_pixels  = width_pixels  * 0.5f;
    f32 half_height_pixels = height_pixels * 0.5f;

    f32 right  = half_width_pixels;
    f32 top    = half_height_pixels;
    f32 left   = half_width_pixels  * -1.0f;
    f32 bottom = half_height_pixels * -1.0f;


    transform.rows.row_0[0] = 2.0f / (right - left);
    transform.rows.row_0[2] = -((right + left) / (right - left));

    transform.rows.row_1[1] = 2.0f / (top - bottom);
    transform.rows.row_1[2] = -((top + bottom) / (top - bottom));

    transform.rows.row_2[2] = 1.0f;

    ItfliesbyRendererPerspective perspective = {0};
    perspective.height_pixels = height_pixels;
    perspective.width_pixels  = width_pixels;
    perspective.transform     = transform;

    return(perspective);
}

external ItfliesbyRenderer*
itfliesby_renderer_create_and_init(
    IFBPlatformApi platform_api,
    memory               core_memory,
    u64                  core_memory_size_bytes) {

    platform = platform_api;

    //initialize the arena
    itfliesby_renderer_memory_create(
            core_memory,
            core_memory_size_bytes);
    
    //allocate the core renderer
    ItfliesbyRenderer* renderer = itfliesby_renderer_memory_allocate_core();
    ifb_assert(renderer);

    //initialize opengl
    renderer->gl_context = platform.graphics_api_init(platform.window);    
    ifb_assert(renderer->gl_context);
    glewExperimental = TRUE;
    ifb_assert(glewInit() == GLEW_OK);

    //set the clear color
    glClearColor(
        0.157f,
        0.157f,
        0.157f,
        1.0f
    );

	glHint(GL_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //initialize our quad buffers
    renderer->quad_manager        = itfliesby_renderer_quad_manager_init();
    renderer->buffers.simple_quad = itfliesby_renderer_shader_simple_quad_buffers_create();
    renderer->textures            = itfliesby_renderer_texture_store_init();
    renderer->perspective         = itfliesby_renderer_perspective(1920.0f, 1080.0f);

    return(renderer);
}

external void
itfliesby_renderer_render(
    ItfliesbyRenderer* renderer,
    f32                window_width,
    f32                window_height,
    f32                screen_width,
    f32                screen_height) {

    ItfliesbyRendererShader*                  solid_quad_shader   = &renderer->shader_store.types.solid_quad; 
    ItfliesbyRendererQuadManager*             quad_manager        = &renderer->quad_manager; 
    ItfliesbyRendererShaderUniformsSolidQuad* solid_quad_uniforms = &renderer->shader_store.uniforms.solid_quad_uniforms;
    ItfliesbyRendererSolidQuadUpdateBatch*    solid_quad_batch    = &renderer->quad_manager.solid_quad_batch;  
    ItfliesbyRendererBatchSimpleQuad*         simple_quad_batch   = &renderer->batches.simple_quad;

    renderer->screen_width  = screen_width;
    renderer->screen_height = screen_height;

    renderer->scale_factor.x = window_width  / screen_width; 
    renderer->scale_factor.y = window_height / screen_height; 

    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    itfliesby_renderer_opengl_set_viewport(
        window_width,
        window_height,
        screen_width,
        screen_height);

    itfliesby_renderer_quad_solid_quads_render(
        solid_quad_shader,
        quad_manager,
        solid_quad_uniforms,
        solid_quad_batch);

    itfliesby_renderer_simple_quad_render(
        &renderer->shader_store.uniforms.simple_quad,
        &renderer->buffers.simple_quad,
        simple_quad_batch,
        renderer->shader_store.types.simple_quad.gl_program_id);
}

external f32
itfliesby_renderer_aspect_ratio(
    ItfliesbyRenderer* renderer) {

    f32 aspect_ratio = renderer->screen_width / renderer->screen_height;

    return(aspect_ratio);
}

external ItfliesbyRendererScaleFactor
itfliesby_renderer_scale_factor(
    ItfliesbyRenderer* renderer) {

    return(renderer->scale_factor);
}