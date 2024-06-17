#ifndef ITFLIESBY_GAME_SCENE_HPP
#define ITFLIESBY_GAME_SCENE_HPP

#include <itfliesby.hpp>

struct ItfliesbyGameSceneTest {
    ItfliesbyEngineSceneId  scene_id;
    ItfliesbyEngineSpriteId connor_sprite_id;    
    ItfliesbyEngineSpriteId jig_sprite_id;    
};  

struct ItfliesbyGameScenes {
    ItfliesbyGameSceneTest test;
};

inline ItfliesbyGameScenes
itfliesby_game_scenes_init() {

    ItfliesbyGameScenes scenes = {0};

    //test scene
    ItfliesbyGameSceneTest test_scene = {0};
    test_scene.scene_id         = ITFLIESBY_ENGINE_SCENE_INVALID;
    test_scene.connor_sprite_id = ITFLIESBY_ENGINE_SPRITE_ID_INVALID;
    test_scene.jig_sprite_id    = ITFLIESBY_ENGINE_SPRITE_ID_INVALID;
    scenes.test = test_scene;

    return(scenes);
}

#endif //ITFLIESBY_GAME_SCENE_HPP