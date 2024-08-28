#ifndef IFB_ENGINE_MEMORY_HPP
#define IFB_ENGINE_MEMORY_HPP

#include <ifb.hpp>

/********************************************************************************************/
/* MEMORY MANAGER                                                                           */
/********************************************************************************************/

namespace ifb_engine {

    external void memory_manager_start_up  (void);
    external void memory_manager_shut_down (void);

    external const size_t memory_manager_page_size              (void);
    external const size_t memory_manager_allocation_granularity (void);
    external const size_t memory_manager_reservation_count      (void);
    external const size_t memory_manager_reservation_max        (void);
    external const memory memory_manager_next_reservation_start (void);

    external const size_t
    memory_manager_align_a_to_b(
        const size_t a, 
        const size_t b);
};

/********************************************************************************************/
/* RESERVATION                                                                              */
/********************************************************************************************/

typedef handle IFBEngineMemoryReservation;

namespace ifb_engine {

    external const size_t
    memory_reservation_calculate_minimum_size(
        const size_t struct_size,
        const size_t struct_alignment,
        const size_t struct_count);

    external const IFBEngineMemoryReservation 
    memory_reserve(
        const char*  tag_value,
        const size_t minimum_reservation_size,
        const size_t minimum_arena_size);

    external const IFBEngineMemoryReservation
    memory_reserve_struct_pool(
        const char*  tag_value,
        const size_t struct_size,
        const size_t struct_alignment,
        const size_t struct_count);

    external const IFBEngineMemoryReservation
    memory_reserve_arena_pool(
        const char*  tag_value,
        const size_t minimum_arena_size,
        const size_t arena_count);

    external const size_t memory_reservation_size_total (const IFBEngineMemoryReservation reservation);
    external const size_t memory_reservation_size_used  (const IFBEngineMemoryReservation reservation);
    external const size_t memory_reservation_size_free  (const IFBEngineMemoryReservation reservation);
    external const char*  memory_reservation_tag_value  (const IFBEngineMemoryReservation reservation);
    external void         memory_reservation_reset      (const IFBEngineMemoryReservation reservation);
};

/********************************************************************************************/
/* ARENA                                                                                    */
/********************************************************************************************/

typedef handle IFBEngineMemoryArena;

namespace ifb_engine {

    external const size_t 
    memory_arena_calculate_minimum_size_for_struct(
        const size_t struct_size,
        const size_t struct_alignment,
        const size_t struct_count);

    external const IFBEngineMemoryArena 
    memory_arena_commit(
        const IFBEngineMemoryReservation reservation);
    
    external void memory_arena_decommit (const IFBEngineMemoryArena arena);

    external const size_t memory_arena_size_total (const IFBEngineMemoryArena arena);
    external const size_t memory_arena_size_used  (const IFBEngineMemoryArena arena);
    external const size_t memory_arena_size_free  (const IFBEngineMemoryArena arena);

    external memory memory_arena_push_bytes(const IFBEngineMemoryArena arena, const size_t size);
    external void   memory_arena_pull_bytes(const IFBEngineMemoryArena arena, const size_t size);

    external memory memory_arena_push_bytes_aligned(const IFBEngineMemoryArena arena, const size_t size, const size_t alignment);
    external void   memory_arena_pull_bytes_aligned(const IFBEngineMemoryArena arena, const size_t size, const size_t alignment);

};

#endif //IFB_ENGINE_MEMORY_HPP