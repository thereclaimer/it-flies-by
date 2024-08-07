#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

using namespace ifb_engine_memory; 

global IFBEngineMemoryContext memory_context;


external void
context_create(
    void) {

    memory_context = {0};
    memory_context.allocation_granularity = ifb_engine_platform_memory_allocation_granularity();
    memory_context.page_size_small        = ifb_engine_platform_memory_page_size();

    ifb_engine_platform_memory_reserve(IFB_ENGINE_MEMORY_RESERVATION_TABLE_SIZE);

}

external void
context_destroy(
    void) {

}

external size_t
context_granularity(
    void) {

}

external size_t
context_page_size_small(
    void) {

}

external size_t
context_page_size_large(
    void) {

}

external size_t
context_reservation_count(
    void) {

}

external size_t
context_reserved_size_total(
    void) {

}
