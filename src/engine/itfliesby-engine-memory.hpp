#ifndef ITFLIESBY_ENGINE_MEMORY_HPP
#define ITFLIESBY_ENGINE_MEMORY_HPP

#define ITFLIESBY_ENGINE_MEMORY_SIZE                  ITFLIESBY_MATH_MEGABYTES(512)

#define ITFLIESBY_ENGINE_PARTITION_SIZE_PHYSICS       ITFLIESBY_MATH_MEGABYTES(64)
#define ITFLIESBY_ENGINE_PARTITION_SIZE_CORE          ITFLIESBY_MATH_MEGABYTES(64)
#define ITFLIESBY_ENGINE_PARTITION_SIZE_RENDERER      ITFLIESBY_MATH_MEGABYTES(64)
#define ITFLIESBY_ENGINE_PARTITION_SIZE_MAPS          ITFLIESBY_MATH_MEGABYTES(64)


#define ITFLIESBY_ENGINE_ALLOCATOR_SIZE_CORE_SYSTEMS  ITFLIESBY_MATH_KILOBYTES(32)

struct ItfliesbyEngineMemoryPartitions {
    itfliesby_memory_partition physics;
    itfliesby_memory_partition maps;
    itfliesby_memory_partition core;
};

struct ItfliesbyEngineMemoryAllocators {
    itfliesby_memory_allocator_linear core_system_allocator;
};

struct ItfliesbyEngineMemory {
    itfliesby_memory_arena          arena;
    ItfliesbyEngineMemoryPartitions partitions;
    ItfliesbyEngineMemoryAllocators allocators;
};

#endif //ITFLIESBY_ENGINE_MEMORY_HPP