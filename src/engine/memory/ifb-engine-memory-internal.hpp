#ifndef IFB_ENGINE_MEMORY_INTERNAL_HPP
#define IFB_ENGINE_MEMORY_INTERNAL_HPP

#include "ifb-engine-memory.hpp"

/********************************************************************************************/
/* FORWARD DECLARATIONS                                                                     */
/********************************************************************************************/

//context
struct IFBEngineMemoryContext;

//tables
struct IFBEngineMemoryReservationTable;
struct IFBEngineMemoryRegionTable;
struct IFBEngineMemoryArenaTable;

//implementations
struct IFBEngineMemoryReservation_Impl;
struct IFBEngineMemoryRegion_Impl;
struct IFBEngineMemoryArena_Impl;

/********************************************************************************************/
/* CONTEXT                                                                                  */
/********************************************************************************************/

struct IFBEngineMemoryContext {
    size_t                          allocation_granularity;
    size_t                          page_size_small;
    size_t                          page_size_large;
    IFBEngineMemoryReservationTable reservation_table;
};

namespace ifb_engine_memory {

    internal IFBEngineMemoryContext& context_get();
};

/********************************************************************************************/
/* RESERVATION                                                                              */
/********************************************************************************************/

#define IFB_ENGINE_MEMORY_RESERVATION_TABLE_SIZE ifb_engine_memory_megabytes(1)

struct IFBEngineMemoryReservationTable {
    size_t                           count_total;
    size_t                           count_used;
    size_t                           pages_used;
    size_t                           pages_available;
    IFBEngineMemoryReservation_Impl* reservations;
};

struct IFBEngineMemoryReservation_Impl {
    IFBTag                      tag;
    IFBEngineMemoryPageType     page_type;
    u64                         owner_process;
    u64                         owner_thread;
    size_t                      total_size;
    size_t                      page_size;
    size_t                      count_regions;
    memory                      start;
    IFBEngineMemoryRegionTable  region_table;
};

namespace ifb_engine_memory {

    internal void                             reservation_table_allocate (IFBEngineMemoryReservationTable& reservation_table);
    internal IFBEngineMemoryReservation_Impl* reservation_table_insert   (IFBEngineMemoryReservationTable& reservation_table);
};

/********************************************************************************************/
/* REGION                                                                                   */
/********************************************************************************************/

#define IFB_ENGINE_MEMORY_REGION_TABLE_SIZE ifb_engine_memory_megabytes(1)

struct IFBEngineMemoryRegionTable {
    size_t                      count_total;
    size_t                      count_used;
    IFBEngineMemoryRegion_Impl* regions;
};

struct IFBEngineMemoryRegion_Impl {
    IFBEngineMemoryReservation_Impl* reservation;
    IFBTag                           tag;
    size_t                           total_size;
    memory                           start;
    IFBEngineMemoryArenaTable        arena_table; 
};

namespace ifb_engine_memory {
    internal void                        region_table_allocate (IFBEngineMemoryRegionTable& region_table);
    internal IFBEngineMemoryRegion_Impl* region_table_insert   (IFBEngineMemoryRegionTable& region_table);
};


/********************************************************************************************/
/* ARENA                                                                                    */
/********************************************************************************************/

struct IFBEngineMemoryArenaTable {
    size_t                     count_total;
    size_t                     count_used;
    IFBEngineMemoryArena_Impl* arenas;
};

struct IFBEngineMemoryArena_Impl {
    IFBEngineMemoryRegion_Impl* region;    
    size_t                      total_size; 
    memory                      start;
    memory                      position;
};

namespace ifb_engine_memory {
    internal void                       arena_table_allocate (IFBEngineMemoryRegionTable& region_table);
    internal IFBEngineMemoryArena_Impl* arena_table_insert   (IFBEngineMemoryRegionTable& region_table);
};

#endif //IFB_ENGINE_MEMORY_INTERNAL_HPP