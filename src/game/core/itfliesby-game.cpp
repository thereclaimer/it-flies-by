#pragma once

#include "itfliesby-game.hpp"
#include "itfliesby-game-scene-test.cpp"

external ItfliesbyGame*
itfliesby_game_create(
    IFBPlatformApi  platform,
    memory                game_memory,
    size                  game_memory_size) {

    //initialize the game memory
    ifb_assert(game_memory_size == ITFLIESBY_GAME_MEMORY_SIZE);

    ItfliesbyGameMemoryArena game_memory_arena = {0};

    game_memory_arena.arena = itfliesby_memory_arena_create("GAME ARENA",game_memory_size,game_memory);

    ifb_assert(game_memory_arena.arena);

    //partitions
    game_memory_arena.partitions.game_core   = itfliesby_memory_partition_create(game_memory_arena.arena, "GAME SYSTEMS", IFB_MATH_KILOBYTES(1));
    game_memory_arena.partitions.game_engine = itfliesby_memory_partition_create(game_memory_arena.arena, "ENGINE PRTN",  IFB_MATH_MEGABYTES(512));
    
    ifb_assert(game_memory_arena.partitions.game_core);
    ifb_assert(game_memory_arena.partitions.game_engine);

    //allocators
    game_memory_arena.allocators.game_core_system_allocator = itfliesby_memory_allocator_linear_create(game_memory_arena.partitions.game_core,"GAME SYSTEMS ALCTR",512);
    
    ifb_assert(game_memory_arena.allocators.game_core_system_allocator);

    //engine
    memory engine_memory    = itfliesby_memory_partition_raw_memory(game_memory_arena.partitions.game_engine);
    u64 engine_memory_size  = itfliesby_memory_partition_space_total(game_memory_arena.partitions.game_engine);
    
    ItfliesbyEngineHandle engine = itfliesby_engine_create(platform,engine_memory,engine_memory_size); 
    ifb_assert(engine);

    //allocate core systems
    ItfliesbyGame* game = (ItfliesbyGame*)itfliesby_memory_allocator_linear_allocate(game_memory_arena.allocators.game_core_system_allocator,sizeof(ItfliesbyGame));
    ifb_assert(game);

    *game = {0};
    game->memory_arena = game_memory_arena;
    game->platform     = platform;
    game->engine       = engine;
    game->scenes       = itfliesby_game_scenes_init();

    return(game);
}

external void
itfliesby_game_destroy(
    ItfliesbyGame* game) {
}

external void
itfliesby_game_update_and_render(
          ItfliesbyGame*      game,
          ItfliesbyUserInput* user_input,
    const f32                 window_width,
    const f32                 window_height,
    const f32                 screen_width,
    const f32                 screen_height) {

    ItfliesbyEngineHandle engine = game->engine;

    itfliesby_game_scene_test(game);

    itfliesby_engine_render_scene(
        engine,
        user_input,
        window_width,
        window_height,
        screen_width,
        screen_height);

}