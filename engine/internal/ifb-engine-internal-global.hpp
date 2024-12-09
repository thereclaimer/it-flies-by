#ifndef IFB_ENGINE_INTERNAL_GLOBAL_HPP
#define IFB_ENGINE_INTERNAL_GLOBAL_HPP

#include "ifb-engine.hpp"
#include "ifb-engine-internal-memory.hpp"


/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

struct IFBEngineContext;

struct IFBEngineGlobalHandle {
    ifb_u32 stack_position;
};

struct IFBEngineGlobals;
struct IFBEngineGlobalStack;

/**********************************************************************************/
/* GLOBAL STACK                                                                   */
/**********************************************************************************/

#ifndef IFB_ENGINE_CONFIG_GLOBAL_STACK_SIZE
    #define IFB_ENGINE_CONFIG_GLOBAL_STACK_SIZE ifb_macro_size_kilobytes(64)
#endif

struct IFBEngineGlobalStack {
    ifb_u32  size;
    ifb_u32  position;
    ifb_byte memory[IFB_ENGINE_CONFIG_GLOBAL_STACK_SIZE];
};

namespace ifb_engine {

    const ifb_void global_stack_push                   (IFBEngineGlobalHandle& ref_global_handle, const ifb_u32 size);
    const ifb_void global_stack_push_aligned           (IFBEngineGlobalHandle& ref_global_handle, const ifb_u32 size, const ifb_u32 alignment);
    const ifb_ptr  global_stack_push_immediate         (const ifb_u32 size);
    const ifb_ptr  global_stack_push_aligned_immediate (const ifb_u32 size, const ifb_u32 alignment);
    const ifb_ptr  global_stack_get_pointer            (const IFBEngineGlobalHandle& ref_global_handle);
};

#define ifb_engine_global_stack_push_type(handle,type)                  ifb_engine::global_stack_push_aligned(sizeof(type),alignof(type)) 
#define ifb_engine_global_stack_push_type_immediate(type)        (type*)ifb_engine::global_stack_push_aligned(sizeof(type),alignof(type)) 
#define ifb_engine_global_stack_push_array(handle,type,count)           ifb_engine::global_stack_push(sizeof(type) * count) 
#define ifb_engine_global_stack_push_array_immediate(type,count) (type*)ifb_engine::global_stack_push(sizeof(type) * count) 
#define ifb_engine_global_stack_get_pointer_type(handle,type)    (type*)ifb_engine::global_stack_get_pointer(handle) 

/**********************************************************************************/
/* GLOBAL VARIABLES                                                               */
/**********************************************************************************/

struct IFBEngineGlobals {
    struct {
        IFBEngineGlobalHandle context; // IFBEngineContext
        IFBEngineGlobalHandle memory;  // IFBEngineMemory
    } handles;
    IFBEngineGlobalStack stack;
    IFBEngineConfig      config;
};

n_initialize (ifb_void);
    
          IFBEngineGlobalStack& global_stack_ref   (ifb_void);
    const IFBEngineConfig&      global_config_ref  (ifb_void);
          IFBEngineContext*     global_context_ptr (ifb_void);
          IFBEngineMemory*      global_memory_ptr  (ifb_void);     
};

#endif //IFB_ENGINE_INTERNAL_GLOBAL_HPP