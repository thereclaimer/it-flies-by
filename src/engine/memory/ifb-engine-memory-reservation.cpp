#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

using namespace ifb_engine_memory;

external IFBEngineMemoryReservation 
reserve(
    const char*                   tag_value,
    const size_t                  minimum_size,
    const IFBEngineMemoryPageType page_type) {

    IFBEngineMemoryContext& context = context_get();

    ifb_assert(context.reservations_free);

    //get the page size
    const size_t page_size = 
        page_type == IFBEngineMemoryPageType_Small
        ? context.page_size_small
        : context.page_size_large;

    //align the reservation size to the allocation granularity
    const size_t reservation_size = alignment_pow_2(minimum_size, context.allocation_granularity);
    
    //we will reserve this much for the region tracking
    const size_t region_list_size = context.allocation_granularity;

    //both together is the total size
    const size_t total_reservation_size = reservation_size + region_list_size; 

    //make the reservation
    const memory reserved_memory = 
        page_type == IFBEngineMemoryPageType_Small
        ? ifb_engine_platform_memory_reserve_small_pages(total_reservation_size)        
        : ifb_engine_platform_memory_reserve_large_pages(total_reservation_size);        
    ifb_assert(reserved_memory);

    //commit the region list memory
    const memory region_list_start = reserved_memory + reservation_size;
    const memory region_list_memory =
        ifb_engine_platform_memory_commit(
            region_list_start,
            region_list_size); 
    ifb_assert(region_list_memory);

    //get the next free reservation node and update
    IFBEngineMemoryReservation_Impl* reservation_node = context.reservations_free;
    
    //update the free list
    context.reservations_free = reservation_node->next;
    
    //update the current reservation node
    reservation_node->previous = NULL;
    reservation_node->next     = context.reservations_used;
    
    //update the used list
    context.reservations_used = reservation_node; 

    //set the free region pointers up
    const size_t region_node_size = sizeof(IFBEngineMemoryRegion_Impl);
    reservation_node->free_regions = (IFBEngineMemoryRegion_Impl*)region_list_memory;
    IFBEngineMemoryRegion_Impl* previous = reservation_node->free_regions;
    IFBEngineMemoryRegion_Impl* current  = NULL;
    previous->previous = NULL;

    //initialize the free region list
    for (
        size_t offset = 0;
        offset < region_list_size;
        offset += region_node_size) {

        previous->next = (IFBEngineMemoryRegion_Impl*)((memory)previous + region_node_size);
        current        = previous->next;
        
        current->reservation = reservation_node;
        current->next        = NULL;
        current->previous    = previous;
        current->arenas      = NULL;
        current->start       = NULL;
        current->total_size  = 0;
        current->tag         = {0};

        previous = current;
    }

    //update everything else
    reservation_node->used_regions     = NULL; 
    reservation_node->start            = reserved_memory;
    reservation_node->tag              = ifb_tag(tag_value);
    reservation_node->page_type        = page_type;
    reservation_node->owner_process    = ifb_engine_platform_process_id();
    reservation_node->owner_thread     = ifb_engine_platform_thread_id();
    reservation_node->reservation_size = reservation_size; 
    reservation_node->region_list_size = region_list_size;
    reservation_node->total_size       = total_reservation_size;
    reservation_node->page_size        = page_size;  
}

external void
release(
    const IFBEngineMemoryReservation reservation) {
    
    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    //release the memory
    ifb_engine_platform_memory_release(
        reservation_impl->start,
        reservation_impl->total_size);

    //put this reservation back in the free list
    IFBEngineMemoryContext& context = context_get();
    reservation_impl->previous = reservation_impl->next;
    reservation_impl->next     = context.reservations_free;
    context.reservations_free->previous = reservation_impl;
    context.reservations_free = reservation_impl;

    //de-initialize our other stuff
    reservation_node->used_regions     = NULL; 
    reservation_node->start            = NULL;
    reservation_node->tag              = {0};
    reservation_node->page_type        = 0;
    reservation_node->owner_process    = 0; 
    reservation_node->owner_thread     = 0;
    reservation_node->reservation_size = 0; 
    reservation_node->region_list_size = 0;
    reservation_node->total_size       = 0;
    reservation_node->page_size        = 0;  
}

external size_t
reservation_space_total(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    return(reservation_impl->reservation_size);
}

external size_t
reservation_space_free(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    size_t space_free = reservation_impl->reservation_size;

    for (
        IFBEngineMemoryRegion_Impl* region = reservation_impl->used_regions;
        region != NULL && region->next != NULL;
        region = region->next) {

        space_free -= region->total_size;
    }

    return(space_free);
}

external size_t
reservation_space_occupied(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    size_t space_occupied = 0;

    for (
        IFBEngineMemoryRegion_Impl* region = reservation_impl->used_regions;
        region != NULL && region->next != NULL;
        region = region->next) {

        space_occupied += region->total_size;
    }

    return(space_occupied);
}

external size_t
reservation_page_size(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    return(reservation_impl->page_size);
}

external size_t
reservation_page_count(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    const size_t page_count = reservation_impl->reservation_size / reservation_impl->page_size;

    return(page_count);
}

external size_t
reservation_region_count(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    size_t region_count = 0;

    for (
        IFBEngineMemoryRegion_Impl* region = reservation_impl->used_regions;
        region != NULL && region->next != NULL;
        region = region->next) {

        ++region_count;
    }

    return(region_count);
}