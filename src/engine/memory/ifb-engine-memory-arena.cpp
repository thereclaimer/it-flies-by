#pragma once

#include "ifb-engine-memory-internal.hpp"
#include "ifb-engine-platform.hpp"

external const size_t 
ifb_engine::memory_arena_calculate_minimum_size_for_struct(
    const size_t struct_size,
    const size_t struct_alignment,
    const size_t struct_count) {

    //align the struct size
    const size_t struct_size_aligned = 
        ifb_engine::memory_manager_align_a_to_b(
            struct_size,
            struct_alignment);

    //get the total struct count size
    const size_t total_struct_size_aligned = 
        struct_size_aligned * struct_count;

    //align the total size to a page size
    const size_t page_size = ifb_engine::memory_manager_page_size();

    const size_t minimum_size =
        ifb_engine::memory_manager_align_a_to_b(
            total_struct_size_aligned,
            page_size);

    return(minimum_size);
}

external const IFBEngineMemoryArena 
ifb_engine::memory_arena_commit(
    const IFBEngineMemoryReservation reservation) {

    //get our stuff
    IFBEngineMemoryReservation_Impl* reservation_impl = (IFBEngineMemoryReservation_Impl*)reservation;
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;

    const memory reservation_start = reservation_impl->start;
    const size_t arena_size        = arena_table.arena_size; 

    //sanity check
    ifb_assert(
        reservation_impl  &&
        reservation_start &&
        arena_size);

    //find the first available uncommitted arena
    for (
        size_t arena_index = 0;
        arena_table.row_count;
        ++arena_index) {

        if (!arena_table.columns.commit[arena_index]) {
            
            //we have an arena, get the starting address
            const memory commit_start  = reservation_start + (arena_size * arena_index);

            //commit the arena
            arena_table.columns.commit[arena_index] =
                ifb_engine_platform_memory_commit(
                    commit_start,
                    arena_size);

            //reset the position        
            arena_table.columns.position[arena_index] = 0;

            //return the key
            return(&arena_table.columns.key[arena_index]);
        }
    }

    //no available arenas, we're done
    return(NULL);
}

external void 
ifb_engine::memory_arena_decommit(
    const IFBEngineMemoryArena arena) {

    //get our stuff
    IFBEngineMemoryArenaKey* arena_key = (IFBEngineMemoryArenaKey*)arena;
    ifb_assert(arena_key);

    const size_t reservation_index = arena_key->reservation_index;
    const size_t arena_index       = arena_key->arena_index; 

    //get the reservation
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        ifb_engine_internal::memory_manager_reservation_at_index(reservation_index);

    ifb_assert(reservation_impl);

    //get the table
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;
    ifb_assert(arena_index < arena_table.row_count);

    //if this is committed, decommit it
    if (arena_table.columns.commit[arena_index]) {
        
        ifb_engine_platform_memory_decommit(
            arena_table.columns.commit[arena_index],
            arena_table.arena_size);

        arena_table.columns.commit[arena_index]   = NULL;
        arena_table.columns.position[arena_index] = NULL;
    }
}

external const size_t
ifb_engine::memory_arena_size_total(
    const IFBEngineMemoryArena arena) {

    IFBEngineMemoryArenaKey* arena_key = (IFBEngineMemoryArenaKey*)arena;
    ifb_assert(arena_key);

    //get the reservation
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        ifb_engine_internal::memory_manager_reservation_at_index(arena_key->reservation_index);

    ifb_assert(reservation_impl);

    return(reservation_impl->arena_table.arena_size);
}

external const size_t
ifb_engine::memory_arena_size_used(
    const IFBEngineMemoryArena arena) {

    //get our stuff
    IFBEngineMemoryArenaKey* arena_key = (IFBEngineMemoryArenaKey*)arena;
    ifb_assert(arena_key);

    const size_t reservation_index = arena_key->reservation_index;
    const size_t arena_index       = arena_key->arena_index; 

    //get the reservation
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        ifb_engine_internal::memory_manager_reservation_at_index(reservation_index);

    ifb_assert(reservation_impl);

    //get the table
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;
    ifb_assert(arena_index < arena_table.row_count);

    //return the position as size used
    const size_t arena_size_used = arena_table.columns.position[arena_index];
    return(arena_size_used); 
}

external const size_t
ifb_engine::memory_arena_size_free(
    const IFBEngineMemoryArena arena) {

    //get our stuff
    IFBEngineMemoryArenaKey* arena_key = (IFBEngineMemoryArenaKey*)arena;
    ifb_assert(arena_key);

    const size_t reservation_index = arena_key->reservation_index;
    const size_t arena_index       = arena_key->arena_index; 

    //get the reservation
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        ifb_engine_internal::memory_manager_reservation_at_index(reservation_index);

    ifb_assert(reservation_impl);

    //get the table
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;
    ifb_assert(arena_index < arena_table.row_count);

    //return the size available
    const size_t arena_size_used      = arena_table.columns.position[arena_index];
    const size_t arena_size_available = arena_table.arena_size - arena_size_used;  

    return(arena_size_available);
}

