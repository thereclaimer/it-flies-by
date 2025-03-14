#ifndef IFB_THREADING_HPP
#define IFB_THREADING_HPP

#include <ifb.hpp>
#include <ifb-memory.hpp>
#include <ifb-data-structures.hpp>

/**********************************************************************************/
/* THREAD POOL                                                                    */
/**********************************************************************************/

namespace ifb_thread_pool {

    //memory
    const IFBB8            validate          (IFBThreadPool*  thread_pool);
    IFBThreadPool*         load              (IFBMemoryArena* arena, const IFBHNDThreadPool thread_pool_handle);
    IFBThreadPool*         commit_absolute   (IFBMemoryArena* arena, const IFBU32 thread_count, const IFBU32 thread_description_stride, const IFBU32 thread_platform_data_size);
    const IFBHNDThreadPool commit_relative   (IFBMemoryArena* arena, const IFBU32 thread_count, const IFBU32 thread_description_stride, const IFBU32 thread_platform_data_size);

    //reserve/release
    const IFBB8            threads_reserve   (IFBThreadPool* thread_pool, const IFBU32 thread_count, const IFBChar*      thread_descriptions, const IFBThreadTask* thread_tasks, IFBHNDThread* thread_handles);
    const IFBB8            threads_release   (IFBThreadPool* thread_pool, const IFBU32 thread_count, const IFBHNDThread* thread_handles);
    
    //control
    const IFBB8            threads_start     (IFBThreadPool* thread_pool, const IFBU32 thread_count, const IFBHNDThread* thread_handles);
    const IFBB8            threads_stop      (IFBThreadPool* thread_pool, const IFBU32 thread_count, const IFBHNDThread* thread_handles);
    const IFBB8            threads_sync      (IFBThreadPool* thread_pool, const IFBU32 thread_count, const IFBHNDThread* thread_handles);
    
    //cores    
    const IFBB8            threads_set_cores (IFBThreadPool* thread_pool, const IFBU32 thread_count, const IFBHNDThread* thread_handles, const IFBU64* thread_core_masks);
    const IFBB8            threads_get_cores (IFBThreadPool* thread_pool, const IFBU32 thread_count, const IFBHNDThread* thread_handles,       IFBU64* thread_core_masks);
};


#endif //IFB_THREADING_HPP