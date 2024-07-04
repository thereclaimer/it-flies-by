#ifndef IFB_ENGINE_HPP
#define IFB_ENGINE_HPP

#include <ifb-types.hpp>

#ifndef IFB_ENGINE_INTERNAL

//--------------------------------
// ENGINE
//--------------------------------

typedef void* IFBEngineHandle;

api IFBEngineHandle
ifb_engine_create_and_initialize(
    IFBPlatformApi platform);

api void
ifb_engine_render_frame();

//--------------------------------
// MEMORY
//--------------------------------

typedef void* IFBEngineMemoryArenaHandle;

api IFBEngineMemoryArenaHandle
ifb_engine_memory_arena_reserve_64kb();

api IFBEngineMemoryArenaHandle
ifb_engine_memory_arena_reserve_64mb();

api void
ifb_engine_memory_arena_release(
    IFBEngineMemoryArenaHandle arena);

api memory
ifb_engine_memory_arena_bytes_push(
    IFBEngineMemoryArenaHandle arena_ptr,
    u64                        size_bytes);

api void
ifb_engine_memory_arena_bytes_pop(
    IFBEngineMemoryArenaHandle arena_ptr,
    u64                        size_bytes);

#endif //IFB_ENGINE_INTERNAL

#endif //IFB_ENGINE_HPP