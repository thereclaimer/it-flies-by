#ifndef IFB_ENGINE_ALLOCATORS_HPP
#define IFB_ENGINE_ALLOCATORS_HPP

#include <ifb-common.hpp>

#include "ifb-engine-memory.hpp"

/**********************************************************************************/
/* LINEAR ALLOCATOR                                                               */
/**********************************************************************************/

namespace ifb_engine {

    ifb_api const IFBEngineLinearAllocatorHandle  
    linear_allocator_create(
        const ifb_cstr linear_allocator_tag_cstr,
        const ifb_u32  linear_allocator_size_minimum);
    
    ifb_api const IFBEngineMemoryHandle linear_allocator_reserve             (const IFBEngineLinearAllocatorHandle linear_allocator_handle, const ifb_u32 size);
    ifb_api const IFBEngineMemoryHandle linear_allocator_release             (const IFBEngineLinearAllocatorHandle linear_allocator_handle, const ifb_u32 size);
    ifb_api const ifb_b8                linear_allocator_save_point_set      (const IFBEngineLinearAllocatorHandle linear_allocator_handle);
    ifb_api const ifb_b8                linear_allocator_save_point_clear    (const IFBEngineLinearAllocatorHandle linear_allocator_handle);
    ifb_api const ifb_b8                linear_allocator_reset               (const IFBEngineLinearAllocatorHandle linear_allocator_handle);
    ifb_api const ifb_b8                linear_allocator_reset_to_save_point (const IFBEngineLinearAllocatorHandle linear_allocator_handle);
};

/**********************************************************************************/
/* BLOCK ALLOCATOR                                                                */
/**********************************************************************************/

struct IFBEngineBlockAllocatorHandle { IFBEngineMemoryHandle memory; };
struct IFBEngineBlockMemoryHandle    { IFBEngineMemoryHandle memory; };

struct IFBEngineBlockAllocator {
    IFBEngineArenaId          arena_id;
    ifb_u32                   block_count;
    ifb_u32                   block_size;
    IFBEngineMemoryHandle     block_array_handle;
};

namespace ifb_engine {

    ifb_api const ifb_b8 
    block_allocator_create(
        const ifb_cstr                        in_block_allocator_tag,
        const ifb_u32                         in_block_size,
        const ifb_u32                         in_block_count,
              IFBEngineBlockAllocatorHandle& out_block_allocator_handle_ref);

    ifb_api const ifb_b8 block_allocator_reserve (const IFBEngineBlockAllocatorHandle in_block_allocator_handle, IFBEngineBlockMemoryHandle& out_block_ref);
    ifb_api const ifb_b8 block_allocator_release (const IFBEngineBlockAllocatorHandle in_block_allocator_handle, IFBEngineBlockMemoryHandle& out_block_ref);
    ifb_api const ifb_b8 block_allocator_reset   (const IFBEngineBlockAllocatorHandle in_block_allocator_handle);
};

/**********************************************************************************/
/* TABLE ALLOCATOR                                                                */
/**********************************************************************************/

struct IFBEngineTableAllocatorHandle { IFBEngineMemoryHandle memory; };
struct IFBEngineTableMemoryHandle    { IFBEngineMemoryHandle memory; };

struct IFBEngineTableAllocator {
    IFBEngineArenaId arena_id;
    ifb_u32          position;
};

namespace ifb_engine {

    ifb_api const IFBEngineTableAllocatorHandle
    table_allocator_create(
        const ifb_cstr table_allocator_tag_cstr,
        const ifb_u32  table_allocator_size_minimum);

    ifb_api const IFBEngineTableMemoryHandle*
    table_allocator_reserve(
        const IFBEngineTableAllocatorHandle table_allocator_handle,
        const ifb_u32                       table_count_rows,
        const ifb_u32                       table_count_columns,
        const ifb_u32                       table_size_header,
        const ifb_u32*                      table_size_columns_ptr);

    ifb_api const ifb_b8
    table_allocator_reset(const IFBEngineTableAllocatorHandle table_allocator_handle);
};

#endif //IFB_ENGINE_ALLOCATORS_HPP