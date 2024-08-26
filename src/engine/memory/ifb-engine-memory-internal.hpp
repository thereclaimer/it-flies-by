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

struct IFBEngineMemoryArenaTable {
    size_t row_count;
    size_t arena_size;
    size_t reservation_index;
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
    size_t                    struct_size;
    size_t                    struct_alignment;
    size_t                    struct_count;
    size_t                    struct_max;
    memory                    start;
    IFBEngineMemoryArenaTable arena_table;
};

struct IFBEngineMemoryReservationTable {
    size_t                          max;
    size_t                          count;
    memory                          next_start;
    IFBEngineMemoryReservation_Impl array[IFB_ENGINE_MEMORY_RESERVATIONS_MAX];
};

namespace ifb_engine {

    internal const size_t 
    memory_reservation_arena_table_size(
        const size_t aligned_reservation_size,
        const size_t aligned_arena_size);
};

/********************************************************************************************/
/* MANAGER                                                                                  */
/********************************************************************************************/

struct IFBEngineMemoryManager {
    IFBEngineMemoryReservationTable reservation_table;
    size_t                          page_size;
    size_t                          allocation_granularity;
};

#endif //IFB_ENGINE_MEMORY_INTERNAL_HPP