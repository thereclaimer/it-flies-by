#pragma once

#include "ifb-engine-stack.hpp"

external void
ifb_engine::stack_create(
    IFBEngineStack& stack,
    memory          start,
    u64             size) {

    stack.start    = start;
    stack.size     = size;
    stack.position = 0;
}

external void
ifb_engine::stack_push(
    IFBEngineStack& stack, 
    u64             size) {

    bool can_push = ifb_engine::stack_can_push(stack,size); 

    if (!can_push) {
        return;
    }

    stack.position += size;
}

external void
ifb_engine::stack_pull(
    IFBEngineStack& stack,
    u64             size) {

    bool can_pull = ifb_engine::stack_can_pull(stack,size);

    if (!can_pull) {
        return;
    }

    stack.position -= size;
}

external bool
ifb_engine::stack_can_push(
    IFBEngineStack& stack,
    u64             size) {

    bool can_push = stack.position + size <= stack.size;

    return(can_push);
}

external bool
ifb_engine::stack_can_pull(
    IFBEngineStack& stack,
    u64             size) {

    bool can_pull = size <= stack.position;

    return(can_pull);
}

external memory
ifb_engine::stack_pointer(
    IFBEngineStack& stack) {

    memory stack_pointer = stack.start + stack.position;

    return(stack_pointer);
}

external void
stack_clear(
    IFBEngineStack& stack) {

    stack.position = 0;
}