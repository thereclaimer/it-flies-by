#pragma once

#include "itfliesby-engine.hpp"
#include "itfliesby-engine-globals.hpp"
#include "itfliesby-engine-memory.cpp"

internal ItfliesbyRendererTextureId
itfliesby_engine_rendering_push_texture_sprite_character(
    ItfliesbyRendererHandle     renderer,
    ItfliesbyEngineAssetsImage  image,
    ItfliesbyEngineAssets*      assets) {

    //load the image
    ItfliesbyEngineAssetsImageData* image_data = 
        itfliesby_engine_assets_image_load(
            assets,
            image);

    //push the image data to the renderer
    ItfliesbyRendererTextureId texture_id = 
        itfliesby_renderer_texture_sprite_character_load(
            renderer,
            (ItfliesbyRendererTextureSpriteCharacterType)image,
            image_data->width_pixels,
            image_data->height_pixels,
            image_data->pixels);

    //unload the image
    itfliesby_engine_assets_image_unload(
        assets,
        &image_data
    ); 

    return(texture_id);
}

internal void
itfliesby_engine_rendering_shader_programs_init(
    ItfliesbyRendererHandle     renderer,
    ItfliesbyEngineAssets*      assets) {

    ITFLIESBY_ASSERT(renderer);
    
    const u32 shaders_count = ITFLIESBY_RENDERER_SHADER_TYPE_COUNT;

    const ItfliesbyEngineAssetsShader shader_stage_asset_id_vertex[shaders_count] = {
        ITFLIESBY_ENGINE_ASSETS_SHADER_TEXTURED_QUAD_VERTEX_SHADER,
        ITFLIESBY_ENGINE_ASSETS_SHADER_SOLID_QUAD_VERTEX_SHADER,
        ITFLIESBY_ENGINE_ASSETS_SHADER_TEST_VERTEX_SHADER
    };
    const ItfliesbyEngineAssetsShader shader_stage_asset_id_fragment[shaders_count] = {
        ITFLIESBY_ENGINE_ASSETS_SHADER_TEXTURED_QUAD_FRAGMENT_SHADER,
        ITFLIESBY_ENGINE_ASSETS_SHADER_SOLID_QUAD_FRAGMENT_SHADER,
        ITFLIESBY_ENGINE_ASSETS_SHADER_TEST_FRAGMENT_SHADER
    };

    //allocate space for the shaders
    const u64 memory_size_shader_stages_vertex   = itfliesby_engine_assets_shader_allocation_size(assets, (ItfliesbyEngineAssetsShader*)shader_stage_asset_id_vertex,   shaders_count);
    const u64 memory_size_shader_stages_fragment = itfliesby_engine_assets_shader_allocation_size(assets, (ItfliesbyEngineAssetsShader*)shader_stage_asset_id_fragment, shaders_count);

    ITFLIESBY_ASSERT(memory_size_shader_stages_vertex   > 0);
    ITFLIESBY_ASSERT(memory_size_shader_stages_fragment > 0);

    memory memory_shader_stage_vertex   = itfliesby_engine_memory_renderer_shader_allocate(memory_size_shader_stages_vertex);
    memory memory_shader_stage_fragment = itfliesby_engine_memory_renderer_shader_allocate(memory_size_shader_stages_fragment);

    //load the shaders
    u64 offsets_vertex[shaders_count];   
    u64 offsets_fragment[shaders_count]; 

    itfliesby_engine_assets_load_shaders(assets, (ItfliesbyEngineAssetsShader*)shader_stage_asset_id_vertex,   memory_shader_stage_vertex,   offsets_vertex,   shaders_count);
    itfliesby_engine_assets_load_shaders(assets, (ItfliesbyEngineAssetsShader*)shader_stage_asset_id_fragment, memory_shader_stage_fragment, offsets_fragment, shaders_count);

    //compile the shaders
    ItfliesbyRendererShaderStageBuffer renderer_shader_stage_buffer_vertex   = {0};
    ItfliesbyRendererShaderStageBuffer renderer_shader_stage_buffer_fragment = {0};
    
    u64 current_offset_vertex   = 0;
    u64 current_offset_fragment = 0;

    const ItfliesbyRendererShaderType shader_types[shaders_count] = {
        ITFLIESBY_RENDERER_SHADER_TYPE_TEXTURED_QUAD,
        ITFLIESBY_RENDERER_SHADER_TYPE_SOLID_QUAD,
        ITFLIESBY_RENDERER_SHADER_TYPE_TEST
    };

    for (
        u32 shader_index = 0;
        shader_index < shaders_count;
        ++shader_index) {

        ItfliesbyRendererShaderType shader_type = shader_types[shader_index];

        renderer_shader_stage_buffer_vertex   = {0};
        renderer_shader_stage_buffer_fragment = {0};

        current_offset_vertex   = offsets_vertex[shader_index]; 
        current_offset_fragment = offsets_fragment[shader_index]; 

        renderer_shader_stage_buffer_vertex.shader_stage_data   = &memory_shader_stage_vertex[current_offset_vertex]; 
        renderer_shader_stage_buffer_fragment.shader_stage_data = &memory_shader_stage_fragment[current_offset_fragment];

        ItfliesbyRendererShaderIndex new_shader = 
            itfliesby_renderer_shader_compile_and_link(
                renderer,
                shader_type,
                &renderer_shader_stage_buffer_vertex,
                &renderer_shader_stage_buffer_fragment);
    }

    itfliesby_engine_memory_renderer_shader_reset();

    //make sure we are ready to go
    b8 renderer_ready = itfliesby_renderer_ready(renderer);
    ITFLIESBY_ASSERT(renderer_ready);
}

internal ItfliesbyRendererHandle
itfliesby_engine_rendering_init(
    ItfliesbyEngineAssets* assets) {

    //init rendering memory
    itfliesby_engine_memory_renderer_create();

    //initialize renderer
    memory renderer_memory           = itfliesby_engine_memory_renderer_context();
    ItfliesbyRendererHandle renderer = itfliesby_renderer_create_and_init(platform_api,renderer_memory,ITFLIESBY_ENGINE_RENDERER_MEMORY_PARTITION_CONTEXT_SIZE);
    ITFLIESBY_ASSERT(renderer);

    //load shaders
    itfliesby_engine_rendering_shader_programs_init(
        renderer,
        assets
    );

    return(renderer);
}