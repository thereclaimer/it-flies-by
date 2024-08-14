#pragma once

#include "ifb-engine-memory.hpp"
#include "ifb-engine-memory-internal.hpp"

/********************************************************************************************/
/* EXTERNAL                                                                                 */
/********************************************************************************************/

external const IFBEngineMemoryReservation
ifb_engine_memory::reserve_memory(
    const char*                   tag_value,
    const size_t                  minimum_size,
    const IFBEngineMemoryPageType page_type) {

    //calculate sizes
    const size_t page_size              = ifb_engine_memory::context_page_size(page_type);
    const size_t reservation_size       = ifb_engine_memory::context_align_to_allocation_granularity(minimum_size);
    const size_t struct_size            = sizeof(IFBEngineMemoryReservation_Impl);
    const size_t aligned_struct_size    = ifb_engine_memory::context_align_to_allocation_granularity(aligned_struct_size); 
    const size_t total_reservation_size = reservation_size + aligned_struct_size;

    //make the reservation
    memory reserved_memory = 
        page_type == IFBEngineMemoryPageType_Small
        ? ifb_engine_platform_memory_reserve_small_pages(reservation_size)
        : ifb_engine_platform_memory_reserve_large_pages(reservation_size);
    ifb_assert(reserved_memory);

    //commit the area we are using for the struct
    const memory struct_start = reserved_memory + reservation_size;
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        (IFBEngineMemoryReservation_Impl*)ifb_engine_platform_memory_commit(
            struct_start,
            aligned_struct_size);
    ifb_assert(reservation_impl);

    //initialize the struct
    reservation_impl->next         = NULL;
    reservation_impl->previous     = NULL;
    reservation_impl->regions      = NULL;
    reservation_impl->start        = reserved_memory;
    reservation_impl->tag          = ifb_tag(tag_value);
    reservation_impl->page_type    = page_type;
    reservation_impl->owner_thread = ifb_engine_platform_thread_id();
    reservation_impl->useable_size = reservation_size;
    reservation_impl->total_size   = total_reservation_size;
    reservation_impl->page_size    = page_size;

    //add the reservation to the list
    ifb_engine_memory::context_add_reservation(reservation_impl);

    //return the reservation
    return(reservation_impl);
}

external void
ifb_engine_memory::release_memory(
    const IFBEngineMemoryReservation reservation) {
    
    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    //remove the reservation from the list
    ifb_engine_memory::context_remove_reservation(reservation_impl);

    //release the memory
    ifb_engine_platform_memory_release(
        reservation_impl->start,
        reservation_impl->total_size);
}

external const size_t
ifb_engine_memory::reservation_space_total(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    return(reservation_impl->total_size);
}

external const size_t 
reservation_space_useable(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    return(reservation_impl->useable_size);
}


external const size_t
ifb_engine_memory::reservation_space_free(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    size_t space_free = reservation_impl->useable_size;

    for (
        IFBEngineMemoryRegion_Impl* region = reservation_impl->regions;
        region != NULL;
        region = region->next) {

        space_free -= region->total_size;
    }

    return(space_free);
}

external const size_t
ifb_engine_memory::reservation_space_occupied(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    size_t space_occupied = 0;

    for (
        IFBEngineMemoryRegion_Impl* region = reservation_impl->regions;
        region != NULL;
        region = region->next) {

        space_occupied += region->total_size;
    }

    return(space_occupied);
}

external const size_t
ifb_engine_memory::reservation_page_size(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    return(reservation_impl->page_size);
}

external const size_t
ifb_engine_memory::reservation_page_count(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    const size_t page_count = reservation_impl->useable_size / reservation_impl->page_size;

    return(page_count);
}

external const size_t
ifb_engine_memory::reservation_region_count(
    const IFBEngineMemoryReservation reservation) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    size_t region_count = 0;

    for (
        IFBEngineMemoryRegion_Impl* region = reservation_impl->regions;
        region != NULL; 
        region = region->next) {

        ++region_count;
    }

    return(region_count);
}

external const size_t 
ifb_engine_memory::reservation_align_to_page_size(
    const IFBEngineMemoryReservation reservation,
    const size_t                     size) {

    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation; 
    ifb_assert(reservation_impl && reservation_impl->start);

    const size_t aligned_size =
        ifb_engine_memory::context_align_to_page(
            reservation_impl->page_type,
            size);
        
    return(aligned_size);
}

/********************************************************************************************/
/* INTERNAL                                                                                 */
/********************************************************************************************/

internal const memory 
reservation_position(
    const IFBEngineMemoryReservation_Impl* reservation) {

    ifb_assert(reservation && reservation->start);

    memory position = reservation->start;

    for (
        IFBEngineMemoryRegion_Impl* region = reservation->regions;
        region != NULL;
        region = region->next) {

        position += region->total_size;
    }

    return(position);
}

internal void 
reservation_add_region(
    IFBEngineMemoryReservation_Impl* reservation,
    IFBEngineMemoryRegion_Impl*      region) {

    ifb_assert(reservation && region);

    region->next = reservation->regions;
    if (reservation->regions->previous) {
        reservation->regions->previous = region;
    }
    reservation->regions = region;
}