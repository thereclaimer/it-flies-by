#pragma once

#include "ifb-engine-internal.hpp"

#include "ifb-engine-core.cpp"
#include "ifb-engine-memory.cpp"
#include "ifb-engine-algorithms.cpp"
#include "ifb-engine-asset.cpp"
#include "ifb-engine-tag.cpp"
#include "ifb-engine-allocators.cpp"
#include "ifb-engine-rendering.cpp"
#include "ifb-engine-platform.cpp"
#include "ifb-engine-tables.cpp"
#include "ifb-engine-controllers.cpp"

/**********************************************************************************/
/* API                                                                            */
/**********************************************************************************/

ifb_api const ifb_b8
ifb_engine::engine_create_context(
    IFBPlatformApi& platform_api_ref) {

    //allocate context
    _engine_context = ifb_engine_memory_global_push_struct_immediate(IFBEngineContext);

    //initialize the api
    result &= ifb_engine::platform_initialize(platform_api_ref);
    
    //validate the arguments
    ifb_b8 result = true;
    result &= platform_info_ref.reservation_start != NULL;
    result &= platform_info_ref.reservation_size  >= ifb_engine::math_size_gigabytes(4);
    result &= platform_info_ref.page_size         >= sizeof(IFBEngineContext);
    if (!result) {
        return(false);
    }

    //commit the first page for the context
    const ifb_memory context_memory = ifb_engine::platform_memory_pages_commit(platform_info_ref.reservation_start,platform_info_ref.page_size);
    if (context_memory != platform_info_ref.reservation_start) {
        return(false);
    }

    //if that worked, cast the struct
    _engine_context = (IFBEngineContext*)context_memory;     
    
    //set the memory info
    _engine_context->platform.memory_page_size        = platform_info_ref.page_size;
    _engine_context->platform.memory_page_count_total = platform_info_ref.reservation_size / platform_info_ref.page_size;
    _engine_context->platform.memory_page_count_used  = 1;

    //initialize the engine core
    result &= ifb_engine::core_routine_initialize();

    //update state
    _engine_context->state = result
        ? IFBEngineState_NotRunning
        : IFBEngineState_Fatal;          

    //we're done
    return(result);
}

ifb_api const ifb_b8
ifb_engine::engine_startup(
    ifb_void) {

    //set startup state
    _engine_context->state = IFBEngineState_Startup;

    //execute startup routine
    ifb_b8 result = ifb_engine::core_routine_startup();

    //update engine state
    _engine_context->state = (result) 
        ? IFBEngineState_Idle
        : IFBEngineState_Fatal;

    //we're done
    return(result);
}

ifb_api const ifb_b8
ifb_engine::engine_frame_execute(
    ifb_void) {

    ifb_b8 result = true;

    result &= ifb_engine::core_routine_frame_start ();
    result &= ifb_engine::core_routine_frame_render();

    return(result);
}

ifb_api const ifb_b8
ifb_engine::engine_shutdown(
    ifb_void) {

    return(true);
}

ifb_api const ifb_b8
ifb_engine::engine_destroy_context(
    ifb_void) {

    return(true);
}

ifb_api const ifb_memory
ifb_engine::engine_platform_alloc(
    const ifb_u32 size) {

    //get the platform allocator
    const IFBEngineLinearAllocatorHandle platform_allocator = ifb_engine::core_allocator_linear_platform();
    
    //get the memory
    const IFBEngineMemoryHandle memory_handle = ifb_engine::linear_allocator_reserve(platform_allocator,size);
    const ifb_memory memory = ifb_engine::memory_pointer_from_handle(memory_handle);

    //we're done
    return(memory);
}

ifb_api const ifb_memory
ifb_engine::engine_frame_alloc(
    const ifb_u32 size) {

    //get the frame allocator
    const IFBEngineLinearAllocatorHandle frame_allocator = ifb_engine::core_allocator_linear_frame();
    
    //get the memory
    const IFBEngineMemoryHandle memory_handle = ifb_engine::linear_allocator_reserve(frame_allocator,size);
    const ifb_memory memory = ifb_engine::memory_pointer_from_handle(memory_handle);

    //we're done
    return(memory);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/