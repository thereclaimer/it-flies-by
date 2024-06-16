#pragma once

#include "ifb-memory.hpp"

internal void
ifb_memory_manager_create_and_initialize(
    const u64    managed_memory_size_bytes,
    const memory managed_memory) {

    ITFLIESBY_ASSERT(
        managed_memory                                    &&
        managed_memory_size_bytes > IFB_MEMORY_ARENA_SIZE &&
        managed_memory_size_bytes % IFB_MEMORY_ARENA_SIZE == 0);

    //initialize the memory manager
    _memory_manager = {0};
    _memory_manager.arenas_reserved = NULL;
    _memory_manager.arenas_released = NULL;
    _memory_manager.managed_memory            = managed_memory;
    _memory_manager.managed_memory_size_bytes = managed_memory_size_bytes;
}

internal void
ifb_memory_manager_destroy() {
    _memory_manager = {0};
}

internal u64
ifb_memory_manager_space_reserved() {

    if (!_memory_manager.arenas_reserved) {
        return(0);
    }

    u64 reserved_size_bytes = 0;

    for (
        IFBMemoryArena_Ptr arena = _memory_manager.arenas_reserved;
        arena->next != NULL;
        arena = arena->next) {

        reserved_size_bytes += IFB_MEMORY_ARENA_SIZE;
    }

    return(reserved_size_bytes);
}

internal u64
ifb_memory_manager_space_available() {

    if (!_memory_manager.arenas_reserved) {
        return(_memory_manager.managed_memory_size_bytes);
    }

    u64 available_size_bytes = 0;

    for (
        IFBMemoryArena_Ptr arena = _memory_manager.arenas_reserved;
        arena->next != NULL;
        arena = arena->next) {

        _memory_manager.managed_memory_size_bytes -= IFB_MEMORY_ARENA_SIZE;
    }

    return(available_size_bytes);
}
