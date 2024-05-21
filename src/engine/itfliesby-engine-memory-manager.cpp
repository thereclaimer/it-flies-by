#pragma once

#include "itfliesby-engine-memory-manager.hpp"
#include "itfliesby-engine-globals.hpp"

internal ItfliesbyEngineMemoryManager*
itfliesby_engine_memory_manager_create_and_init() {

    u64 memory_size_bytes = ITFLIESBY_MATH_GIGABYTES(2);

    memory base_memory = platform_api.memory_allocate(memory_size_bytes);

    ItfliesbyEngineMemoryManager* memory_manager = (ItfliesbyEngineMemoryManager*)base_memory;
    memory_manager->total_memory_size_bytes = memory_size_bytes;
    memory_manager->arena_list              = NULL;

    return(memory_manager);
}

internal u64
itfliesby_engine_memory_manager_used_bytes(
    ItfliesbyEngineMemoryManager* memory_manager) {

    ItfliesbyEngineMemoryArenaNode* memory_manager_arena_node_list = memory_manager->arena_list;

    u64 used_memory_bytes = sizeof(ItfliesbyEngineMemoryManager);

    for (
        ItfliesbyEngineMemoryArenaNode* node = memory_manager_arena_node_list;
        node->next != NULL;
        node = node->next) {

        used_memory_bytes +=
            itfliesby_engine_memory_arena_node_size(node);
    }

    return(used_memory_bytes);
};

internal ItfliesbyEngineMemoryArena*
itfliesby_engine_memory_arena_push(
    ItfliesbyEngineMemoryManager* memory_manager,
    u64                           memory_size_bytes,
    char                          memory_arena_tag[32]) {

    ItfliesbyEngineMemoryArenaNode* memory_manager_arena_node_list = memory_manager->arena_list;
    ItfliesbyEngineMemoryArenaNode* new_node        = NULL;
    memory                          new_node_memory = NULL;
    
    //first, determine if we have the space for the new arena
    u64 current_used_memory = 
        itfliesby_engine_memory_manager_used_bytes(memory_manager);

    u64 allocation_space_requirement = 
        itfliesby_engine_memory_arena_allocation_requirement(memory_size_bytes);

    if (current_used_memory + allocation_space_requirement > memory_size_bytes) {
        return(NULL);
    }

    //create our new node
    if (memory_manager->arena_list == NULL) {

        memory base_memory     = (memory)memory_manager;
        new_node_memory = base_memory + sizeof(ItfliesbyEngineMemoryManager);

        new_node = (ItfliesbyEngineMemoryArenaNode*)new_node_memory;
        memory_manager->arena_list = new_node;
    }
    else {

        //get the last node
        ItfliesbyEngineMemoryArenaNode* last_node = 
            itfliesby_engine_memory_arena_node_last(memory_manager_arena_node_list);

        //get the size of the last node
        u64 last_node_size = 
            itfliesby_engine_memory_arena_node_size(last_node);
    
        memory last_node_memory = (memory)last_node;
        new_node_memory  = last_node_memory + last_node_size;

        new_node = (ItfliesbyEngineMemoryArenaNode*)new_node_memory;
        last_node->next = new_node; 
    }

    //create the new arena
    memory arena_memory = new_node_memory + sizeof(ItfliesbyEngineMemoryArenaNode);
    ItfliesbyEngineMemoryArena* new_arena = (ItfliesbyEngineMemoryArena*)arena_memory;
    strcpy(
        new_arena->tag,
        memory_arena_tag
    );
    new_arena->size_bytes = memory_size_bytes;
    
    new_node->arena = new_arena;
    
    return(new_arena);
}
