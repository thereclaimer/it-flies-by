#pragma once

#include "itfliesby-engine-sprites.hpp"
#include "itfliesby-engine-rendering.cpp"


internal ItfliesbyEngineSpriteId
itfliesby_engine_sprites_activate_next(
    ItfliesbyEngineSprites* sprites) {

    ItfliesbyEngineSpriteId new_sprite = ITFLIESBY_ENGINE_SPRITE_ID_INVALID;

    //find the next sprite
    for (
        u32 index = 0;
        index < ITFLIESBY_ENGINE_SPRITE_COUNT_MAX;
        ++index) {

        if (!sprites->sprite_used[index]) {
            new_sprite = index;
            break;
        }
    }

    return(new_sprite);
}

internal ItfliesbyEngineSpriteId
itfliesby_engine_sprites_create(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEnginePhysics*        physics,
    ItfliesbyEnginePhysicsPosition position,
    ItfliesbyRendererHandle        renderer,
    ItfliesbyRendererColorHex      color) {

    ItfliesbyEngineSpriteId new_sprite = itfliesby_engine_sprites_activate_next(sprites);

    //create the physics transforms
    ItfliesbyEnginePhysicsId physics_id = 
        itfliesby_engine_physics_transforms_create(
            physics,     // physics
            {0.0f,0.0f}, // position
            {1.0f,1.0f}, // scale
            0.0f);       // rotation degrees

    if (physics_id == ITFLIESBY_ENGINE_PHYSICS_OBJECT_INVALID) {
        return(ITFLIESBY_ENGINE_SPRITE_ID_INVALID);
    }

    //activate the new sprite        
    sprites->sprite_used[new_sprite]    = true;
    sprites->sprite_colors[new_sprite]  = color;
    sprites->sprite_physics[new_sprite] = physics_id;
    

    //sanity check
    ITFLIESBY_ASSERT(
        new_sprite       >= 0 &&
        physics_id       >= 0);

    return(new_sprite);
}

internal ItfliesbyEngineSprites
itfliesby_engine_sprites_create_and_init() {

    ItfliesbyEngineSprites sprites = {0};

    return(sprites);
}

internal ItfliesbyEngineSpriteId
itfliesby_engine_sprites_connor_test(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEnginePhysics*        physics,
    ItfliesbyEnginePhysicsPosition position,
    ItfliesbyEngineAssets*         assets,
    ItfliesbyRendererHandle        renderer) {

    ItfliesbyEngineSpriteId connor_sprite_id = itfliesby_engine_sprites_activate_next(sprites);

    ItfliesbyEnginePhysicsScale scale = itfliesby_engine_physics_scale(0.5f, 0.5f);

    //create the physics transforms
    ItfliesbyEnginePhysicsId physics_id = 
        itfliesby_engine_physics_transforms_create(
            physics,  // physics
            position, // position
            scale,    // scale
            0.0f);    // rotation degrees

    if (physics_id == ITFLIESBY_ENGINE_PHYSICS_OBJECT_INVALID) {
        return(ITFLIESBY_ENGINE_SPRITE_ID_INVALID);
    }

    //send the sprite to the renderer
    ItfliesbyRendererTextureId texture_id = 
        itfliesby_engine_rendering_push_texture_sprite_character(
            renderer,
            ITFLIESBY_ENGINE_ASSETS_IMAGE_CALIBRATION_CONNOR,
            assets);

    //activate the new sprite        
    sprites->sprite_used[connor_sprite_id]       = true;
    sprites->sprite_colors[connor_sprite_id]     = ITFLIESBY_ENGINE_SPRITE_COLOR_CONOR;
    sprites->sprite_physics[connor_sprite_id]    = physics_id;
    sprites->sprite_textures[connor_sprite_id]   = ITFLIESBY_ENGINE_ASSETS_IMAGE_CALIBRATION_CONNOR;
    sprites->renderer_textures[connor_sprite_id] = texture_id; 

    //sanity check
    ITFLIESBY_ASSERT(
        connor_sprite_id >= 0 &&
        physics_id       >= 0);

    return(connor_sprite_id);
}

