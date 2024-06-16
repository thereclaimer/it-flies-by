#pragma once

#include "ifb-engine-memory.hpp"

internal IFBEngineMemoryManager
ifb_engine_memory_manager_create_and_initialize(
    const u64    managed_memory_size_bytes,
    const u64    managed_memory_arena_size,
    const memory managed_memory) {

    ifb_assert(
        managed_memory                                         &&
        managed_memory_size_bytes >= managed_memory_arena_size &&
        managed_memory_size_bytes % managed_memory_arena_size == 0);

    //initialize the memory manager
    IFBEngineMemoryManager memory_manager = {0};
    memory_manager.arenas_reserved           = NULL;
    memory_manager.arenas_released           = NULL;
    memory_manager.arena_size                = managed_memory_arena_size;
    memory_manager.managed_memory            = managed_memory;
    memory_manager.managed_memory_size_bytes = managed_memory_size_bytes;

    return(memory_manager);
}

internal u64
ifb_engine_memory_manager_space_reserved(
    IFBEngineMemoryManager_Ref memory_manager_ref) {

    if (!memory_manager_ref.arenas_reserved) {
        return(0);
    }

    u64 reserved_size_bytes = 0;

    for (
        IFBEngineMemoryArena_Ptr arena = memory_manager_ref.arenas_reserved;
        arena->next != NULL;
        arena = arena->next) {

        reserved_size_bytes += memory_manager_ref.arena_size;
    }

    return(reserved_size_bytes);
}

internal u64
ifb_engine_memory_manager_space_available(
    IFBEngineMemoryManager_Ptr memory_manager_ptr) {

    ifb_assert(memory_manager_ptr);

    IFBEngineMemoryArena_Ptr arenas_reserved_ptr = memory_manager_ptr->arenas_reserved;

    u64 memory_manager_size_bytes = memory_manager_ptr->managed_memory_size_bytes;
    u64 arena_size                = memory_manager_ptr->arena_size;


    if (!arenas_reserved_ptr) {
        return(memory_manager_size_bytes);
    }

    u64 available_size_bytes = 0;

    for (
        IFBEngineMemoryArena_Ptr arena = arenas_reserved_ptr; 
        arena->next != NULL;
        arena = arena->next) {

        memory_manager_size_bytes -= arena_size;
    }

    return(available_size_bytes);
}
