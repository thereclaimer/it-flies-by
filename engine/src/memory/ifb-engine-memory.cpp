#pragma once

#include "ifb-engine-internal-memory.hpp" 
#include "ifb-engine-internal-config.hpp"
#include "ifb-engine-internal-context.hpp"

/**********************************************************************************/
/* MEMORY                                                                         */
/**********************************************************************************/

struct IFBEngineMemory {
    ifb_byte*  stack_buffer_ptr;
    IFBMemory* base_memory_ptr;
};

/**********************************************************************************/
/* RESERVE / RELEASE                                                              */
/**********************************************************************************/

inline const ifb_b8
ifb_engine::memory_reserve(
    ifb_void) {
    
    //get context info
    const IFBPlatformApi&  platform_api_ref  = ifb_engine::context_get_platform_api();
    const IFBEngineConfig& engine_config_ref = ifb_engine::context_get_config();
    IFBEngineMemory&       engine_memory_ref = ifb_engine::context_get_memory();

    //calculate sizes
    const ifb_u32 memory_size_stack       = ifb_macro_size_kilobytes(engine_config_ref.memory_stack_size_kb);
    const ifb_u64 memory_size_reservation = ifb_macro_size_gigabytes(engine_config_ref.memory_reservation_size_gb);

    //create the stack buffer
    ifb_local ifb_byte memory_stack_buffer[memory_size_stack];

    //create the memory
    const IFBMemory* memory_ptr = ifb_memory::create(
        platform_api_ref,
        memory_stack_buffer,
        memory_size_stack);
    if (!memory_ptr) return(false);

    //reserve the memory    
    if (!ifb_memory::reserve(_memory_core_handle,core_size_reservation)) {
        return(false);
    }

    //initialize the engine memory
    engine_memory_ref.stack_buffer_ptr = memory_stack_buffer;
    engine_memory_ref.base_memory_ptr  = memory_ptr;

    //we're done
    return(true);
}

inline const ifb_b8
ifb_engine::memory_release(
    ifb_void) {

    //get the memory
    IFBEngineMemory& memory_ref = ifb_engine::context_get_memory();

    //release the reserved memory
    const ifb_b8 result = ifb_memory::release(memory_ref.base_memory_ptr);

    //we're done
    return(result);
}

inline const ifb_b8 
ifb_engine::memory_get_info(
    IFBEngineMemoryInfo* memory_info_ptr) {

    //get the memory
    IFBEngineMemory& memory_ref = ifb_engine::context_get_memory();

    //sanity check
    ifb_b8 result = true;
    result &= memory_info_ptr             != NULL;
    result &= memory_ref.base_memory_ptr  != NULL;
    result &= memory_ref.stack_buffer_ptr != NULL;
    if (!result) return(false);

    //get the memory info
    memory_ref.reservation_size_total = ifb_memory::reservation_get_size_total     (memory_ref.base_memory_ptr);
    memory_ref.reservation_size_used  = ifb_memory::reservation_get_size_committed (memory_ref.base_memory_ptr);
    memory_ref.stack_size_total       = ifb_memory::stack_get_size_total           (memory_ref.base_memory_ptr);
    memory_ref.stack_size_used        = ifb_memory::stack_get_size_used            (memory_ref.base_memory_ptr);

    //we're done
    return(true);
}

inline IFBMemoryArena*
ifb_engine::memory_commit_arena_unmanaged(
    const ifb_u32 size_minimum) {

    //get the memory
    IFBEngineMemory& memory_ref = ifb_engine::context_get_memory();

    //sanity check
    ifb_b8 result = true;
    result &= memory_ref.base_memory_ptr  != NULL;
    result &= memory_ref.stack_buffer_ptr != NULL;
    if (!result) return(NULL);

    //commit the arena
    IFBMemoryArena* arena_ptr = ifb_memory::arena_commit(
        memory_ref.base_memory_ptr,
        size_minimum);
    
    //we're done
    return(arena_ptr);
}

inline IFBMemoryLinearArena*
ifb_engine::memory_commit_arena_linear(
    const ifb_u32 size_minimum) {

    //get the memory
    IFBEngineMemory& memory_ref = ifb_engine::context_get_memory();

    //sanity check
    ifb_b8 result = true;
    result &= memory_ref.base_memory_ptr  != NULL;
    result &= memory_ref.stack_buffer_ptr != NULL;
    if (!result) return(NULL);

    //commit the arena
    IFBMemoryLinearArena* linear_arena_ptr = ifb_memory::linear_arena_commit(
        memory_ref.base_memory_ptr,
        size_minimum);
    
    //we're done
    return(linear_arena_ptr);
}

inline IFBMemoryBlockArena*
ifb_engine::memory_commit_arena_block(
    const ifb_u32 block_size_minimum,
    const ifb_u32 block_count) {

    //get the memory
    IFBEngineMemory& memory_ref = ifb_engine::context_get_memory();

    //sanity check
    ifb_b8 result = true;
    result &= memory_ref.base_memory_ptr  != NULL;
    result &= memory_ref.stack_buffer_ptr != NULL;
    if (!result) return(NULL);

    //commit the arena
    IFBMemoryBlockArena* block_arena_ptr = ifb_memory::block_arena_commit(
        memory_ref.base_memory_ptr,
        block_size_minimum,
        block_count);

    //we're done
    return(block_arena_ptr);
}
