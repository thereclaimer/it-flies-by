#ifndef IFB_ENGINE_MEMORY_HPP
#define IFB_ENGINE_MEMORY_HPP

#include <string.h>
#include <itfliesby-math.hpp>

#define IFB_ENGINE_MEMORY_REQUIREMENT ITFLIESBY_MATH_GIGABYTES(4)

//--------------------------------
// TYPES
//--------------------------------

typedef struct                  IFBEngineMemory;
typedef IFBEngineMemory*        IFBEngineMemory_Ptr;
typedef IFBEngineMemory&        IFBEngineMemory_Ref;

typedef struct                  IFBEngineMemoryManager;
typedef IFBEngineMemoryManager* IFBEngineMemoryManager_Ptr;
typedef IFBEngineMemoryManager& IFBEngineMemoryManager_Ref;

typedef struct                  IFBEngineMemoryArena;
typedef IFBEngineMemoryArena*   IFBEngineMemoryArena_Ptr;
typedef IFBEngineMemoryArena&   IFBEngineMemoryArena_Ref;

//--------------------------------
// MEMORY MANAGER
//--------------------------------

struct IFBEngineMemoryManager {
    u64                      managed_memory_size_bytes;
    u64                      arena_size;
    memory                   managed_memory;
    IFBEngineMemoryArena_Ptr arenas_reserved;
    IFBEngineMemoryArena_Ptr arenas_released;
};

IFBEngineMemoryManager
ifb_engine_memory_manager_create_and_initialize(
    const u64    managed_memory_size_bytes,
    const u64    managed_memory_arena_size,
    const memory managed_memory
);

u64
ifb_engine_memory_manager_space_reserved(
    IFBEngineMemoryManager_Ref memory_manager_ref);

u64
ifb_engine_memory_manager_space_available(
    IFBEngineMemoryManager_Ref memory_manager_ref);


//--------------------------------
// MEMORY
//--------------------------------

struct IFBEngineMemory {
    memory                 platform_memory;
    u64                    platform_memory_size_bytes;
    IFBEngineMemoryManager memory_manager_64kb;
    IFBEngineMemoryManager memory_manager_64mb;
};

global IFBEngineMemory ifb_engine_memory;

IFBEngineMemory_Ptr
ifb_engine_memory_create_and_initialize();


//--------------------------------
// MEMORY ARENA
//--------------------------------

struct IFBEngineMemoryArena {
    char                       tag[32];
    u64                        used_memory;
    IFBEngineMemoryManager_Ptr manager_ptr;
    IFBEngineMemoryArena_Ptr   next;
    IFBEngineMemoryArena_Ptr   previous;
    memory                     memory;
};

IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_reserve(
          IFBEngineMemoryManager_Ptr memory_manager_ptr,
    const char                       arena_tag[32]);

IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_reserve_64kb(
    const char arena_tag[32]);

IFBEngineMemoryArena_Ptr
ifb_engine_memory_arena_reserve_64mb(
    const char arena_tag[32]);

void
ifb_engine_memory_arena_release(
    IFBEngineMemoryArena_Ptr arena_ptr);

memory
ifb_engine_memory_arena_bytes_push(
    IFBEngineMemoryArena_Ptr arena_ptr,
    u64                      size_bytes);

void
ifb_engine_memory_arena_bytes_pop(
    IFBEngineMemoryArena_Ptr arena_ptr,
    u64                      size_bytes);

void
ifb_engine_memory_arena_reset(
    IFBEngineMemoryArena_Ptr arena_ptr);

#endif //IFB_ENGINE_MEMORY_HPP