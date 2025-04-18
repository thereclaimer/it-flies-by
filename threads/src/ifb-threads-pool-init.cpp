#pragma once

#include "ifb-threads.hpp"

#define IFB_THREAD_POOL_INIT_MAX_SIZE 0xFFFF

using namespace ifb;

namespace ifb::threads {

    void pool_init_step_0_validate_args  (thread_pool_init_t* init, b8& result_ref);
    void pool_init_step_1_set_size_cache (thread_pool_init_t* init, b8& result_ref);
    void pool_init_step_2_cast_memory    (thread_pool_init_t* init, b8& result_ref);
    void pool_init_step_3_set_header     (thread_pool_init_t* init, b8& result_ref);
    void pool_init_step_4_set_handles    (thread_pool_init_t* init, b8& result_ref);
    void pool_init_step_5_validate_pool  (thread_pool_init_t* init, b8& result_ref);
};

inline void 
threads::pool_init_step_0_validate_args(
    thread_pool_init_t* init,
    b8&                 result_ref) {

    //check init struct
    result_ref = (init != NULL);
    if (!result_ref) return;

    //check pointers
    result_ref &= (init->args        != NULL);
    result_ref &= (init->sizes_cache != NULL);
    result_ref &= (init->pointer     != NULL);    
    if (!result_ref) return;

    //check remaining args
    const thread_pool_args_t* args = init->args;
    result_ref &= (args->thread_pool_memory.start  != 0);
    result_ref &= (args->thread_pool_memory.size   != 0);
    result_ref &= (args->thread_count              != 0);
    result_ref &= (args->stride_task_data          != 0);
    result_ref &= (args->stride_thread_description != 0);    
}

inline void
threads::pool_init_step_1_set_size_cache(
    thread_pool_init_t* init,
    b8&             result_ref) {

    if (!result_ref) return;

    //store arg constants
    thread_pool_args_t* args                           = init->args;
    const u32       arg_thread_count               = args->thread_count;
    const u32       arg_thread_stride_description  = args->stride_thread_description;
    const u32       arg_thread_stride_task_data    = args->stride_task_data;

    //get the cache
    thread_pool_sizes_cache_t* sizes_cache = init->sizes_cache;

    //temporary values
    const u32 tmp_size_max               = IFB_THREAD_POOL_INIT_MAX_SIZE;
    const u32 tmp_size_thread_handle     = sizeof(thread_h);
    const u32 tmp_size_task_function     = sizeof(thread_task_callback_f);
    const u32 tmp_size_affinity_mask     = sizeof(thread_affinity_mask_e);
    const u32 tmp_size_platform_instance = platform::thread_size();

    //thread pool properties
    const u32 pool_size_struct                     = ifb_macro_align_size_struct(thread_pool_t); 
    const u32 pool_size_platform_total             = arg_thread_count * tmp_size_platform_instance;
    const u32 pool_size_description_buffer         = arg_thread_count * arg_thread_stride_description;
    const u32 pool_size_task_data                  = arg_thread_count * arg_thread_stride_task_data;
    const u32 pool_size_task_functions             = arg_thread_count * tmp_size_task_function;
    const u32 pool_size_array_affinity_mask        = arg_thread_count * tmp_size_affinity_mask;
    const u32 pool_size_array_list_running_threads = array_list::memory_size(tmp_size_thread_handle,arg_thread_count);

    //thread pool total size
    const u32 pool_size_total = 
        pool_size_struct              +
        pool_size_platform_total      +
        pool_size_description_buffer  +
        pool_size_task_data           +
        pool_size_task_functions      +
        pool_size_array_affinity_mask +
        pool_size_array_list_running_threads;

    //make sure we fit within max allocation size
    result_ref &= (pool_size_total < tmp_size_max);
    
    //non-essential cache properties
    sizes_cache->other.max                         = tmp_size_max;
    sizes_cache->other.thread_handle               = tmp_size_thread_handle;
    sizes_cache->other.task_function               = tmp_size_task_function;
    sizes_cache->other.affinity_mask               = tmp_size_affinity_mask;
    sizes_cache->other.platform_instance           = tmp_size_platform_instance;

    //memory size properties
    sizes_cache->memory.total                      = *((ifb::u16*)&pool_size_total);
    sizes_cache->memory.pool_struct_size           = *((ifb::u16*)&pool_size_struct);
    sizes_cache->memory.platform_total             = *((ifb::u16*)&pool_size_platform_total);
    sizes_cache->memory.description_buffer         = *((ifb::u16*)&pool_size_description_buffer);
    sizes_cache->memory.task_data                  = *((ifb::u16*)&pool_size_task_data);
    sizes_cache->memory.thread_functions           = *((ifb::u16*)&pool_size_task_functions);
    sizes_cache->memory.array_list_running_threads = *((ifb::u16*)&pool_size_array_list_running_threads);
    sizes_cache->memory.array_affinity_mask        = *((ifb::u16*)&pool_size_array_affinity_mask);
}

