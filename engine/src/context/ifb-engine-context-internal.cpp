#pragma once

#include "ifb-engine.hpp"
#include "ifb-engine-context.hpp"

namespace ifb_engine {

    IFBEngineCore* context_memory_push_core (const IFBEngineContext* engine_context);
    IFBVoid        context_memory_reset     (const IFBEngineContext* engine_context);
};

inline IFBEngineCore*
ifb_engine::context_memory_push_core(
    const IFBEngineContext* engine_context) {

    //push a core struct on the arena
    IFBEngineCore* core = (IFBEngineCore*)ifb_engine::memory_arena_push_bytes_absolute_pointer(
        engine_context->memory,
        IFBEngineArena_Context,
        ifb_engine::_global_core.core_struct_size);

    //read the core data
    ifb_engine::memory_stack_get_core(engine_context->memory,core);

    //we're done
    return(core);
}

inline IFBVoid
ifb_engine::context_memory_reset(
    const IFBEngineContext* engine_context) {

    ifb_engine::memory_arena_reset(
        engine_context->memory,
        IFBEngineArena_Context);
}
