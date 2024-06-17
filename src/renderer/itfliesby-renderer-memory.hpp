#ifndef ITFLIESBY_RENDERER_MEMORY_HPP
#define ITFLIESBY_RENDERER_MEMORY_HPP

#include <ifb.hpp>
#include <common/itfliesby-memory.hpp>

#define ITFLIESBY_RENDERER_MEMORY_SIZE_BYTES                       IFB_MATH_MEGABYTES(64)

#define ITFLIESBY_RENDERER_MEMORY_PARTITION_CORE_SIZE_BYTES        IFB_MATH_KILOBYTES(32)

#define ITFLIESBY_RENDERER_MEMORY_PARTITION_UNIFORM_BUFFER_BYTES   IFB_MATH_MEGABYTES(2)

#define ITFLIESBY_RENDERER_MEMORY_ALLOCATOR_CORE_SYSTEM_SIZE_BYTES IFB_MATH_KILOBYTES(16)
#define ITFLIESBY_RENDERER_MEMORY_ALLOCATOR_UNIFORM_BUFFER_BYTES   IFB_MATH_MEGABYTES(1)

struct ItfliesbyRendererMemory {

    itfliesby_memory_arena arena;
    
    struct {
        itfliesby_memory_partition core;
        itfliesby_memory_partition uniform_buffers;
    } partitions;

    struct {
        itfliesby_memory_allocator_linear core_system_allocator;
        itfliesby_memory_allocator_linear uniform_buffer_allocator;
    } allocators;
};

#endif //ITFLIESBY_RENDERER_MEMORY_HPP