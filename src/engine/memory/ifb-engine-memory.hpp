#ifndef IFB_ENGINE_MEMORY_HPP
#define IFB_ENGINE_MEMORY_HPP

#include <ifb.hpp>
#include <ifb-math.hpp>

#include "ifb-engine-platform.hpp"

#define ifb_engine_memory_kilobytes(n) n * 1024
#define ifb_engine_memory_megabytes(n) ifb_engine_memory_kilobytes(n) * 1024
#define ifb_engine_memory_gigabytes(n) ifb_engine_memory_megabytes(n) * 1024

#define IFB_ENGINE_MEMORY_REQUIREMENT  ifb_engine_memory_gigabytes(4)

struct IFBEngineMemoryArena        {};
struct IFBEngineMemoryRegion       {};
struct IFBEngineMemorySystemRegion {};
struct IFBEngineMemoryRegion       {};
struct IFBEngineMemoryArena        {};

/*****************************************************************************************************************************************/
/* ALIGNMENT                                                                                                                             */
/*****************************************************************************************************************************************/

inline u64  ifb_engine_memory_alignment_pow_2            (u64 size, u64 align_to) { return(((size) + (align_to) - 1)&(~((align_to) - 1)));}
inline u64  ifb_engine_memory_alignment_pow_2_down       (u64 size, u64 align_to) { return((size)&(~((align_to) - 1)));}
inline u64  ifb_engine_memory_alignment_pow_2_pad        (u64 size, u64 align_to) { return((0-(size)) & ((align_to) - 1));}
inline bool ifb_engine_memory_alignment_is_pow_2         (u64 size)               { return((size)!=0 && ((size)&((size)-1))==0);}
inline bool ifb_engine_memory_alignment_is_pow_2_or_zero (u64 size)               { return((((size) - 1)&(size)) == 0);}

/*********************************************/
/* ENGINE MEMORY                             */
/*********************************************/

struct IFBEngineMemory {
    memory                       start;
    u64                          total_size;
    u64                          position;
    u64                          page_size;
    IFBEngineMemorySystemRegion* system_regions;
};

internal void ifb_engine_memory_create  (void);
internal void ifb_engine_memory_destroy (void);

/***************************************************/
/* SYSTEM REGION                                   */
/***************************************************/

struct IFBEngineMemorySystemRegion {
    IFBTag                      tag;                  
    u64                         total_size;
    u64                         position;
    IFBEngineMemoryReservation* next;
    IFBEngineMemoryReservation* previous;
    IFBEngineMemoryRegion*      regions;
    memory                      start;
};

internal void 
ifb_engine_memory_system_region_reserve(
          IFBEngineMemoryRegion* region,
    const char*                  tag,
    const u64                    minimum_total_size,
    const u64                    minimum_arena_size);

/**********/
/* REGION */
/**********/


internal void
ifb_engine_memory_region_reserve(
    IFBEngineMemorySystemRegion* system_region,
    IFBEngineMemoryRegion*       region,
    const char*                  tag,
    const u64                    minimum_total_size,
    const u64                    minimum_arena_size);

struct IFBEngineMemoryRegion {
    IFBTag                tag;
    u64                   total_size;
    u64                   arena_size;
    IFBEngineMemoryArena* free_arenas;
    IFBEngineMemoryArena* reserved_arenas;    
};


struct IFBEngineMemoryArena {
    IFBEngineMemoryRegion* region;
    IFBEngineMemoryArena*  next;
    IFBEngineMemoryArena*  previous;
    u64                    size;
    u64                    position;
    memory                 start;
};


#endif //IFB_ENGINE_MEMORY_HPP