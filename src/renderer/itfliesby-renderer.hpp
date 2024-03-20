#ifndef ITFLIESBY_RENDERER_HPP
#define ITFLIESBY_RENDERER_HPP

#include <itfliesby.hpp>

#define ITFLIESBY_RENDERER_MEMORY_SIZE_ARENA           ITFLIESBY_MATH_MEGABYTES(64)
#define ITFLIESBY_RENDERER_MEMORY_SIZE_PARTITION_CORE  ITFLIESBY_MATH_KILOBYTES(2)
#define ITFLIESBY_RENDERER_MEMORY_SIZE_ALLLOCATOR_CORE ITFLIESBY_MATH_KILOBYTES(1)


struct ItfliesbyRendererAllocators {
    itfliesby_memory_allocator_linear core_system_allocator;
};

struct ItfliesbyRendererPartitions {
    itfliesby_memory_partition core;
};

struct ItfliesbyRendererMemory {
    itfliesby_memory_arena      arena;
    ItfliesbyRendererPartitions partitions;
    ItfliesbyRendererAllocators allocators;
};

struct ItfliesbyRenderer {
    ItfliesbyRendererMemory memory;
    handle                  gl_context;
};

api ItfliesbyRenderer*
itfliesby_renderer_create(
    ItfliesbyPlatformApi platform,
    memory               memory,
    u64                  memory_size
);

api void
itfliesby_renderer_destroy(
    ItfliesbyRenderer* renderer
);

#endif //ITFLIESBY_RENDERER_HPP