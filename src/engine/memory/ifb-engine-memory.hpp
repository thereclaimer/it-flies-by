#ifndef IFB_ENGINE_MEMORY_HPP
#define IFB_ENGINE_MEMORY_HPP

#include <string.h>
#include <ifb-math.hpp>

#include "ifb-engine-platform.hpp"

#define IFB_ENGINE_MEMORY_REQUIREMENT IFB_MATH_GIGABYTES(4)

//--------------------------------
// TYPES
//--------------------------------

typedef struct                  IFBEngineMemory;
typedef IFBEngineMemory*        IFBEngineMemoryPtr;
typedef IFBEngineMemory&        IFBEngineMemoryRef;
typedef void*                   IFBEngineMemoryHandle;

typedef struct                  IFBEngineMemoryManager;
typedef IFBEngineMemoryManager* IFBEngineMemoryManagerPtr;
typedef IFBEngineMemoryManager& IFBEngineMemoryManagerRef;

typedef struct                  IFBEngineMemoryArena;
typedef IFBEngineMemoryArena*   IFBEngineMemoryArenaPtr;
typedef IFBEngineMemoryArena&   IFBEngineMemoryArenaRef;

//--------------------------------
// MEMORY MANAGER
//--------------------------------

struct IFBEngineMemoryManager {
    u64                     managed_memory_size_bytes;
    u64                     arena_size;
    memory                  managed_memory;
    IFBEngineMemoryArenaPtr arenas_reserved;
    IFBEngineMemoryArenaPtr arenas_released;
};

IFBEngineMemoryManager
ifb_engine_memory_manager_create_and_initialize(
    const u64    managed_memory_size_bytes,
    const u64    managed_memory_arena_size,
    const memory managed_memory);

u64
ifb_engine_memory_manager_space_reserved(
    IFBEngineMemoryManagerRef memory_manager_ref);

u64
ifb_engine_memory_manager_space_available(
    IFBEngineMemoryManagerRef memory_manager_ref);


//--------------------------------
// MEMORY
//--------------------------------

struct IFBEngineMemory {
    memory                 platform_memory;
    u64                    platform_memory_size_bytes;
    IFBEngineMemoryManager memory_manager_64kb;
    IFBEngineMemoryManager memory_manager_64mb;
};

IFBEngineMemoryPtr
ifb_engine_memory_create_and_initialize();

//--------------------------------
// MEMORY ARENA
//--------------------------------

struct IFBEngineMemoryArena {
    char                       tag[32];
    u64                        used_memory;
    IFBEngineMemoryManagerPtr  manager_ptr;
    IFBEngineMemoryArenaPtr    next;
    IFBEngineMemoryArenaPtr    previous;
    memory                     memory;
};

IFBEngineMemoryArenaPtr
ifb_engine_memory_arena_reserve(
          IFBEngineMemoryManagerPtr memory_manager_ptr,
    const char                      arena_tag[32]);

IFBEngineMemoryArenaPtr
ifb_engine_memory_arena_reserve_64kb(
    const char arena_tag[32]);

IFBEngineMemoryArenaPtr
ifb_engine_memory_arena_reserve_64mb(
    const char arena_tag[32]);

void
ifb_engine_memory_arena_release(
    IFBEngineMemoryArenaPtr arena_ptr);

memory
ifb_engine_memory_arena_bytes_push(
    IFBEngineMemoryArenaPtr arena_ptr,
    u64                     size_bytes);

void
ifb_engine_memory_arena_bytes_pop(
    IFBEngineMemoryArenaPtr arena_ptr,
    u64                     size_bytes);

void
ifb_engine_memory_arena_reset(
    IFBEngineMemoryArenaPtr arena_ptr);

#endif //IFB_ENGINE_MEMORY_HPP