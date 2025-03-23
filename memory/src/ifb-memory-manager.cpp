#pragma once

#include "ifb-memory.hpp"
#include "ifb-memory-manager-init.cpp"

/**********************************************************************************/
/* MEMORY MANAGER                                                                 */
/**********************************************************************************/

const IFBMemoryManagerID
ifb_memory::manager_create(
    const IFBMemoryStack memory_stack,
    const IFBU64         memory_size_reservation,
    const IFBU32         memory_size_arena) {

    //init struct
    IFBMemoryManagerInit init;
    init.args.stack_start      = memory_stack; 
    init.args.size_reservation = memory_size_reservation; 
    init.args.size_arena       = memory_size_arena; 

    //initialize the memory manager
    ifb_memory::manager_init_step_0_validate_args         (init);
    ifb_memory::manager_init_step_1_get_system_info       (init);
    ifb_memory::manager_init_step_3_calculate_sizes       (init);
    ifb_memory::manager_init_step_2_allocate_manager      (init);
    ifb_memory::manager_init_step_4_reserve_system_memory (init);
    ifb_memory::manager_init_step_5_set_properties        (init);
    ifb_memory::manager_init_step_6_cleanup               (init);

    //get the handle and result
    const IFBMemoryManagerID manager_id = init.result 
        ? init.manager_id
        : IFB_MEMORY_INVALID_VALUE;

    //we're done
    return(manager_id);
}

const IFBB8      
ifb_memory::manager_destroy(
    const IFBMemoryStack     memory_stack,
    const IFBMemoryManagerID memory_manager_id) {

    return(false);
}

inline IFBMemoryManager*
ifb_memory::manager_load_and_assert_valid(
    const IFBMemoryStack     stack,
    const IFBMemoryManagerID manager_id) {

    //assert arguments
    ifb_macro_assert(stack);
    ifb_macro_assert(manager_id);

    //get the memory manager
    IFBMemoryManager* memory_manager_internal = (IFBMemoryManager*)ifb_memory::stack_get_pointer(
        stack,
        manager_id);

    //assert memory manager
    ifb_macro_assert(memory_manager_internal);
    ifb_macro_assert(memory_manager_internal->reserved_memory_start);
    ifb_macro_assert(memory_manager_internal->count_arenas);
    ifb_macro_assert(memory_manager_internal->size_arena);
    ifb_macro_assert(memory_manager_internal->size_page);
    ifb_macro_assert(memory_manager_internal->size_granularity);
    ifb_macro_assert(memory_manager_internal->offset_arena_array_start);
    ifb_macro_assert(memory_manager_internal->offset_arena_array_position);

    //we're done
    return(memory_manager_internal);
}

inline IFBAddr*
ifb_memory::manager_load_array_arena_start(
    IFBMemoryManager* memory_manager) {

    const IFBU32  offset  = memory_manager->offset_arena_array_start;
    const IFBAddr start   = (IFBAddr)memory_manager;
    const IFBAddr address = start + offset;
    IFBAddr*      pointer = (IFBAddr*)address;
    
    return(pointer); 
}

inline IFBU32* 
ifb_memory::manager_load_array_arena_position(
    IFBMemoryManager* memory_manager) {

    const IFBU32  offset  = memory_manager->offset_arena_array_position;
    const IFBAddr start   = (IFBAddr)memory_manager;
    const IFBAddr address = start + offset;
    IFBAddr*      pointer = (IFBAddr*)address;
}

inline IFBVoid
ifb_memory::manager_load_arrays(
    IFBMemoryManager* memory_manager,
    IFBMemoryManagerArrays*   arrays) {

    ifb_macro_assert(memory_manager);
    ifb_macro_assert(arrays);

    const IFBAddr start    = (IFBAddr)memory_manager;
    arrays->arena_start    = (IFBAddr*)(start + memory_manager->offset_arena_array_start);
    arrays->arena_position =  (IFBU32*)(start + memory_manager->offset_arena_array_position);
}
