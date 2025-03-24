#ifndef IFB_MEMORY_HPP
#define IFB_MEMORY_HPP

#include <ifb.hpp>
#include <ifb-platform.hpp>

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

typedef IFBU32  IFBMEM32Handle;
typedef IFBAddr IFBMEM64Handle;

struct IFBMEM64Stack   { IFBMEM64Handle h64; };
struct IFBMEM32Manager { IFBMEM32Handle h32; };
struct IFBMEM32Arena   { IFBMEM32Handle h32; };

#define IFB_MEMORY_INVALID_HANDLE_32 0xFFFFFFFF
#define IFB_MEMORY_INVALID_HANDLE_64 0

/**********************************************************************************/
/* STACK                                                                          */
/**********************************************************************************/

namespace ifb_memory {

    const IFBMEM64Stack
    stack_create(
        const IFBByte* stack_memory,
        const IFBU32   stack_size);

    const IFBU32 stack_push_bytes_relative (const IFBMEM64Stack stack_handle, const IFBU32 size, const IFBU32 alignment = 0);
    const IFBPtr stack_push_bytes_absolute (const IFBMEM64Stack stack_handle, const IFBU32 size, const IFBU32 alignment = 0);
    const IFBB8  stack_pull_bytes          (const IFBMEM64Stack stack_handle, const IFBU32 size, const IFBU32 alignment = 0);
    const IFBPtr stack_get_pointer         (const IFBMEM64Stack stack_handle, const IFBU32 offset);
};

/**********************************************************************************/
/* MEMORY MANAGER                                                                 */
/**********************************************************************************/

struct IFBMemoryManagerContext {
    IFBMEM64Stack   handle_stack;
    IFBMEM32Manager handle_manager;
};

namespace ifb_memory {

    const IFBB8 manager_create  (IFBMemoryManagerContext& manager_context, const IFBU64 size_reservation,const IFBU32 size_arena);
    const IFBB8 manager_destroy (IFBMemoryManagerContext& manager_context);
};

/**********************************************************************************/
/* ARENA                                                                          */
/**********************************************************************************/

struct IFBMemoryArenaContext {
    IFBMEM64Stack   handle_stack;
    IFBMEM32Manager handle_manager;
    IFBMEM32Arena   handle_arena;
};

namespace ifb_memory {

    //commit/decommit
    const IFBB8 arena_commit               (IFBMemoryArenaContext& arena_context);
    const IFBB8 arena_decommit             (IFBMemoryArenaContext& arena_context);

    //operations
    const IFBB8  arena_reset               (IFBMemoryArenaContext& arena_context);
    const IFBU32 arena_push_bytes_relative (IFBMemoryArenaContext& arena_context, const IFBU32 size);
    const IFBPtr arena_push_bytes_absolute (IFBMemoryArenaContext& arena_context, const IFBU32 size);
    const IFBB8  arena_pull_bytes          (IFBMemoryArenaContext& arena_context, const IFBU32 size);
    const IFBPtr arena_get_pointer         (IFBMemoryArenaContext& arena_context, const IFBU32 offset);
};

/**********************************************************************************/
/* UTILITIES                                                                      */
/**********************************************************************************/

namespace ifb_memory {

    const IFBPtr get_pointer(const IFBAddr start, const IFBU32 offset);
};

inline const IFBPtr
ifb_memory::get_pointer(
    const IFBAddr start,
    const IFBU32  offset) {

    const IFBAddr address = start + offset;
    const IFBPtr  pointer = (IFBPtr)address;

    return(pointer);
}

/**********************************************************************************/
/* MACROS                                                                         */
/**********************************************************************************/

#define ifb_memory_macro_is_handle_valid_stack(stack)       (stack.h64 != IFB_MEMORY_INVALID_HANDLE_64)
#define ifb_memory_macro_is_handle_valid_manager(manager) (manager.h32 != IFB_MEMORY_INVALID_HANDLE_32 && manager.h32 != 0)
#define ifb_memory_macro_is_handle_valid_arena(arena)       (arena.h32 != IFB_MEMORY_INVALID_HANDLE_32)


#endif //IFB_MEMORY_HPP
