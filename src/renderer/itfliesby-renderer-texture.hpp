#ifndef ITFLIESBY_RENDERER_TEXTURE_HPP
#define ITFLIESBY_RENDERER_TEXTURE_HPP

#include <ifb.hpp>
#include <common/itfliesby-platform-api.hpp>
#include <common/itfliesby-memory.hpp>
#include <math/itfliesby-math.hpp>

#include "itfliesby-renderer-opengl.hpp"
#include "itfliesby-renderer-types.hpp"

typedef GLuint ItfliesbyRendererTextureId;

enum ItfliesbyRendererTextureSpriteCharacterType : s32 {
    ITFLIESBY_RENDERER_TEXTURE_SPRITE_CHARACTER_TYPE_INVALID            = -1,
    ITFLIESBY_RENDERER_TEXTURE_SPRITE_CHARACTER_TYPE_CALIBRATION_CONNOR =  0,
    ITFLIESBY_RENDERER_TEXTURE_SPRITE_CHARACTER_TYPE_CALIBRATION_JIG    =  1,
    ITFLIESBY_RENDERER_TEXTURE_SPRITE_CHARACTER_TYPE_COUNT              =  2
};

struct ItfliesbyRendererGlTextureSpritesCharacters {

    union {

        struct {
            GLuint gl_texture_id_sprite_connor_calibration;
            GLuint gl_texture_id_sprite_jig_calibration;
        };

        GLuint gl_texture_id_array[ITFLIESBY_RENDERER_TEXTURE_SPRITE_CHARACTER_TYPE_COUNT];
    };

    GLuint gl_texture_unit;
};


struct ItfliesbyRendererTextureStore {
    ItfliesbyRendererGlTextureSpritesCharacters sprites_characters;
};

#endif //ITFLIESBY_RENDERER_TEXTURE_HPP