ItfliesbyEngineSpriteId
itfliesby_engine_sprites_jig(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEngineSpriteId        connor_sprite_id,
    ItfliesbyEnginePhysics*        physics,
    ItfliesbyEngineAssets*         assets,
    ItfliesbyRendererHandle        renderer) {

    //get the next available sprite id
    ItfliesbyEngineSpriteId jig_sprite_id = itfliesby_engine_sprites_activate_next(sprites);

    ItfliesbyEnginePhysicsScale jig_scale = itfliesby_engine_physics_scale(0.25f,0.25f);    

    //jig's position is based off connor's position
    ItfliesbyEnginePhysicsPosition connor_position =
        itfliesby_engine_physics_position(
            physics,
            connor_sprite_id);

    //TODO: for now, it'll just be at the top left
    ItfliesbyEnginePhysicsPosition jig_position = {0};
    jig_position.x = -0.3f;
    jig_position.y =  0.3f;

    //create the physics transforms
    ItfliesbyEnginePhysicsId jig_physics_id = 
        itfliesby_engine_physics_transforms_create(
            physics,      // physics
            jig_position, // position
            jig_scale,    // scale
            0.0f);        // rotation degrees

    if (jig_physics_id == ITFLIESBY_ENGINE_PHYSICS_OBJECT_INVALID) {
        return(ITFLIESBY_ENGINE_SPRITE_ID_INVALID);
    }

    //create jig's texture
    ItfliesbyRendererTextureId jig_texture_id = 
        itfliesby_engine_rendering_push_texture_sprite_character(
            renderer,
            ITFLIESBY_ENGINE_ASSETS_IMAGE_CALIBRATION_JIG,
            assets);

    //add jig's shit to the store
    sprites->sprite_used[jig_sprite_id]       = true;
    //TODO: not proper, also not a big deal
    sprites->sprite_colors[jig_sprite_id]     = ITFLIESBY_ENGINE_SPRITE_COLOR_CONOR;
    sprites->sprite_physics[jig_sprite_id]    = jig_physics_id;
    sprites->sprite_textures[jig_sprite_id]   = ITFLIESBY_ENGINE_ASSETS_IMAGE_CALIBRATION_JIG;
    sprites->renderer_textures[jig_sprite_id] = jig_texture_id; 

    return(jig_sprite_id);
}

internal ItfliesbyEnginePhysicsId
itfliesby_engine_sprites_physics_id_get(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEngineSpriteId        sprite_id) {

    ItfliesbyEnginePhysicsId physics_id = sprites->sprite_physics[sprite_id];

    return(physics_id);
}

ItfliesbyEngineSprite
itfliesby_engine_sprites_get(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEngineSpriteId        sprite_id) {

    ItfliesbyEngineSprite sprite = {0};

    sprite.sprite_id        = sprite_id;
    sprite.color            = sprites->sprite_colors[sprite_id];
    sprite.physics_id       = sprites->sprite_physics[sprite_id];
    sprite.texture          = sprites->sprite_textures[sprite_id];
    sprite.renderer_texture = sprites->renderer_textures[sprite_id]; 

    return(sprite);
}

internal u32
itfliesby_engine_sprites_active(
    const ItfliesbyEngineSprites*  sprites,
          ItfliesbyEngineSpriteId* active_sprite_ids) {

    const b8* sprites_active       = sprites->sprite_used;
    u32       sprites_active_count = 0;

    for (
        u32 sprite_index = 0;
        sprite_index < ITFLIESBY_ENGINE_SPRITE_COUNT_MAX;
        ++sprite_index) {

        if (sprites_active[sprite_index]) {
            active_sprite_ids[sprites_active_count] = sprite_index;
            ++sprites_active_count;
        }
    }
    
    return(sprites_active_count);
}

internal void
itfliesby_engine_sprites_rendering_context(
    const ItfliesbyEngineSprites*                   sprites,
    const ItfliesbyEnginePhysics*                   physics,
    const ItfliesbyRendererHandle                   renderer,
          ItfliesbyEngineSpriteRenderingContext*    sprite_rendering_context) {

    const ItfliesbyRendererTextureId* sprite_textures = sprites->renderer_textures;
    const ItfliesbyRendererColorHex*  sprites_colors            = sprites->sprite_colors;
    const ItfliesbyEnginePhysicsId*   sprites_physics           = sprites->sprite_physics;

    ItfliesbyEngineSpriteId*    sprite_rendering_ids        = sprite_rendering_context->sprite_ids;
    ItfliesbyRendererColorHex*  sprite_rendering_colors     = sprite_rendering_context->colors;
    ItfliesbyRendererTextureId* sprite_rendering_textures   = sprite_rendering_context->renderer_textures;
    ItfliesbyMathMat3*          sprite_rendering_transforms = sprite_rendering_context->transforms;
    ItfliesbyEnginePhysicsId    sprite_rendering_physics_ids[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];

    //get the active sprite ids    
    u32 active_sprite_ids_count = itfliesby_engine_sprites_active(sprites,sprite_rendering_ids);
    ItfliesbyEngineSpriteId current_sprite = 0;

    //get the physics ids, colors, and textures    
    for (
        u32 sprite_rendering_index = 0;
        sprite_rendering_index < active_sprite_ids_count;
        ++sprite_rendering_index) {

        current_sprite = sprite_rendering_ids[sprite_rendering_index]; 

        sprite_rendering_colors[sprite_rendering_index]      = sprites_colors[current_sprite];
        sprite_rendering_textures[sprite_rendering_index]    = sprite_textures[current_sprite];
        sprite_rendering_physics_ids[sprite_rendering_index] = sprites_physics[current_sprite];
    }

    ItfliesbyRendererScaleFactor scale_factor = itfliesby_renderer_scale_factor(renderer);

    //now we need our physics transforms
    ItfliesbyMathMat3 physics_transforms[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    itfliesby_engine_physics_transforms(
        physics,
        sprite_rendering_physics_ids,
        active_sprite_ids_count,
        scale_factor,
        physics_transforms);

    //apply projection    

    for (
        u32 transform_index = 0;
        transform_index < active_sprite_ids_count;
        ++transform_index) {

        sprite_rendering_transforms[transform_index] = physics_transforms[transform_index];

    }

    sprite_rendering_context->sprite_count = active_sprite_ids_count;
}