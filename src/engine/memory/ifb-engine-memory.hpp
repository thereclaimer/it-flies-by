#ifndef IFB_ENGINE_MEMORY_HPP
#define IFB_ENGINE_MEMORY_HPP

#include <ifb.hpp>
#include <ifb-math.hpp>

#include "ifb-engine-platform.hpp"

#define ifb_engine_memory_kilobytes(n) n * 1024
#define ifb_engine_memory_megabytes(n) ifb_engine_memory_kilobytes(n) * 1024
#define ifb_engine_memory_gigabytes(n) ifb_engine_memory_megabytes(n) * 1024

/********************************************************************************************/
/* CONTEXT                                                                                  */
/********************************************************************************************/

namespace ifb_engine_memory {

    external void   context_create              (void);
    external void   context_destroy             (void);
    external size_t context_granularity         (void);
    external size_t context_page_size_small     (void);
    external size_t context_page_size_large     (void);
    external size_t context_reservation_count   (void);
    external size_t context_reserved_size_total (void);
};

/********************************************************************************************/
/* RESERVATION                                                                              */
/********************************************************************************************/

typedef handle IFBEngineMemoryReservation;

enum IFBEngineMemoryPageType_ { 
     IFBEngineMemoryPageType_Small = 0,
     IFBEngineMemoryPageType_Large = 1
};

typedef u32 IFBEngineMemoryPageType;

namespace ifb_engine_memory {

    external IFBEngineMemoryReservation 
    reserve(
        const char*                   tag_value,
        const size_t                  minimum_size,
        const IFBEngineMemoryPageType page_type);
    
    external void   release                    (const IFBEngineMemoryReservation reservation);
    external size_t reservation_space_total    (const IFBEngineMemoryReservation reservation);
    external size_t reservation_space_free     (const IFBEngineMemoryReservation reservation);
    external size_t reservation_space_occupied (const IFBEngineMemoryReservation reservation);
    external size_t reservation_page_size      (const IFBEngineMemoryReservation reservation);
    external size_t reservation_page_count     (const IFBEngineMemoryReservation reservation);
    external size_t reservation_region_count   (const IFBEngineMemoryReservation reservation);
};

/********************************************************************************************/
/* REGION                                                                                   */
/********************************************************************************************/

typedef handle IFBEngineMemoryRegion;

namespace ifb_engine_memory {

    external IFBEngineMemoryRegion
    region_create(
        const IFBEngineMemoryReservation reservation,
        const char*                      tag_value,
        const size_t                     minimum_size,
        const size_t                     arena_minimum_size);

    external void   region_destroy        (const IFBEngineMemoryRegion region);
    external size_t region_space_total    (const IFBEngineMemoryRegion region);
    external size_t region_space_free     (const IFBEngineMemoryRegion region);
    external size_t region_space_occupied (const IFBEngineMemoryRegion region);
    external size_t region_page_size      (const IFBEngineMemoryRegion region);
    external size_t region_page_count     (const IFBEngineMemoryRegion region);
    external size_t region_arena_count    (const IFBEngineMemoryRegion region);
};

/********************************************************************************************/
/* ARENA                                                                                    */
/********************************************************************************************/

typedef handle IFBEngineMemoryArena;

namespace ifb_engine_memory {

    external IFBEngineMemoryArena arena_commit         (const IFBEngineMemoryRegion region); 
    external void                 arena_decommit       (const IFBEngineMemoryArena arena); 
    external void                 arena_clear          (const IFBEngineMemoryArena arena); 
    external size_t               arena_space_total    (const IFBEngineMemoryArena arena);
    external size_t               arena_space_free     (const IFBEngineMemoryArena arena);
    external size_t               arena_space_occupied (const IFBEngineMemoryArena arena);
    external size_t               arena_page_size      (const IFBEngineMemoryArena arena);
    external size_t               arena_page_count     (const IFBEngineMemoryArena arena);

    external memory arena_push_bytes (const IFBEngineMemoryArena arena, const size_t size);
    external memory arena_pull_bytes (const IFBEngineMemoryArena arena, const size_t size);
}

/********************************************************************************************/
/* ALIGNMENT                                                                                */
/********************************************************************************************/

namespace ifb_engine_memory {

    inline u64  
    alignment_pow_2(
        const u64 size, 
        const u64 align_to) { 
        
        return(((size) + (align_to) - 1)&(~((align_to) - 1))); 
    }

    inline u64
    alignment_pow_2_down(
        const u64 size, 
        const u64 align_to) {
        
        return((size)&(~((align_to) - 1)));
    }
    
    inline u64
    memory_alignment_pow_2_pad(
        u64 size,
        u64 align_to) { 
        
        return((0-(size)) & ((align_to) - 1));
    }
    
    inline bool
    alignment_is_pow_2(
        u64 size) { 
        
        return((size)!=0 && ((size)&((size)-1))==0);
    }
    
    inline bool
    alignment_is_pow_2_or_zero(
        u64 size) {
        
        return((((size) - 1)&(size)) == 0);
    }
};

#endif //IFB_ENGINE_MEMORY_HPP