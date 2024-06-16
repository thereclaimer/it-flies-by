#pragma once

#include "itfliesby-game.hpp"
#include "itfliesby-game-scene.hpp"

internal void
itfliesby_game_scene_test(
    ItfliesbyGame* game) {

    ItfliesbyGameSceneTest* game_scene_test = &game->scenes.test;
    ItfliesbyEngineHandle   game_engine     = game->engine;

    //create the scene 
    if (game_scene_test->scene_id == ITFLIESBY_ENGINE_SCENE_INVALID) {
        game_scene_test->scene_id   = itfliesby_engine_scene_create(game_engine);
        ifb_assert(game_scene_test->scene_id != ITFLIESBY_ENGINE_SCENE_INVALID);
    }

    //make the scene active
    itfliesby_engine_scene_set_active(
        game_engine,
        game_scene_test->scene_id);

    //create connor
    if (game_scene_test->connor_sprite_id == ITFLIESBY_ENGINE_SPRITE_ID_INVALID) {
        game_scene_test->connor_sprite_id =
            itfliesby_engine_scene_connor_test(
                game_engine,
                game_scene_test->scene_id);
    }

    //create jig
    if (game_scene_test->jig_sprite_id == ITFLIESBY_ENGINE_SPRITE_ID_INVALID) {
        game_scene_test->jig_sprite_id =
            itfliesby_engine_scene_jig(
                game_engine,
                game_scene_test->scene_id);
    }


    ifb_nop();
}