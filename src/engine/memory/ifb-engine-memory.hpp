#ifndef IFB_ENGINE_MEMORY_HPP
#define IFB_ENGINE_MEMORY_HPP

#include <ifb.hpp>
#include <ifb-math.hpp>

#include "ifb-engine-platform.hpp"

#define IFB_ENGINE_MEMORY_REQUIREMENT IFB_MATH_GIGABYTES(4)

//---------------------------------
// TYPES
//---------------------------------

struct  IFBEngineMemory;
typedef IFBEngineMemory* IFBEngineMemoryPtr;
typedef IFBEngineMemory& IFBEngineMemoryRef;

struct  IFBEngineMemoryRegion;
typedef IFBEngineMemoryRegion* IFBEngineMemoryRegionPtr;
typedef IFBEngineMemoryRegion& IFBEngineMemoryRegionRef;

struct  IFBEngineMemoryArenaAllocator;
typedef IFBEngineMemoryArenaAllocator* IFBEngineMemoryArenaAllocatorPtr;
typedef IFBEngineMemoryArenaAllocator& IFBEngineMemoryArenaAllocatorRef;

struct  IFBEngineMemoryArena;
typedef IFBEngineMemoryArena* IFBEngineMemoryArenaPtr;
typedef IFBEngineMemoryArena& IFBEngineMemoryArenaRef;

//---------------------------------
// MEMORY
//---------------------------------

struct IFBEngineMemoryBlock {
    memory memory;
    u64    memory_size_bytes;
};

struct IFBEngineMemory {
    IFBEngineMemoryBlock     platform_memory_block;
    IFBEngineMemoryRegionPtr regions;
};

IFBEngineMemoryPtr
ifb_engine_memory_create_and_initialize();

IFBEngineMemoryPtr
ifb_engine_memory_get();

//---------------------------------
// REGION
//---------------------------------

struct IFBEngineMemoryRegion {
    IFBTag                           tag;
    IFBEngineMemoryBlock             block;
    IFBEngineMemoryRegionPtr         next;
    IFBEngineMemoryArenaAllocatorPtr arena_allocators;
};

IFBEngineMemoryRegionPtr
ifb_engine_memory_region_create(
    u64  region_size_bytes,
    char region_tag[32]);

//---------------------------------
// ARENA ALLOCATOR
//---------------------------------

struct IFBEngineMemoryArenaAllocator {
    IFBTag                  tag;
    IFBEngineMemoryBlock    block;

    struct {
        u64                     arena_size_bytes;
        IFBEngineMemoryArenaPtr reserved;
        IFBEngineMemoryArenaPtr available;
    } arenas;
};

IFBEngineMemoryArenaAllocatorPtr
ifb_engine_memory_arena_allocator_create(
    IFBEngineMemoryRegionRef region,
    u64                      arena_size_bytes;
    u64                      allocator_size_bytes);

//---------------------------------
// ARENA
//---------------------------------

struct IFBEngineMemoryArena {
    IFBEngineMemoryBlock    block;
    IFBEngineMemoryArenaPtr next;
    u64                     bytes_used;
};

IFBEngineMemoryArenaPtr
ifb_engine_memory_arena_reserve();

void
ifb_engine_memory_arena_release(
    IFBEngineMemoryArenaPtr arena_ptr);

memory
ifb_engine_memory_arena_bytes_push(
    IFBEngineMemoryArenaPtr arena_ptr,
    u64                     bytes_count);

void
ifb_engine_memory_arena_bytes_pop(
    IFBEngineMemoryArenaPtr arena_ptr,
    u64                     bytes_count);

#endif //IFB_ENGINE_MEMORY_HPP