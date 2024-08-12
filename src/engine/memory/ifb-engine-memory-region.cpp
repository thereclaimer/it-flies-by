#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

external IFBEngineMemoryRegion
ifb_engine_memory::region_create(
    const IFBEngineMemoryReservation reservation,
    const char*                      tag_value,
    const size_t                     minimum_size,
    const size_t                     arena_minimum_size) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation;
    ifb_assert(reservation_impl);

    //get the start and size of the region
    const memory region_start = ifb_engine_memory::reservation_position(reservation);
    const size_t region_size  = ifb_engine_memory::reservation_align_to_page_size(reservation,minimum_size);

    //get the arena and arena list size
    const size_t arena_size       = ifb_engine_memory::reservation_align_to_page_size(reservation,arena_minimum_size); 
    const size_t page_size        = ifb_engine_memory::reservation_page_size(reservation);
    const size_t arena_count      = region_size / arena_size;
    const size_t arena_list_size  = sizeof(IFBEngineMemoryArena_Impl) * arena_count;

    //calculate our memory used for the region pointer and arena list
    const memory region_memory_offset = region_start + region_size;

    //the size is the list size plus one page for the pointer itself
    const size_t region_memory_size = arena_list_size + page_size;

    //commit the memory for tracking
    IFBEngineMemoryRegion_Impl* region_impl =
        ifb_engine_platform_memory_commit(
            region_memory_offset,
            region_memory_size);
    ifb_assert(region_impl);

    //initialize the list
    memory arena_list_memory = ((memory)region_impl) + page_size; 
    IFBEngineMemoryArena_Impl* arena_list = (IFBEngineMemoryArena_Impl*)arena_list_memory; 
    for (
        size_t arena_index = 0;
        arena_index < arena_count;
        ++arena_index) {

        IFBEngineMemoryArena_Impl* current_arena  = &arena_list[arena_index];
        IFBEngineMemoryArena_Impl* previous_arena = arena_index == 0 ? NULL : &arena_list[arena_index - 1];

        if (previous) {
            previous_arena->next = current;
        }

        memory reserved_memory_start = region_start + (arena_size * arena_index);

        current_arena->region                 = region_impl;
        current_arena->next                   = NULL;
        current_arena->previous               = previous_arena;
        current_arena->reserved_memory_start  = reserved_memory_start; 
        current_arena->committed_memory_start = NULL;
        current_arena->commit                 = NULL;
        current_arena->size                   = 0;
        current_arena->position               = 0;
    }

    //initialize the structure
    region_impl->reservation        = reservation;
    region_impl->next               = NULL;
    region_impl->previous           = NULL;
    region_impl->committed_arenas   = NULL;
    region_impl->uncommitted_arenas = arena_list;
    region_impl->start              = region_start;
    region_impl->total_size         = region_size + region_memory_size;
    region_impl->region_size        = region_size;
    region_impl->arena_list_size    = arena_list_size;
    region_impl->arena_count        = arena_count;
    region_impl->tag                = ifb_tag(tag_value);

    //add the region to the list
    ifb_engine_memory::reservation_add_region(region_impl);

    //return the region
    return(region_impl);
}

external size_t
ifb_engine_memory::region_space_total(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    return(region_impl->total_size);
}

external size_t
ifb_engine_memory::region_space_free(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    size_t space_free = region_impl->region_size;

    for (
        IFBEngineMemoryArena_Impl* arena = region_impl->committed_arenas;
        arena != NULL && arena->next != NULL;
        arena = arena->next) {

        space_free -= arena->total_size;
    }

    return(space_free);        
}

external size_t
ifb_engine_memory::region_space_occupied(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    size_t space_occupied = 0;

    for (
        IFBEngineMemoryArena_Impl* arena = region_impl->committed_arenas;
        arena != NULL && arena->next != NULL;
        arena = arena->next) {

        space_occupied += arena->total_size;
    }

    return(space_occupied);
}

external size_t
ifb_engine_memory::region_page_size(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    return(region_impl->reservation->page_size);
}

external size_t
ifb_engine_memory::region_page_count(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    const size_t page_size  = region_impl->reservation->page_size;
    const size_t page_count = region_impl->region_size / page_size; 

    return(page_count);
}

external size_t
ifb_engine_memory::region_arena_count(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    return(region_impl->arena_count);
}