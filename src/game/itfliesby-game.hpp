#ifndef ITFLIESBY_GAME_HPP
#define ITFLIESBY_GAME_HPP

#include <itfliesby.hpp>
#include "itfliesby-game-scene.hpp"

#define ITFLIESBY_GAME_MEMORY_SIZE IFB_MATH_GIGABYTES(1)

struct ItfliesbyGameMemoryAllocators {
    itfliesby_memory_allocator_linear game_core_system_allocator;
};

struct ItfliesbyGameMemoryPartitions {
    itfliesby_memory_partition game_core;
    itfliesby_memory_partition game_engine;
};

struct ItfliesbyGameMemoryArena {
    itfliesby_memory_arena        arena;
    ItfliesbyGameMemoryPartitions partitions;
    ItfliesbyGameMemoryAllocators allocators;
};

struct ItfliesbyGame {
    u64                      previous_time_ticks;
    u64                      delta_time_ticks;
    IFBPlatformApi     platform;
    ItfliesbyEngineHandle    engine;
    ItfliesbyGameMemoryArena memory_arena;
    ItfliesbyGameScenes      scenes;
};

api ItfliesbyGame*
itfliesby_game_create(
    IFBPlatformApi  platform,
    memory                game_memory,
    size                  game_memory_size
);

api void
itfliesby_game_destroy(
    ItfliesbyGame* game
);

api void
itfliesby_game_update_and_render(
          ItfliesbyGame*      game,
          ItfliesbyUserInput* user_input,
    const f32                 window_width,
    const f32                 window_height,
    const f32                 screen_width,
    const f32                 screen_height);

#endif //ITFLIESBY_GAME_HPP

