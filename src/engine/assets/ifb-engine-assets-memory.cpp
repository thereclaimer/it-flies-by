#pragma once

#include "ifb-engine-assets-new.hpp"
#include "ifb-engine-assets-internal.hpp"

global IFBEngineAssetsMemory asset_memory;

internal void 
ifb_engine_assets::memory_reserve(
    void) {

    asset_memory = {0};

    //sizes
    const size_t asset_reservation_size    = ifb_engine_memory_gigabytes(1);

    const size_t asset_region_size_file    = ifb_engine_memory_kilobytes(64);
    const size_t asset_region_size_request = ifb_engine_memory_kilobytes(64);
    const size_t asset_region_size_data    = ifb_engine_memory_megabytes(512);

    const size_t asset_arena_size_file    = ifb_engine_memory_kilobytes(4);
    const size_t asset_arena_size_request = ifb_engine_memory_kilobytes(4);
    const size_t asset_arena_size_data    = ifb_engine_memory_kilobytes(64);

    //make the reservation
    const IFBEngineMemoryReservation asset_reservation = 
        ifb_engine_memory::reserve_memory(
            "ASSETS",
            asset_reservation_size,
            IFBEngineMemoryPageType_Small);
    ifb_assert(asset_reservation);


    //create the regions
    const IFBEngineMemoryRegion asset_region_file = 
        ifb_engine_memory::region_create(
            asset_reservation,
            "ASSET FILES",
            asset_region_size_file,
            asset_arena_size_file);
    ifb_assert(asset_region_file);

    const IFBEngineMemoryRegion asset_region_request = 
        ifb_engine_memory::region_create(
            asset_reservation,
            "ASSET REQUESTS",
            asset_region_size_request,
            asset_arena_size_request);
    ifb_assert(asset_region_request);

    const IFBEngineMemoryRegion asset_region_data = 
        ifb_engine_memory::region_create(
            asset_reservation,
            "ASSET DATA",
            asset_region_size_data,
            asset_arena_size_data);
    ifb_assert(asset_region_data);

    //initialize the struct
    asset_memory.reservation     = asset_reservation;
    asset_memory.regions.file    = asset_region_file;
    asset_memory.regions.request = asset_region_request;
    asset_memory.regions.data    = asset_region_data;
}

internal void 
ifb_engine_assets::memory_release(
    void) {

    ifb_engine_memory::release_memory(asset_memory.reservation);
    asset_memory = {0}; 
}

internal const IFBEngineMemoryArena
ifb_engine_assets::memory_arena_file_commit(
    void) {

    const IFBEngineMemoryArena file_arena = 
        ifb_engine_memory::arena_commit(asset_memory.regions.file);

    ifb_assert(file_arena);

    return(file_arena);
}

internal const IFBEngineMemoryArena 
ifb_engine_assets::memory_arena_request_commit(
    void) {

    const IFBEngineMemoryArena request_arena = 
        ifb_engine_memory::arena_commit(asset_memory.regions.request);

    ifb_assert(request_arena);

    return(request_arena);
}

internal const IFBEngineMemoryArena 
ifb_engine_assets::memory_arena_data_commit(
    void) {

    const IFBEngineMemoryArena data_arena = 
        ifb_engine_memory::arena_commit(asset_memory.regions.data);

    ifb_assert(data_arena);

    return(data_arena);
}

internal void 
ifb_engine_assets::memory_arena_decommit(
    const IFBEngineMemoryArena arena) {

    ifb_engine_memory::arena_decommit(arena);
}