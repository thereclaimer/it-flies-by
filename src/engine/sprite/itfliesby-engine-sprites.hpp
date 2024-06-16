#ifndef ITFLIESBY_ENGINE_SPRITES_HPP
#define ITFLIESBY_ENGINE_SPRITES_HPP

#include "itfliesby-engine.hpp"
#include "itfliesby-engine-globals.hpp"

typedef s8 ItfliesbyEngineSpriteId;

#define ITFLIESBY_ENGINE_SPRITE_ID_INVALID -1

struct ItfliesbyEngineSprite {
    ItfliesbyEngineSpriteId    sprite_id;
    ItfliesbyEnginePhysicsId   physics_id;
    ItfliesbyRendererColorHex  color;
    ItfliesbyEngineAssetsImage texture;
    ItfliesbyRendererTextureId renderer_texture;
};

#define ITFLIESBY_ENGINE_SPRITE_COUNT_MAX       128

struct ItfliesbyEngineSpriteRenderingContext {
    ItfliesbyEngineSpriteId    sprite_ids[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    ItfliesbyRendererTextureId renderer_textures[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    ItfliesbyRendererColorHex  colors[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    ItfliesbyMathMat3          transforms[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    size_t                     sprite_count;
};

struct ItfliesbyEngineSprites {
    b8                         sprite_used[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    ItfliesbyRendererColorHex  sprite_colors[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    ItfliesbyEngineAssetsImage sprite_textures[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    ItfliesbyEnginePhysicsId   sprite_physics[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
    ItfliesbyRendererTextureId renderer_textures[ITFLIESBY_ENGINE_SPRITE_COUNT_MAX];
};

#define ITFLIESBY_ENGINE_SPRITE_COLOR_CONOR {111,149,98 ,0  }
#define ITFLIESBY_ENGINE_SPRITE_COLOR_NONE  {0  ,0  ,0  ,0  }

ItfliesbyEngineSpriteId
itfliesby_engine_sprites_connor_test(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEnginePhysics*        physics,
    ItfliesbyEnginePhysicsPosition position,
    ItfliesbyEngineAssets*         assets,
    ItfliesbyRendererHandle        renderer);

ItfliesbyEngineSpriteId
itfliesby_engine_sprites_jig(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEngineSpriteId        connor_sprite_id,
    ItfliesbyEnginePhysics*        physics,
    ItfliesbyEngineAssets*         assets,
    ItfliesbyRendererHandle        renderer);

ItfliesbyEnginePhysicsId
itfliesby_engine_sprites_physics_id_get(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEngineSpriteId        sprite_id);

ItfliesbyEngineSprite
itfliesby_engine_sprites_get(
    ItfliesbyEngineSprites*        sprites,
    ItfliesbyEngineSpriteId        sprite_id
);

void
itfliesby_engine_sprites_rendering_context(
    const ItfliesbyEngineSprites*                   sprites,
    const ItfliesbyEnginePhysics*                   physics,
    const ItfliesbyRendererHandle                   renderer,
          ItfliesbyEngineSpriteRenderingContext*    sprite_rendering_context);

#endif //ITFLIESBY_ENGINE_SPRITES_HPP