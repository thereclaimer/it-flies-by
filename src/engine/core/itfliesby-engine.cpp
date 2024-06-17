#pragma once

#include "itfliesby-engine.hpp"
#include "itfliesby-engine-memory.cpp"
#include "itfliesby-engine-assets.cpp"
#include "itfliesby-engine-rendering.cpp"
#include "itfliesby-engine-physics.cpp"
#include "itfliesby-engine-sprites.cpp"
#include "itfliesby-engine-scene.cpp"
#include "itfliesby-engine-devtools.cpp"
#include "itfliesby-engine-memory-manager.cpp"

external ItfliesbyEngine*
itfliesby_engine_create(
    IFBPlatformApi platform,
    memory               core_memory,
    u64                  core_memory_size_bytes) {

    ifb_assert(core_memory_size_bytes == ITFLIESBY_ENGINE_MEMORY_SIZE);

    platform_api = platform;

    //initialize engine memory
    itfliesby_engine_memory_create(
        core_memory,
        core_memory_size_bytes);

    //allocate our core engine structure
    ItfliesbyEngine* engine = itfliesby_engine_memory_allocate_core();

    //frame information
    ItfliesbyEngineFrame frame_info = {0};
    frame_info.frame_profile_max         = 120;
    frame_info.target_fps                = 120;
    frame_info.target_ms_per_frame       = (1000 / 120);

    f64 target_ms_per_frame = (1 / engine->frame.target_fps) * 1000.0f;


    engine->frame = frame_info;
    //initialize assets
    itfliesby_engine_assets_init(&engine->assets);
    engine->renderer = itfliesby_engine_rendering_init(
        &engine->assets);

    engine->physics   = itfliesby_engine_physics_create_and_init();
    engine->sprites   = itfliesby_engine_sprites_create_and_init();
    engine->dev_tools = itfliesby_engine_devtools_create_and_init();

    return(engine);
}

external void
itfliesby_engine_destroy(
    ItfliesbyEngine* engine) {

}

internal void
itfliesby_engine_fetch_graphics_information(
    ItfliesbyEngine*                        engine,
    ItfliesbyEnginePhysicsTransformCollection* physics_payload) {

    ItfliesbyEngineSprites*     sprites     = &engine->sprites;
    ItfliesbyEnginePhysics*     physics     = &engine->physics;
    ItfliesbyRendererHandle     renderer    = engine->renderer;
    ItfliesbyRendererSimpleQuad simple_quad = {0};

    ItfliesbyEngineSpriteRenderingContext sprite_rendering = {0};
    itfliesby_engine_sprites_rendering_context(
        sprites,
        physics,
        renderer,
        &sprite_rendering);

    for (
        u32 index = 0;
        index < sprite_rendering.sprite_count;
        ++index) {


            IFBMathMat3               current_transform = sprite_rendering.transforms[index]; 
            ItfliesbyRendererColorHex       current_color     = sprite_rendering.colors[index];
            ItfliesbyRendererTextureId      current_texture   = sprite_rendering.renderer_textures[index];

            simple_quad.color     = itfliesby_renderer_color_normalize(current_color);
            simple_quad.transform = current_transform; 
            simple_quad.texture   = current_texture;

            itfliesby_renderer_simple_quad_push(
                renderer,
                simple_quad);
        }
}


external void
itfliesby_engine_render_scene(
          ItfliesbyEngine*    engine,
          ItfliesbyUserInput* user_input,
    const f32                 window_width,
    const f32                 window_height,
    const f32                 screen_width,
    const f32                 screen_height) {

    u64 ticks_before_frame = platform_api.ticks();

    engine->user_input = user_input;

    ItfliesbyEngineAssets*  assets   = &engine->assets;
    ItfliesbyRendererHandle renderer = engine->renderer;

    //process the active scene
    itfliesby_engine_scene_process_active(engine);

    //udpate the physics system
    ItfliesbyEnginePhysicsTransformCollection physics_payload = {0};
    itfliesby_engine_physics_update(
        &engine->physics,
        &physics_payload
    );

    //now we need to zip up the payload we are sending to the GPU
    itfliesby_engine_fetch_graphics_information(
        engine,
        &physics_payload
    );

    itfliesby_engine_assets_update(assets);

    //render the scene
    itfliesby_renderer_render(
        renderer,
        window_width,
        window_height,
        screen_width,
        screen_height);

    itfliesby_engine_devtools_update(engine);

    //frame end
    u64 ticks_after_frame = platform_api.ticks();
    f64 delta_time_ms = 
        platform_api.delta_time_ms(
            ticks_before_frame,
            ticks_after_frame);

    engine->frame.delta_time_ms = delta_time_ms;

    //sleep time
    if (delta_time_ms < engine->frame.target_ms_per_frame) {
        u64 sleep_time_ms = (u64)(engine->frame.target_ms_per_frame - delta_time_ms); 
        platform_api.sleep(sleep_time_ms);
    }

    ifb_nop();
}