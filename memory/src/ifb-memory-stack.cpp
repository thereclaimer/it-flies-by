#pragma once

#include "ifb-memory.hpp"
#include "ifb-memory-internal.hpp"

/**********************************************************************************/
/* PUSH ABSOLUTE                                                                  */
/**********************************************************************************/

ifb_internal inline const IFBMemoryHandle
ifb_memory::stack_push(
    const ifb_u32 size,
    const ifb_u32 alignment) {

    //create the handle
    IFBMemoryHandle handle;
    handle.stack_position = IFB_MEMORY_HANDLE_INVALID;

    //cache the stack
    IFBMemoryStack& stack_ref = ifb_memory::context_get_stack();

    //align the size
    const ifb_u32 size_aligned = ifb_macro_align_a_to_b(size,alignment);

    //make sure we can fit the push
    const ifb_u32 new_position = stack_ref.position + size_aligned; 
    if (new_position >= stack_ref.size) return(handle);

    //update the handle
    handle.stack_position = stack_ref.position;

    //update the stack position
    stack_ref.position = new_position;

    //we're done
    return(handle);
}

/**********************************************************************************/
/* POINTERS                                                                       */
/**********************************************************************************/

ifb_internal inline const ifb_ptr
ifb_memory::stack_get_pointer(
    const IFBMemoryHandle memory_handle) {

    //cache the stack
    IFBMemoryStack& stack_ref = ifb_memory::context_get_stack();

    //make sure the offset is valid
    ifb_b8 offset_valid = true;                          // the offset is valid                      IF...
    offset_valid &= offset >= IFB_MEMORY_HANDLE_MINIMUM; // the offset is ahead of the memory struct AND...
    offset_valid &= offset <  stack_ref.position;        // the offset is before the current stack position

    //calculate the address
    const ifb_address address = offset_valid
        ? stack_ref.start + memory_handle.stack_position;
        : 0;

    //get the pointer
    const ifb_ptr pointer = (ifb_ptr)address;

    //we're done
    return(pointer);
}

/**********************************************************************************/
/* INFO                                                                           */
/**********************************************************************************/

ifb_internal inline const ifb_b8
ifb_memory::stack_get_info(
    IFBMemoryStackInfo* stack_info_ptr) {

    //get the stack
    IFBMemoryStack& stack_ref = ifb_memory::context_get_stack();

    //sanity check
    if (!stack_info_ptr) return(false);

    //set the stack info
    stack_info_ptr->size_total = stack_ref.size;
    stack_info_ptr->size_used  = stack_ref.position;

    //we're done
    return(true);
}
