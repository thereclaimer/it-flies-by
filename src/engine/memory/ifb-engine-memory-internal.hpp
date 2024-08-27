#ifndef IFB_ENGINE_MEMORY_INTERNAL_HPP
#define IFB_ENGINE_MEMORY_INTERNAL_HPP

#define IFB_ENGINE_MEMORY_RESERVATIONS_MAX 128

#include <ifb-engine-memory.hpp>

/********************************************************************************************/
/* FORWARD DECLARATIONS                                                                     */
/********************************************************************************************/

struct IFBEngineMemoryManager;
struct IFBEngineMemoryReservationTable;
struct IFBEngineMemoryReservation_Impl;
struct IFBEngineMemoryArenaTable;
struct IFBEngineMemoryArena_Impl;

/********************************************************************************************/
/* ARENA                                                                                    */
/********************************************************************************************/

struct IFBEngineMemoryArena_Impl {
    memory reservation;
    memory commit;
    size_t position;
};

struct IFBEngineMemoryArenaKey {
    size_t reservation_index;
    size_t 
};

struct IFBEngineMemoryArenaTable {
    size_t row_count;
    size_t arena_size;
    union {
        memory table_start;
        struct {
            memory* reservation;
            memory* commit;
            size_t* position;
        } columns;
    };
};

/********************************************************************************************/
/* RESERVATION                                                                              */
/********************************************************************************************/

struct IFBEngineMemoryReservation_Impl {
    IFBTag                    tag;
    size_t                    size_total;
    size_t                    size_usable;
    memory                    start;
    IFBEngineMemoryArenaTable arena_table;
};

struct IFBEngineMemoryReservationTable {
    size_t                          max;
    size_t                          count;
    memory                          next_start;
    IFBEngineMemoryReservation_Impl array[IFB_ENGINE_MEMORY_RESERVATIONS_MAX];
};

namespace ifb_engine_internal {

    internal const size_t 
    memory_reservation_arena_table_size(
        const size_t arena_count);
};

/********************************************************************************************/
/* MANAGER                                                                                  */
/********************************************************************************************/

struct IFBEngineMemoryManager {
    IFBEngineMemoryReservationTable reservation_table;
    size_t                          page_size;
    size_t                          allocation_granularity;
};

namespace ifb_engine_internal {

    internal IFBEngineMemoryReservation_Impl* memory_manager_next_reservation(void);    
};

#endif //IFB_ENGINE_MEMORY_INTERNAL_HPP