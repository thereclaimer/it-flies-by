#ifndef ITFLIESBY_ENGINE_SCENE_HPP
#define ITFLIESBY_ENGINE_SCENE_HPP

#include "itfliesby-engine.hpp"
#include "itfliesby-engine-globals.hpp"
#include "itfliesby-engine-sprites.hpp"

#define ITFLIESBY_ENGINE_SCENE_COUNT_MAX 8

typedef s8 ItfliesbyEngineSceneId; 

#define ITFLIESBY_ENGINE_SCENE_INVALID -1

struct ItfliesbyEngineScene {
    ItfliesbyEngineSpriteId sprite_id_connor;
    ItfliesbyEngineSpriteId sprite_id_jig;
};

inline ItfliesbyEngineScene
itfliesby_engine_scene_reset() {

    ItfliesbyEngineScene scene = {0};

    scene.sprite_id_connor = ITFLIESBY_ENGINE_SCENE_INVALID;

    return(scene);
}

struct ItfliesbyEngineSceneManager {
    b8                     used_scenes[ITFLIESBY_ENGINE_SCENE_COUNT_MAX];
    ItfliesbyEngineScene   scenes[ITFLIESBY_ENGINE_SCENE_COUNT_MAX];
    ItfliesbyEngineSceneId active_scene;
};

void
itfliesby_engine_scene_manager_create(
    ItfliesbyEngineSceneManager* scene_manager);

#endif //ITFLIESBY_ENGINE_SCENE_HPP