external memory 
ifb_engine::memory_arena_push_bytes(
    const IFBEngineMemoryArena arena,
    const size_t               size) {

    //get our stuff
    IFBEngineMemoryArenaKey* arena_key = (IFBEngineMemoryArenaKey*)arena;
    ifb_assert(arena_key);

    const size_t reservation_index = arena_key->reservation_index;
    const size_t arena_index       = arena_key->arena_index; 

    //get the reservation
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        ifb_engine_internal::memory_manager_reservation_at_index(reservation_index);

    ifb_assert(reservation_impl);

    //get the table
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;
    ifb_assert(arena_index < arena_table.row_count);

    const size_t position_current = arena_table.columns.position[arena_index]; 
    const size_t position_new     = position_current + size;
    if (position_new > arena_table.arena_size) {
        return(NULL);
    }

    const memory commit_start = arena_table.columns.commit[arena_index];
    const memory arena_memory = commit_start + position_current; 
    
    arena_table.columns.position[arena_index] = position_new;    

    return(arena_memory);
}

external void   
ifb_engine::memory_arena_pull_bytes(
    const IFBEngineMemoryArena arena,
    const size_t               size) {

    // NOTE(SAM): this one is not as straighforward
    // when overflowing an arena, dont do anything, just return null
    // but pulling too much?
    // not as clear cut
    // I'm thinking the best thing to do is to just set the used size to 0
    // better to not allocate than to not free

    //get our stuff
    IFBEngineMemoryArenaKey* arena_key = (IFBEngineMemoryArenaKey*)arena;
    ifb_assert(arena_key);

    const size_t reservation_index = arena_key->reservation_index;
    const size_t arena_index       = arena_key->arena_index; 

    //get the reservation
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        ifb_engine_internal::memory_manager_reservation_at_index(reservation_index);

    ifb_assert(reservation_impl);

    //get the table
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;
    ifb_assert(arena_index < arena_table.row_count);

    //if the size is greater than the current position, set the new position to 0
    const size_t position_current = arena_table.columns.position[arena_index]; 
    const size_t position_new = 
        position_current > size
            ? 0
            : position_current - size;

    //set the position
    arena_table.columns.position[arena_index] = position_new;
}

external memory 
ifb_engine::memory_arena_push_bytes_aligned(
    const IFBEngineMemoryArena arena, 
    const size_t               size,
    const size_t               alignment) {

    //get our stuff
    IFBEngineMemoryArenaKey* arena_key = (IFBEngineMemoryArenaKey*)arena;
    ifb_assert(arena_key);

    const size_t reservation_index = arena_key->reservation_index;
    const size_t arena_index       = arena_key->arena_index; 

    //get the reservation
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        ifb_engine_internal::memory_manager_reservation_at_index(reservation_index);

    ifb_assert(reservation_impl);

    //get the table
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;
    ifb_assert(arena_index < arena_table.row_count);

    //align the size
    const size_t size_aligned = 
        ifb_engine::memory_manager_align_a_to_b(
            size,
            alignment);

    const size_t position_current = arena_table.columns.position[arena_index]; 
    const size_t position_new     = position_current + size_aligned;
    if (position_new > arena_table.arena_size) {
        return(NULL);
    }

    const memory commit_start = arena_table.columns.commit[arena_index];
    const memory arena_memory = commit_start + position_current; 
    
    arena_table.columns.position[arena_index] = position_new;    

    return(arena_memory);

}

external void
ifb_engine::memory_arena_pull_bytes_aligned(
    const IFBEngineMemoryArena arena, 
    const size_t               size,
    const size_t               alignment) {

    // NOTE(SAM): same deal here, see ifb_engine::memory_arena_pull_bytes()

    //get our stuff
    IFBEngineMemoryArenaKey* arena_key = (IFBEngineMemoryArenaKey*)arena;
    ifb_assert(arena_key);

    const size_t reservation_index = arena_key->reservation_index;
    const size_t arena_index       = arena_key->arena_index; 

    //get the reservation
    IFBEngineMemoryReservation_Impl* reservation_impl = 
        ifb_engine_internal::memory_manager_reservation_at_index(reservation_index);

    ifb_assert(reservation_impl);

    //get the table
    IFBEngineMemoryArenaTable& arena_table = reservation_impl->arena_table;
    ifb_assert(arena_index < arena_table.row_count);

    //align the size
    const size_t size_aligned = 
        ifb_engine::memory_manager_align_a_to_b(
            size,
            alignment);

    //if the size is greater than the current position, set the new position to 0
    const size_t position_current = arena_table.columns.position[arena_index]; 
    const size_t position_new = 
        position_current > size_aligned
            ? 0
            : position_current - size_aligned;

    //set the position
    arena_table.columns.position[arena_index] = position_new;
}