inline void
threads::pool_init_step_2_cast_memory(
    thread_pool_init_t* init,
    b8&             result_ref) {

    if (!result_ref) return;

    //get the memory args
    thread_pool_args_t* args = init->args;
    const ifb::addr args_mem_start = args->thread_pool_memory.start;
    const ifb::u64  args_mem_size  = args->thread_pool_memory.size;

    //make sure its what we expect
    result_ref &= args_mem_start != NULL;
    result_ref &= args_mem_size  >= init->sizes_cache->memory.total;
    if (!result_ref) return;

    //cast the memory
    init->pointer = (thread_pool_t*)args_mem_start;
}

inline void
threads::pool_init_step_3_set_header(
    thread_pool_init_t* init,
    b8&             result_ref) {

    if (!result_ref) return;

    //get the pointers
    thread_pool_args_t*        args  = init->args;
    thread_pool_t*             pool  = init->pointer;
    thread_pool_sizes_cache_t* sizes = init->sizes_cache;

    //initialize the header
    thread_pool_header_t& header_ref  = pool->header;
    header_ref.memory.start           = args->thread_pool_memory.start;
    header_ref.memory.size            = args->thread_pool_memory.size;
    header_ref.thread_count           = args->thread_count;
    header_ref.stride_description     = args->stride_thread_description;
    header_ref.stride_memory_platform = sizes->other.platform_instance;
    header_ref.stride_memory_task     = args->stride_task_data;
}

inline void 
threads::pool_init_step_4_set_handles(
    thread_pool_init_t* init,
    b8&             result_ref) {
        
    if (!result_ref) return;

    //get the pointers
    thread_pool_args_t*       args  = init->args;
    thread_pool_sizes_cache_t* sizes = init->sizes_cache;
    thread_pool_t*           pool  = init->pointer;

    //calculate offsets
    const ifb::u16 offset_description_buffer         = sizes->memory.pool_struct_size;
    const ifb::u16 offset_memory_platform            = sizes->memory.platform_total             + offset_description_buffer;
    const ifb::u16 offset_memory_task                = sizes->memory.description_buffer         + offset_memory_platform;
    const ifb::u16 offset_array_task_functions       = sizes->memory.task_data                  + offset_memory_task;
    const ifb::u16 offset_array_list_running_threads = sizes->memory.thread_functions           + offset_array_task_functions;
    const ifb::u16 offset_array_affinity_mask        = sizes->memory.array_list_running_threads + offset_array_list_running_threads;

    //set handles
    thread_pool_offsets_t& offsets = pool->offsets;
    offsets.description_buffer         = offset_description_buffer;
    offsets.memory_platform            = offset_memory_platform;
    offsets.memory_task                = offset_memory_task;
    offsets.array_task_functions       = offset_array_task_functions;
    offsets.array_list_running_threads = offset_array_list_running_threads;
    offsets.array_affinity_mask        = offset_array_affinity_mask;
}

inline void 
threads::pool_init_step_5_validate_pool(
    thread_pool_init_t* init,
    b8&             result_ref) {

    if (!result_ref) return;

    //get the pointers
    thread_pool_args_t*       args  = init->args;
    thread_pool_sizes_cache_t* sizes = init->sizes_cache;
    thread_pool_t*           pool  = init->pointer;

    //validate header
    result_ref &= (pool->header.memory.start           == args->thread_pool_memory.start);
    result_ref &= (pool->header.memory.size            == sizes->memory.total);
    result_ref &= (pool->header.thread_count           == args->thread_count);
    result_ref &= (pool->header.stride_description     == args->stride_thread_description);
    result_ref &= (pool->header.stride_memory_platform == sizes->other.platform_instance);
    result_ref &= (pool->header.stride_memory_task     == sizes->other.task_function); 

    //validate handles
    result_ref &= (pool->offsets.description_buffer         != 0);
    result_ref &= (pool->offsets.memory_platform            != 0);
    result_ref &= (pool->offsets.memory_task                != 0);
    result_ref &= (pool->offsets.array_task_functions       != 0);
    result_ref &= (pool->offsets.array_list_running_threads != 0);
    result_ref &= (pool->offsets.array_affinity_mask        != 0);
}
