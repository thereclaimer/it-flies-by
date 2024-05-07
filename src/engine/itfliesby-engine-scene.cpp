#pragma once

#include "itfliesby-engine-scene.hpp"

void
itfliesby_engine_scene_manager_create(
    ItfliesbyEngineSceneManager* scene_manager) {

    *scene_manager = {0};
    scene_manager->active_scene = ITFLIESBY_ENGINE_SCENE_INVALID;
}

external ItfliesbyEngineSceneId
itfliesby_engine_scene_create(
    ItfliesbyEngine* engine) {
    
    ItfliesbyEngineSceneManager* scene_manager             = &engine->scenes;    
    ItfliesbyEngineScene*        scene_manager_scenes      = scene_manager->scenes;
    b8*                          scene_manager_used_scenes = scene_manager->used_scenes;

    ItfliesbyEngineSceneId new_scene_id = ITFLIESBY_ENGINE_SCENE_INVALID;

    //find the next available scene id
    for (
        ItfliesbyEngineSceneId scene_id = 0;
        scene_id < ITFLIESBY_ENGINE_SCENE_COUNT_MAX;
        ++scene_id) {

        if (!scene_manager_used_scenes[scene_id]) {
            new_scene_id = scene_id;
            break;
        }
    }

    //activate the scene
    ItfliesbyEngineScene new_scene = {0};
    new_scene.sprite_id_connor = ITFLIESBY_ENGINE_SPRITE_ID_INVALID;

    scene_manager_used_scenes[new_scene_id] = true;
    scene_manager_scenes[new_scene_id]      = itfliesby_engine_scene_reset();

    return(new_scene_id);
}

external void
itfliesby_engine_scene_destroy(
    ItfliesbyEngine*       engine,
    ItfliesbyEngineSceneId scene_id) {

    engine->scenes.used_scenes[scene_id] = false;

    if (engine->scenes.active_scene == scene_id) {
        engine->scenes.active_scene = ITFLIESBY_ENGINE_SCENE_INVALID;
    }
}

external void
itfliesby_engine_scene_set_active(
    ItfliesbyEngine*       engine,
    ItfliesbyEngineSceneId scene_id) {

    engine->scenes.active_scene = scene_id;
}

external ItfliesbyEngineSpriteId
itfliesby_engine_scene_connor_test(
    ItfliesbyEngine*       engine,
    ItfliesbyEngineSceneId scene_id) {

    ItfliesbyEngineSpriteId connor_sprite_id = 
        itfliesby_engine_sprites_connor_test(
            &engine->sprites,
            &engine->physics,
            ITFLIESBY_ENGINE_PHYSICS_POSITION_CENTER);
        
    return(connor_sprite_id);
}

internal void
itfliesby_engine_scene_process_active(
    ItfliesbyEngine*       engine) {

    ItfliesbyEngineSceneManager* engine_scene_manager        = &engine->scenes;
    ItfliesbyEngineScene*        engine_scene_manager_scenes = engine_scene_manager->scenes;

    //get the active scene
    ItfliesbyEngineSceneId active_scene_id = engine_scene_manager->active_scene;
    ItfliesbyEngineScene   active_scene    = engine_scene_manager_scenes[active_scene_id];

    //get the player sprite id
    ItfliesbyEngineSprites*  engine_sprites   = &engine->sprites;
    ItfliesbyEngineSpriteId connor_sprite_id = active_scene.sprite_id_connor;

    //get the physics information
    ItfliesbyEnginePhysics*        engine_physics    =  &engine->physics;
    ItfliesbyEnginePhysicsId       connor_physics_id = itfliesby_engine_sprites_physics_id_get(engine_sprites,connor_sprite_id);
    ItfliesbyEnginePhysicsVelocity connor_velocity   = {0};

    //update the velocity based on the user input
    ItfliesbyUserInput engine_user_input = *engine->user_input;
    if (ITFLIESBY_KEYCODE_STATE_W(engine_user_input)) {
        connor_velocity.y = 0.1f; 
    }
    if (ITFLIESBY_KEYCODE_STATE_S(engine_user_input)) {
        connor_velocity.y = 0.1f; 
    }
    if (ITFLIESBY_KEYCODE_STATE_A(engine_user_input)) {
        connor_velocity.x = 0.1f; 
    }
    if (ITFLIESBY_KEYCODE_STATE_D(engine_user_input)) {
        connor_velocity.x = 0.1f; 
    }

    itfliesby_engine_physics_update_velocity(
        engine_physics,
        connor_physics_id,
        connor_velocity);
}