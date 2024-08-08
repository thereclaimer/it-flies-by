#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

using namespace ifb_engine_memory;

external IFBEngineMemoryRegion
region_create(
    const IFBEngineMemoryReservation reservation,
    const char*                      tag_value,
    const size_t                     minimum_size,
    const size_t                     arena_minimum_size) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation;
    ifb_assert(reservation_impl);

    //calculate all the sizes we are working with
    const size_t page_size         = reservation_impl->page_size;
    const size_t arena_size        = alignment_pow_2(arena_minimum_size,page_size);
    const size_t region_size       = alignment_pow_2(minimum_size,page_size);
    const size_t arena_count       = region_size / arena_size; 
    const size_t arena_node_size   = sizeof(IFBEngineMemoryArena_Impl);
    const size_t arena_list_size   = arena_node_size * arena_count;
    const size_t total_region_size = region_size + arena_list_size;   
    const size_t free_space        = reservation_space_free();
    const size_t occupied_space    = reservation_space_occupied();

    //confirm we can fit the region
    ifb_assert(free_space >= total_region_size);

    //get the next region node
    ifb_assert(reservation_impl->free_regions);
    IFBEngineMemoryRegion_Impl* region = reservation_impl->free_regions;

    //update the free list
    reservation_impl->free_regions = region->next;

    //update the new region
    region->previous = NULL;
    region->next     = reservation_impl->used_regions;

    //add the new region to the used list
    reservation_impl->used_regions = region;

    //get the start of this region
    const memory region_start = reservation_impl->start + occupied_space; 

    //commit the memory for the arena list
    memory arena_list_memory_start = region_start + region_size;
    memory arena_list_memory = 
        ifb_engine_platform_memory_commit(
            arena_list_memory_start,
            arena_list_size);
    ifb_assert(arena_list_memory);

    //update the arena list pointers
    region->uncommitted_arenas = (IFBEngineMemoryArena_Impl*)arena_list_memory; 
    IFBEngineMemoryArena_Impl* previous = region->uncommitted_arenas; 
    IFBEngineMemoryArena_Impl* current  = NULL;
    previous->previous = NULL;

    for (
        size_t offset = 0;
        offset < arena_list_size;
        offset += arena_node_size) {

        previous->next  = (IFBEngineMemoryArena_Impl*)((memory)previous + arena_node_size);
        current         = previous;

        current->region     = region;
        current->next       = NULL;
        current->previous   = previous;
        current->start      = region_start + offset;
        current->commit     = NULL;
        current->total_size = arena_size;
        current->position   = 0;

        previous = current;
    }

    //everything else
    region->committed_arenas   = NULL;
    region->reservation        = reservation_impl;
    region->committed_arenas   = NULL;
    region->start              = region_start;
    region->total_size         = total_region_size;
    region->region_size        = region_size;
    region->arena_list_size    = arena_list_size;
    region->arena_count        = arena_count;
    region->tag                = ifb_tag(tag_value);
}

external size_t
region_space_total(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    return(region_impl->total_size);
}

external size_t
region_space_free(
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
region_space_occupied(
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
region_page_size(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    return(region_impl->reservation->page_size);
}

external size_t
region_page_count(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    const size_t page_size  = region_impl->reservation->page_size;
    const size_t page_count = region_impl->region_size / page_size; 

    return(page_count);
}

external size_t
region_arena_count(
    const IFBEngineMemoryRegion region) {

    IFBEngineMemoryRegion_Impl* region_impl = (IFBEngineMemoryRegion_Impl*)region;
    ifb_assert(region_impl);

    return(region_impl->arena_count);
}
