#include "ifb-engine-assets-new.hpp"
#include "ifb-engine-assets-internal.hpp"

using namespace ifb_engine_assets;

internal void
memory_reserve(void) {

    //get the memory from the context
    IFBEngineAssetsContext& context = context_get();
    IFBEngineAssetsMemory& asset_memory = context.memory;

    //sizes
    const size_t asset_memory_reservation_size  = ifb_engine_memory_megabytes(64);
    const size_t asset_memory_region_size_index = ifb_engine_memory_kilobytes(64);
    const size_t asset_memory_region_size_read  = ifb_engine_memory_megabytes(8);
    const size_t asset_memory_region_size_data  = ifb_engine_memory_megabytes(8);

    //make the reservation
    asset_memory.reservation = 
        ifb_engine_memory::reserve(
            "ASSET RESERVATION",
            asset_memory_reservation_size,
            IFBEngineMemoryPageType_Small);
    
    ifb_assert(assets_reservation);

    //index region
    asset_memory.regions.index =
        ifb_engine_memory::region_create(
            asset_memory.reservation, 
            "ASSETS INDEX REGION", 
            asset_memory_region_size_index);
    ifb_assert(asset_memory.regions.index);

    //read region
    asset_memory.regions.read =
        ifb_engine_memory::region_create(
            asset_memory.reservation,
            "ASSETS READ REGION",
            asset_memory_region_size_read);
    ifb_assert(asset_memory.regions.index);

    //data region    
    asset_memory.regions.data = 
        ifb_engine_memory::region_create(
            asset_memory.reservation,
            "ASSETS DATA REGION",
            asset_memory_region_size_data);
    ifb_assert(asset_memory.regions.index);
}