#ifndef IFB_ENGINE_CORE_HPP
#define IFB_ENGINE_CORE_HPP

#include <r-libs.hpp>

#include "ifb-engine.hpp"

#include "ifb-engine-internal-asset.hpp"

struct IFBEngineCoreMemory {
    RHNDMemoryReservation rhnd_reservation_ifb;
    RHNDMemoryRegion      rhnd_region_engine_core;
    RHNDMemoryArena       rhnd_arena_system;
    RHNDMemoryArena       rhnd_arena_frame;
};

struct IFBEngineCore {
    IFBEngineCoreMemory memory;
};

namespace ifb_engine {

    ifb_internal const r_b8 
    core_initialize(
        const RHNDMemoryReservation in_rhnd_reservation_ifb,
        const RHNDMemoryRegion      in_rhnd_region_engine_core,
        const RHNDMemoryArena       in_rhnd_arena_system,
        const RHNDMemoryArena       in_rhnd_arena_frame,
              IFBEngineCore&       out_engine_core_ref);


    ifb_internal ifb_memory core_system_memory_push         (const ifb_size size);
    ifb_internal ifb_memory core_system_memory_push_aligned (const ifb_size size, const ifb_size alignment);

    ifb_internal const RHNDMemoryRegion
    core_memory_create_arena_pool(
        const ifb_size arena_size,
        const ifb_size arena_count);
};

#define ifb_engine_core_memory_push_struct(struct)        \
    (struct*)ifb_engine::core_system_memory_push_aligned( \
        sizeof(struct),                                   \
        alignof(struct))                                  \

#define ifb_engine_core_memory_push_array(type,count) \
    (type*)ifb_engine::core_system_memory_push(       \
        sizeof(type) * count)                         \


#endif //IFB_ENGINE_CORE_HPP