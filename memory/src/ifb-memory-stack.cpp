#pragma once

#include "ifb-memory.hpp"
#include "ifb-memory-internal.cpp"

/**********************************************************************************/
/* STACK                                                                          */
/**********************************************************************************/

IFBMEMStack
ifb_memory::stack_create(
    const IFBMemory& stack_memory) {

    //check args
    ifb::b8 can_create = true;                                            // we can create the stack IF...
    can_create &= (stack_memory.start != NULL);                         //...the memory isn't null AND
    can_create &= (stack_memory.size  >= IFB_MEMORY_STRUCT_SIZE_STACK); //...we can fit the struct in the memory
    if (!can_create) return(NULL);                              // if we can't create, we're done

    //cast the memory
    IFBMemoryStack* stack = (IFBMemoryStack*)stack_memory.start;
    
    //initialize the stack
    stack->size     = stack_memory.size;
    stack->position = IFB_MEMORY_STRUCT_SIZE_STACK;

    //we're done
    return(stack);
}

const ifb::u32
ifb_memory::stack_push_bytes_relative(
          IFBMemoryStack* stack,
    const ifb::u32          size) {

    //validate stack
    ifb_memory::validate_stack(stack);

    //calculate the new position
    const ifb::u32 stack_size         = stack->size;
    const ifb::u32 stack_offset       = stack->position;
    const ifb::u32 stack_position_new = stack_offset + size;

    //make sure we can fit the commit
    const ifb::b8 can_commit = (stack_position_new < stack_size); 
    if (!can_commit) return(NULL);

    //update the position
    stack->position = stack_position_new;
    
    //we're done
    return(stack_offset);
}

const ifb::ptr
ifb_memory::stack_push_bytes_absolute_pointer(
          IFBMemoryStack* stack,
    const ifb::u32          size) {

    //validate stack
    ifb_memory::validate_stack(stack);

    //calculate the new position
    const ifb::u32 stack_size         = stack->size;
    const ifb::u32 stack_offset       = stack->position;
    const ifb::u32 stack_position_new = stack_offset + size;

    //make sure we can fit the commit
    const ifb::b8 can_commit = (stack_position_new < stack_size); 
    if (!can_commit) return(NULL);

    //update the position
    stack->position = stack_position_new;
    
    //calculate the pointer
    const ifb::addr stack_result_offset  = (ifb::addr)stack + stack_offset;
    const ifb::ptr  stack_result_pointer = (ifb::ptr)stack_result_offset;

    //we're done
    return(stack_result_pointer);
}

const ifb::addr
ifb_memory::stack_push_bytes_absolute_address(
          IFBMemoryStack* stack,
    const ifb::u32          size) {

    //validate stack
    ifb_memory::validate_stack(stack);

    //calculate the new position
    const ifb::u32 stack_size         = stack->size;
    const ifb::u32 stack_offset       = stack->position;
    const ifb::u32 stack_position_new = stack_offset + size;

    //make sure we can fit the commit
    const ifb::b8 can_commit = (stack_position_new < stack_size); 
    if (!can_commit) return(NULL);

    //update the position
    stack->position = stack_position_new;
    
    //calculate the address
    const ifb::addr stack_result_offset = (ifb::addr)stack + stack_offset;

    //we're done
    return(stack_result_offset);
}

const ifb::ptr
ifb_memory::stack_get_pointer(
          IFBMemoryStack* stack,
    const ifb::u32          offset) {

    //validate stack
    ifb_memory::validate_stack(stack);

    //calculate the pointer
    const ifb::addr stack_offset  = ((ifb::addr)stack) + offset;
    const ifb::ptr  stack_pointer = (stack_offset < stack->size) ? (ifb::ptr)stack_offset : NULL; 
        
    //we're done
    return(stack_pointer);
}

const ifb::b8
ifb_memory::stack_pull_bytes(
          IFBMemoryStack* stack,
    const ifb::u32          size) {

    //validate stack
    ifb_memory::validate_stack(stack);

    //get sizes
    const ifb::u32 position_current = stack->position;
    const ifb::u32 position_dif     = position_current - IFB_MEMORY_STRUCT_SIZE_STACK; 
    const ifb::u32 position_new     = stack->position - size;

    //make sure we can pull
    //there has to be at least enough memory for the stack struct
    ifb::b8 can_pull = (size <= position_dif);
    if (!can_pull) return(false);

    //update the stack position
    stack->position = position_new;

    //we're done
    return(true);
}
