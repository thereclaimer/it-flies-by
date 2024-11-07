#pragma once

#include "ifb-engine-internal-memory.hpp"
#include "ifb-engine-internal.hpp"

/**********************************************************************************/
/* INLINE METHODS                                                                 */
/**********************************************************************************/

inline const IFBEngineMemoryTableIndexArenaDetail
ifb_engine::memory_arena_detail_table_insert(
          IFBEngineMemoryArenaDetailTable&     arena_detail_table_ref,
    const IFBEngineMemoryTableIndexArenaHeader arena_header_index,
    const ifb_u32                              arena_count) {

    //get the starting arena index
    const IFBEngineMemoryTableIndexArenaDetail starting_arena_index = arena_detail_table_ref.arena_count_current;

    //if we can't fit this number of arenas, we're done
    const ifb_size arena_count_new = starting_arena_index + arena_count;
    if (arena_count_new > arena_detail_table_ref.arena_count_max) {
        return(IFB_ENGINE_MEMORY_ARENA_DETAIL_INDEX_INVALID);
    }

    //initialize these arenas
    for (
        IFBEngineMemoryTableIndexArenaDetail arena_index = starting_arena_index;
        arena_index < arena_count_new;
        ++arena_index) {
        
        arena_detail_table_ref.columns.committed   [arena_index] = false;
        arena_detail_table_ref.columns.header_index[arena_index] = arena_header_index;
        arena_detail_table_ref.columns.size_used   [arena_index] = 0;
        arena_detail_table_ref.columns.pool_index  [arena_index] = arena_index - starting_arena_index;
    }

    //update the table
    arena_detail_table_ref.arena_count_current = arena_count_new;

    //we're done
    return(starting_arena_index);
}

inline const ifb_b8 
ifb_engine::memory_arena_detail_next_available_index(
          IFBEngineMemoryArenaDetailTable&       in_arena_detail_table_ref,
    const IFBEngineMemoryTableIndexArenaDetail   in_arena_detail_index_start,
    const IFBEngineMemoryTableIndexArenaHeader   in_arena_header_index,
          IFBEngineMemoryTableIndexArenaDetail& out_arena_detail_ref) {

    //set the out index
    out_arena_detail_ref = IFB_ENGINE_MEMORY_ARENA_DETAIL_INDEX_INVALID; 

    //sanity check
    if (in_arena_detail_index_start >= in_arena_detail_table_ref.arena_count_current) {
        return(false);
    }

    for (
        IFBEngineMemoryTableIndexArenaDetail arena_detail_index = in_arena_detail_index_start;
        arena_detail_index < in_arena_detail_table_ref.arena_count_current;
        ++arena_detail_index) {

        const ifb_b8                               detail_committed    = ifb_engine::memory_arena_detail_committed(in_arena_detail_table_ref, arena_detail_index);
        const IFBEngineMemoryTableIndexArenaHeader detail_header_index = ifb_engine::memory_arena_detail_header_index(in_arena_detail_table_ref, arena_detail_index);

        //if the header indexes don't match, we're in a different arena group
        if (detail_header_index != in_arena_header_index) {
            break;
        }

        //if this arena isn't committed and has a matching header, its available
        if (!detail_committed) {
            out_arena_detail_ref = arena_detail_index;
            break;
        }
    }

    //we're done
    return(out_arena_detail_ref != IFB_ENGINE_MEMORY_ARENA_DETAIL_INDEX_INVALID);
}

inline const ifb_b8
ifb_engine::memory_arena_detail_committed(
          IFBEngineMemoryArenaDetailTable& arena_table_detail_ref,
    const IFBEngineMemoryTableIndexArenaDetail  arena_detail_index) {

    const ifb_b8 committed = arena_table_detail_ref.columns.committed[arena_detail_index];

    return(committed);
}

inline const IFBEngineMemoryTableIndexArenaHeader
ifb_engine::memory_arena_detail_header_index(
          IFBEngineMemoryArenaDetailTable& arena_table_detail_ref,
    const IFBEngineMemoryTableIndexArenaDetail  arena_detail_index) {

    const IFBEngineMemoryTableIndexArenaHeader header_index = arena_table_detail_ref.columns.header_index[arena_detail_index];

    return(header_index);
}

inline const ifb_u32
ifb_engine::memory_arena_detail_size_used(
          IFBEngineMemoryArenaDetailTable& arena_table_detail_ref,
    const IFBEngineMemoryTableIndexArenaDetail  arena_detail_index) {

    const ifb_u32 size_used = arena_table_detail_ref.columns.size_used[arena_detail_index];

    return(size_used);
}

inline const ifb_index
ifb_engine::memory_arena_detail_pool_index(
          IFBEngineMemoryArenaDetailTable& arena_table_detail_ref,
    const IFBEngineMemoryTableIndexArenaDetail  arena_detail_index) {

    const ifb_index pool_index = arena_table_detail_ref.columns.pool_index[arena_detail_index];

    return(pool_index);
}

inline const ifb_void 
ifb_engine::memory_arena_detail_committed_set_true(
          IFBEngineMemoryArenaDetailTable& arena_table_detail_ref,
    const IFBEngineMemoryTableIndexArenaDetail  arena_detail_index) {

    arena_table_detail_ref.columns.committed[arena_detail_index] = true;
}

inline const ifb_void 
ifb_engine::memory_arena_detail_committed_set_false(
          IFBEngineMemoryArenaDetailTable& arena_table_detail_ref,
    const IFBEngineMemoryTableIndexArenaDetail  arena_detail_index) {

    arena_table_detail_ref.columns.committed[arena_detail_index] = false;
}


inline const ifb_void 
ifb_engine::memory_arena_detail_used_size_update(
          IFBEngineMemoryArenaDetailTable& arena_table_detail_ref, 
    const IFBEngineMemoryTableIndexArenaDetail  arena_detail_index,
    const ifb_size                         arena_used_size) {

    arena_table_detail_ref.columns.size_used[arena_detail_index] = arena_used_size;
}