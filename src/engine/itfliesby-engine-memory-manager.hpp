#ifndef ITFLIESBY_ENGINE_MEMORY_MANAGER_HPP
#define ITFLIESBY_ENGINE_MEMORY_MANAGER_HPP

#include <itfliesby.hpp>

struct ItfliesbyEngineMemoryArena {
    u64    size_bytes;
    char   tag[32];
};

struct ItfliesbyEngineMemoryArenaNode {
    ItfliesbyEngineMemoryArena*     arena;
    ItfliesbyEngineMemoryArenaNode* next;
};

struct ItfliesbyEngineMemoryManager {
    u64                             total_memory_size_bytes;
    ItfliesbyEngineMemoryArenaNode* arena_list;
};

internal ItfliesbyEngineMemoryArena*
itfliesby_engine_memory_arena_push(
    ItfliesbyEngineMemoryManager memory_manager,
    u64                          memory_size_bytes);

inline ItfliesbyEngineMemoryArenaNode*
itfliesby_engine_memory_arena_node_last(
    ItfliesbyEngineMemoryArenaNode* arena_list) {

    if (arena_list == NULL) {
        return(NULL);
    }

    ItfliesbyEngineMemoryArenaNode* last_node = NULL;

    for (
        ItfliesbyEngineMemoryArenaNode* node = arena_list;
        node->next != NULL;
        node = node->next) {

        last_node = node;
    }

    return(last_node);
}

inline u64
itfliesby_engine_memory_arena_allocation_requirement(
    u64 allocation_size_bytes) {

    u64 requirement = 
        sizeof(ItfliesbyEngineMemoryArenaNode) + 
        sizeof(ItfliesbyEngineMemoryArena)     + 
        allocation_size_bytes;

    return(requirement);
}

inline u64
itfliesby_engine_memory_arena_node_size(
    ItfliesbyEngineMemoryArenaNode* node) {

    u64 node_size = 
        sizeof(ItfliesbyEngineMemoryArenaNode) + 
        sizeof(ItfliesbyEngineMemoryArena)     + 
        node->arena->size_bytes;

    return(node_size);
}

//------------------------------------------------------
// This allows us to make custom allocators
//------------------------------------------------------

typedef memory
*funcptr_itfliesby_engine_memory_allocator_allocate(
    void
);

typedef void
*funcptr_itfliesby_engine_memory_allocator_free(
    memory
);

struct ItfliesbyEngineMemoryAllocator {
    ItfliesbyEngineMemoryArena                         arena;
    funcptr_itfliesby_engine_memory_allocator_allocate allocate;
    funcptr_itfliesby_engine_memory_allocator_free     free;
};


#endif //ITFLIESBY_ENGINE_MEMORY_MANAGER_HPP