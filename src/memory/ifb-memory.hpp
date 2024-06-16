#ifndef IFB_MEMORY_HPP
#define IFB_MEMORY_HPP

#include <string.h>
#include <itfliesby-types.hpp>
#include <itfliesby-math.hpp>

#define IFB_MEMORY_ARENA_SIZE ITFLIESBY_MATH_KILOBYTES(64)

//--------------------------------
// TYPES
//--------------------------------

typedef struct            IFBMemoryManager;
typedef IFBMemoryManager* IFBMemoryManager_Ptr;
typedef IFBMemoryManager& IFBMemoryManager_Ref;

typedef struct            IFBMemoryArena;
typedef IFBMemoryArena*   IFBMemoryArena_Ptr;
typedef IFBMemoryArena&   IFBMemoryArena_Ref;

typedef struct IFBMemoryArenaHandle;

//--------------------------------
// MEMORY MANAGER
//--------------------------------


struct IFBMemoryManager {
    u64                managed_memory_size_bytes;
    memory             managed_memory;
    IFBMemoryArena_Ptr arenas_reserved;
    IFBMemoryArena_Ptr arenas_released;
};

global IFBMemoryManager _memory_manager;

void
ifb_memory_manager_create_and_initialize(
    const u64    managed_memory_size_bytes,
    const memory managed_memory
);

void
ifb_memory_manager_destroy();

u64
ifb_memory_manager_space_reserved();

u64
ifb_memory_manager_space_available();


//--------------------------------
// MEMORY ARENA
//--------------------------------

struct IFBMemoryArena {
    char               tag[32];
    u64                used_memory;  
    IFBMemoryArena_Ptr next;
    IFBMemoryArena_Ptr previous;
    memory             memory;
};

IFBMemoryArena_Ptr
ifb_memory_arena_reserve(
    const char tag[32]);

void
ifb_memory_arena_release(
    IFBMemoryArena_Ptr arena_ptr);

memory
ifb_memory_arena_bytes_push(
    IFBMemoryArena_Ptr arena_ptr,
    u64                size_bytes);

void
ifb_memory_arena_bytes_pop(
    IFBMemoryArena_Ptr arena_ptr,
    u64                size_bytes);

void
ifb_memory_arena_reset(
    IFBMemoryArena_Ptr arena_ptr);

#endif //IFB_MEMORY_HPP