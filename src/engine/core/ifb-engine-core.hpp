#ifndef IFB_ENGINE_CORE_HPP
#define IFB_ENGINE_CORE_HPP

#include <ifb.hpp>


#include "ifb-engine-memory.hpp"

//--------------------------------
// TYPES
//--------------------------------
typedef struct     IFBEngine; 
typedef IFBEngine* IFBEngine_Ptr; 
typedef IFBEngine& IFBEngine_Ref; 

//--------------------------------
// ENGINE
//--------------------------------

struct IFBEngineFrameAllocator {
    IFBEngineMemoryArena_Ptr arena;
};

struct IFBEngine {
    IFBEngineMemory_Ptr     memory;
    IFBEngineFrameAllocator frame_allocator;
};

external IFBEngine_Ptr
ifb_engine_create_and_initialize(
    IFBPlatformApi platform);

external void
ifb_engine_render_frame();

memory
ifb_engine_frame_allocator_reserve(
    u64 size);

//--------------------------------
// PLATFORM
//--------------------------------

global IFBPlatformApi ifb_platform_api;

#define ifb_engine_platform_file_size         ifb_platform_api.file_size
#define ifb_engine_platform_file_read         ifb_platform_api.file_read
#define ifb_engine_platform_file_write        ifb_platform_api.file_write
#define ifb_engine_platform_file_open         ifb_platform_api.file_open
#define ifb_engine_platform_file_close        ifb_platform_api.file_close
#define ifb_engine_platform_memory_allocate   ifb_platform_api.memory_allocate
#define ifb_engine_platform_memory_free       ifb_platform_api.memory_free
#define ifb_engine_platform_ticks             ifb_platform_api.ticks
#define ifb_engine_platform_delta_time_ms     ifb_platform_api.delta_time_ms
#define ifb_engine_platform_sleep             ifb_platform_api.sleep

#endif //IFB_ENGINE_CORE_HPP