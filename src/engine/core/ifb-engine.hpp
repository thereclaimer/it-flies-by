#ifndef IFB_ENGINE_CORE_HPP
#define IFB_ENGINE_CORE_HPP

#include <ifb.hpp>
#include "ifb-engine-platform.hpp"
#include "ifb-engine-memory.hpp"
#include "ifb-engine-assets.hpp"

//--------------------------------
// TYPES
//--------------------------------
typedef struct     IFBEngine; 
typedef IFBEngine* IFBEnginePtr; 
typedef IFBEngine& IFBEngineRef; 

//--------------------------------
// ENGINE
//--------------------------------

struct IFBEngineFrameAllocator {
    IFBEngineMemoryArenaPtr arena;
};

struct IFBEngine {
    IFBEngineMemoryPtr      memory;
    IFBEngineAssetsPtr      assets;
    IFBEngineFrameAllocator frame_allocator;
};

external IFBEnginePtr
ifb_engine_create_and_initialize(
    IFBPlatformApi platform);

external void
ifb_engine_render_frame();

memory
ifb_engine_frame_allocator_reserve(
    u64 size);

#endif //IFB_ENGINE_CORE_HPP