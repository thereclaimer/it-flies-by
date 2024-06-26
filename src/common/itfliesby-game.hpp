#ifndef ITFLIESBY_GAME_HPP
#define ITFLIESBY_GAME_HPP

#include "itfliesby-types.hpp"

typedef handle itfliesby_game;

#define ITFLIESBY_GAME_MEMORY_SIZE ITFLIESBY_MATH_GIGABYTES(1)

api itfliesby_game
itfliesby_game_create(
    ItfliesbyPlatformApi platform,
    memory               game_memory,
    size                 game_memory_size
);

api void
itfliesby_game_destroy(
    itfliesby_game game
);

api void
itfliesby_game_update_and_render(
          itfliesby_game      game,
          ItfliesbyUserInput* user_input,
    const f32                 window_width,
    const f32                 window_height,
    const f32                 screen_width,
    const f32                 screen_height);

#endif //ITFLIESBY_GAME_HPP