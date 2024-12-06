#ifndef IFB_ENGINE_INTERNAL_MEMORY_HPP
#define IFB_ENGINE_INTERNAL_MEMORY_HPP

#include "ifb-engine.hpp"
#include "ifb-engine-internal-global.hpp"

/**********************************************************************************/
/* MEMORY                                                                         */
/**********************************************************************************/

struct IFBEngineMemory {
    ifb_address           reservation_start;
    ifb_u32               system_page_size;
    ifb_u32               system_allocation_granularity;
    ifb_u32               page_count_total;
    ifb_u32               page_count_committed;
    ifb_u32               commit_count_max;
    ifb_u32               commit_count_current;
    IFBEngineGlobalHandle global_handle_commit_array;
};

namespace ifb_engine {

          ifb_void          memory_reserve (IFBEngineMemory* ptr_memory, const ifb_size reservation_size_minimum);
    const IFBMemoryCommitId memory_commit  (IFBEngineMemory* ptr_memory, const ifb_u32  commit_size_minimum);
    
    const ifb_u32 memory_get_commit_size        (const IFBEngineMemory* ptr_memory, const IFBMemoryCommitId commit_id);
    const ifb_u32 memory_get_commit_page_number (const IFBEngineMemory* ptr_memory, const IFBMemoryCommitId commit_id);
    const ifb_u32 memory_get_commit_page_count  (const IFBEngineMemory* ptr_memory, const IFBMemoryCommitId commit_id);
    const ifb_ptr memory_get_pointer            (const IFBEngineMemory* ptr_memory, const IFBMemoryCommitId commit_id);
    const ifb_ptr memory_get_pointer            (const IFBEngineMemory* ptr_memory, const IFBMemoryCommitId commit_id, const ifb_u32 commit_offset);

    const ifb_size memory_align_size_to_page        (const IFBEngineMemory* ptr_memory, const ifb_size size);
    const ifb_size memory_align_size_to_granularity (const IFBEngineMemory* ptr_memory, const ifb_size size);

    const IFBMemory* memory_get_commit_array_pointer(const IFBEngineMemory* ptr_memory);
    const ifb_u32    memory_get_size_committed(const IFBEngineMemory* ptr_memory);
};

#endif //IFB_ENGINE_INTERNAL_MEMORY_HPP