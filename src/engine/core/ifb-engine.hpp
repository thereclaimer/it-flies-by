#ifndef IFB_ENGINE_CORE_HPP
#define IFB_ENGINE_CORE_HPP

#define IFB_ENGINE_INTERNAL

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

struct  IFBEngineCoreMemory; 
typedef IFBEngineCoreMemory* IFBEngineCoreMemoryPtr; 
typedef IFBEngineCoreMemory& IFBEngineCoreMemoryRef; 

//--------------------------------
// MEMORY
//--------------------------------

#define IFB_ENGINE_CORE_MEMORY_REGION_SIZE IFB_MATH_MEGABYTES(64)

struct IFBEngineCoreMemoryArenaAllocator {
    IFBEngineMemoryArenaAllocatorPtr arena_16kb;    
};

IFBEngineMemoryArenaPtr
ifb_engine_core_memory_arena_reserve_16kb();

//this is a scratch buffer that is reset each frame
//if you need some bytes for a quick allocation, this
//is the allocator to use
struct IFBEngineCoreMemoryFrameAllocator {
    IFBEngineMemoryArenaPtr          arena;
    memory                           stack_ptr;
};

struct IFBEngineCoreMemory {

    IFBEngineMemoryRegionPtr region;

    struct {
        IFBEngineCoreMemoryArenaAllocator arena;
        IFBEngineCoreMemoryFrameAllocator frame;
    } allocators;
};

IFBEngineCoreMemoryPtr
ifb_engine_core_memory_create_and_initialize();

void
ifb_engine_core_memory_frame_reset();

memory
ifb_engine_core_memory_frame_bytes_push(
    u64 size);

memory
ifb_engine_memory_frame_bytes_pop(
    u64 size);


//--------------------------------
// ENGINE
//--------------------------------

struct IFBEngineSystems {
    IFBEngineMemoryPtr memory;
    IFBEngineAssetsPtr assets;
};

struct IFBEngine {
    IFBEngineSystems       systems;
    IFBEngineCoreMemoryPtr memory;
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
