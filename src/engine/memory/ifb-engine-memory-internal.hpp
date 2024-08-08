#ifndef IFB_ENGINE_MEMORY_INTERNAL_HPP
#define IFB_ENGINE_MEMORY_INTERNAL_HPP

#include "ifb-engine-memory.hpp"

/********************************************************************************************/
/* FORWARD DECLARATIONS                                                                     */
/********************************************************************************************/

//context
struct IFBEngineMemoryContext;

//implementations
struct IFBEngineMemoryReservation_Impl;
struct IFBEngineMemoryRegion_Impl;
struct IFBEngineMemoryArena_Impl;

/********************************************************************************************/
/* CONTEXT                                                                                  */
/********************************************************************************************/

struct IFBEngineMemoryContext {
    size_t                           allocation_granularity;
    size_t                           page_size_small;
    size_t                           page_size_large;
    memory                           initial_reservation;
    memory                           initial_commit;
    IFBEngineMemoryReservation_Impl* reservations_free;
    IFBEngineMemoryReservation_Impl* reservations_used;
};

namespace ifb_engine_memory {

    internal IFBEngineMemoryContext& context_get();
};

/********************************************************************************************/
/* RESERVATION                                                                              */
/********************************************************************************************/

struct IFBEngineMemoryReservation_Impl {
    IFBEngineMemoryReservation_Impl* next;
    IFBEngineMemoryReservation_Impl* previous;
    IFBEngineMemoryRegion_Impl*      free_regions;
    IFBEngineMemoryRegion_Impl*      used_regions;
    memory                           start;
    IFBTag                           tag;
    IFBEngineMemoryPageType          paage_type;
    u64                              owner_process;
    u64                              owner_thread;
    size_t                           reservation_size;
    size_t                           region_list_size;
    size_t                           total_size;
    size_t                           page_size;
};

/********************************************************************************************/
/* REGION                                                                                   */
/********************************************************************************************/

struct IFBEngineMemoryRegion_Impl {
    IFBEngineMemoryReservation_Impl* reservation;
    IFBEngineMemoryRegion_Impl*      next;
    IFBEngineMemoryRegion_Impl*      previous;
    IFBEngineMemoryArena_Impl*       committed_arenas; 
    IFBEngineMemoryArena_Impl*       uncommitted_arenas; 
    memory                           start;
    size_t                           total_size;
    size_t                           region_size;
    size_t                           arena_list_size;
    size_t                           arena_count;
    IFBTag                           tag;
};

/********************************************************************************************/
/* ARENA                                                                                    */
/********************************************************************************************/

struct IFBEngineMemoryArena_Impl {
    IFBEngineMemoryRegion_Impl* region;
    IFBEngineMemoryArena_Impl*  next;
    IFBEngineMemoryArena_Impl*  previous;
    memory                      start;
    memory                      commit;
    size_t                      total_size; 
    size_t                      position;
};

#endif //IFB_ENGINE_MEMORY_INTERNAL_HPP