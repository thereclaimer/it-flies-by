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

u64
ifb_engine_memory_space_availabe_bytes();

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

u64
ifb_engine_memory_region_total_size_bytes(
    IFBEngineMemoryRegionPtr region_ptr);
u64
ifb_engine_memory_region_total_required_size_bytes(
    u64 region_size_bytes);

u64
ifb_engine_memory_region_space_availabe_bytes(
    IFBEngineMemoryRegionPtr region_ptr);

//---------------------------------
// ARENA ALLOCATOR
//---------------------------------

struct IFBEngineMemoryArenaAllocator {
    IFBTag                           tag;
    IFBEngineMemoryBlock             block;
    IFBEngineMemoryArenaAllocatorPtr next;

    struct {
        u64                          arena_size_bytes;
        IFBEngineMemoryArenaPtr      reserved;
        IFBEngineMemoryArenaPtr      available;
    } arenas;
};

IFBEngineMemoryArenaAllocatorPtr
ifb_engine_memory_arena_allocator_create(
    IFBEngineMemoryRegionPtr region_ptr,
    char                     region_tag[32],
    u64                      arena_size_bytes,
    u64                      allocator_size_bytes);

u64
ifb_engine_memory_arena_allocator_total_size_bytes(
    IFBEngineMemoryArenaAllocatorPtr allocator_ptr);

u64
ifb_engine_memory_arena_allocator_total_required_size_bytes(
    u64 allocator_size_bytes);


//---------------------------------
// ARENA
//---------------------------------

struct IFBEngineMemoryArena {
    IFBEngineMemoryBlock    block;
    IFBEngineMemoryArenaPtr next;
    u64                     bytes_used;
};

u64
ifb_engine_memory_arena_total_size_bytes(
    IFBEngineMemoryArenaPtr allocator_ptr);

u64
ifb_engine_memory_arena_total_required_size_bytes(
    u64 arena_size_bytes);

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