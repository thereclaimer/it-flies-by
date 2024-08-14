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
    u64                              owner_process_id;
    IFBEngineMemoryReservation_Impl* reservations;
};

namespace ifb_engine_memory {

    internal IFBEngineMemoryContext& context_get                (void);
    internal void                    context_add_reservation    (IFBEngineMemoryReservation_Impl* reservation);
    internal void                    context_remove_reservation (IFBEngineMemoryReservation_Impl* reservation);
};

/********************************************************************************************/
/* RESERVATION                                                                              */
/********************************************************************************************/

struct IFBEngineMemoryReservation_Impl {
    IFBEngineMemoryReservation_Impl* next;
    IFBEngineMemoryReservation_Impl* previous;
    IFBEngineMemoryRegion_Impl*      regions;
    memory                           start;
    IFBTag                           tag;
    IFBEngineMemoryPageType          page_type;
    u64                              owner_thread;
    size_t                           useable_size;
    size_t                           total_size;
    size_t                           page_size;
};

namespace ifb_engine_memory {

    internal const memory reservation_position   (const IFBEngineMemoryReservation_Impl* reservation);
    
    internal void
    reservation_add_region(
        const IFBEngineMemoryReservation_Impl* reservation,
        const IFBEngineMemoryRegion_Impl*      region);
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
    size_t                           useable_size;
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
    memory                      reserved_memory_start;
    memory                      committed_memory_start;
    size_t                      size; 
    size_t                      position;
};

#endif //IFB_ENGINE_MEMORY_INTERNAL_HPP