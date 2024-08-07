#pragma once

#include "ifb-engine-memory.hpp"

global IFBEngineMemoryContext memory_context;

internal void
ifb_engine::memory_context_create(
    void) {

    memory_context = {0};
    memory_context.allocation_granularity = ifb_engine_platform_memory_allocation_granularity();
    memory_context.page_size              = ifb_engine_platform_memory_page_size();
    memory_context.reservations           = NULL;
}

internal void
ifb_engine::memory_context_destroy(
    void) {

}

internal IFBEngineMemoryContext&
ifb_engine::memory_context_get(
    void) {

    return(&memory_context);
}