#ifndef IFB_ENGINE_MEMORY_HPP
#define IFB_ENGINE_MEMORY_HPP

#include <ifb.hpp>
#include <ifb-math.hpp>

#include "ifb-engine-platform.hpp"
#include "ifb-engine-stack.hpp"

#define ifb_engine_memory_kilobytes(n) n * 1024
#define ifb_engine_memory_megabytes(n) ifb_engine_memory_kilobytes(n) * 1024
#define ifb_engine_memory_gigabytes(n) ifb_engine_memory_megabytes(n) * 1024

struct IFBEngineMemoryArena;
struct IFBEngineMemoryRegion;
struct IFBEngineMemoryReservation;
struct IFBEngineMemoryRegion;
struct IFBEngineMemoryArena;

/************************************************************/
/* ALIGNMENT                                                */
/************************************************************/

namespace ifb_engine {

    inline u64  
    memory_alignment_pow_2(
        const u64 size, 
        const u64 align_to) { 
        
        return(((size) + (align_to) - 1)&(~((align_to) - 1))); 
    }

    inline u64
    memory_alignment_pow_2_down(
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
    memory_alignment_is_pow_2(
        u64 size) { 
        
        return((size)!=0 && ((size)&((size)-1))==0);
    }
    
    inline bool
    memory_alignment_is_pow_2_or_zero(
        u64 size) {
        
        return((((size) - 1)&(size)) == 0);
    }
};

/************************************************************/
/* MEMORY CONTEXT                                           */
/************************************************************/

struct IFBEngineMemoryContext {
    u64                          allocation_granularity;
    u64                          page_size;
    IFBEngineMemoryReservation*  reservations;
};

namespace ifb_engine {
    
    internal void                    memory_context_create  (void);
    internal void                    memory_context_destroy (void);
    internal IFBEngineMemoryContext& memory_context_get     (void);
};

/************************************************************/
/* RESERVATION                                              */
/************************************************************/

struct IFBEngineMemoryReservation {
    IFBEngineMemoryReservation* next;
    IFBEngineMemoryRegion*      regions;
    IFBTag                      tag;  
    IFBEngineStack              stack;
};

namespace ifb_engine {

    internal void 
    memory_reserve(
              IFBEngineMemoryReservation& reservation,
        const char*                       tag_value,
        const u64                         minimum_size);
};

/************************************************************/
/* REGION                                                   */
/************************************************************/

struct IFBEngineMemoryRegion {
    IFBEngineMemoryReservation* reservation;
    IFBEngineMemoryRegion*      next;
    IFBEngineMemoryArena*       arenas;
    IFBTag                      tag;
    IFBEngineStack              stack;
};

namespace ifb_engine {

    internal void
    memory_region_create(
              IFBEngineMemoryRegion&      region,
              IFBEngineMemoryReservation& reservation,
        const char*                       tag_value,
        const u64                         minimum_size);
};

/************************************************************/
/* ARENA                                                    */
/************************************************************/

struct IFBEngineMemoryArena {
    IFBEngineMemoryArena*  next;
    IFBEngineMemoryRegion* region;
    IFBEngineStack         stack;
};

namespace ifb_engine {

    internal void
    memory_arena_commit(
              IFBEngineMemoryRegion& region, 
              IFBEngineMemoryArena&  arena,
        const u64                    minimum_size);
    
    internal memory memory_arena_push_bytes (IFBEngineMemoryArena& arena, const u64 size);
    internal memory memory_arena_pull_bytes (IFBEngineMemoryArena& arena, const u64 size);
    internal void   memory_arena_clear      (IFBEngineMemoryArena& arena);
    internal void   memory_arena_decommit   (IFBEngineMemoryArena& arena);
};

#endif //IFB_ENGINE_MEMORY_HPP