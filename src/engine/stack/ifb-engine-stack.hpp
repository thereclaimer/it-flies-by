#ifndef IFB_ENGINE_STACK_HPP
#define IFB_ENGINE_STACK_HPP

#include "ifb.hpp"

struct IFBEngineStack {
    memory start;
    u64    size;
    u64    position;
};

namespace ifb_engine {

    external void   stack_create   (IFBEngineStack& stack, memory start, u64 size);
    external void   stack_push     (IFBEngineStack& stack, u64 size);
    external void   stack_pull     (IFBEngineStack& stack, u64 size);
    external bool   stack_can_push (IFBEngineStack& stack, u64 size);
    external bool   stack_can_pull (IFBEngineStack& stack, u64 size);
    external memory stack_pointer  (IFBEngineStack& stack);
    external void   stack_clear    (IFBEngineStack& stack);
};

#endif //IFB_ENGINE_STACK_HPP