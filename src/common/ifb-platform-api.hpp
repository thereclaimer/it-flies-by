#ifndef IFB_PLATFORM_API_HPP
#define IFB_PLATFORM_API_HPP

#include "ifb-types.hpp"

typedef memory
(*func_ifb_platform_memory_reserve_small_pages)(
    u64 reservation_size);

typedef memory
(*func_ifb_platform_memory_reserve_large_pages)(
    u64 reservation_size);

typedef memory
(*func_ifb_platform_memory_reserve)(
    const memory reservation_start,
    const size_t reservation_size);

typedef void
(*func_ifb_platform_memory_release)(
    memory reserved_memory,
    u64    reserved_size);

typedef memory
(*func_ifb_platform_memory_commit)(
    memory reserved_memory,
    u64    commit_size);

typedef void
(*func_ifb_platform_memory_decommit)(
    memory committed_memory,
    u64    committed_size);

typedef u64
(*func_ifb_platform_memory_page_size_small)(
    void);


typedef u64
(*func_ifb_platform_memory_large_page_size)(
    void);

typedef u64
(*func_ifb_platform_memory_allocation_granularity)(
    void);

typedef  u64 
(*func_ifb_platform_file_size)(
    handle file_path,
    b8     terminate);

typedef handle
(*func_ifb_platform_file_open)(   
    const char* file_path,
          b8    create_new);

typedef void
(*func_ifb_platform_file_close)(
    handle file_handle);

typedef void
(*func_ifb_platform_file_write)(
    handle file_handle,
    u64    offset,
    u64    allocated_buffer_size,
    memory allocated_buffer);

typedef void
(*func_ifb_platform_file_read)(
    handle file_handle,
    u64    offset,
    u64    allocated_buffer_size,
    memory allocated_buffer,
    b8     terminate);

typedef u64
(*func_ifb_platform_ticks)(
    void);

typedef f64
(*func_ifb_platform_delta_time_ms)(
    u64 ticks_before,
    u64 ticks_after);

typedef void
(*func_ifb_platform_sleep)(
    u64 time_ms);

typedef u64
(*func_ifb_platform_process_id)(
    void);

typedef u64
(*func_ifb_platform_thread_id)(
    void);

struct IFBPlatformApi  {
    func_ifb_platform_file_size                     file_size;
    func_ifb_platform_file_read                     file_read;
    func_ifb_platform_file_write                    file_write;
    func_ifb_platform_file_open                     file_open;
    func_ifb_platform_file_close                    file_close;
    func_ifb_platform_memory_reserve_small_pages    memory_reserve_small_pages;
    func_ifb_platform_memory_reserve_large_pages    memory_reserve_large_pages;
    func_ifb_platform_memory_reserve                memory_reserve;
    func_ifb_platform_memory_release                memory_release;
    func_ifb_platform_memory_commit                 memory_commit;
    func_ifb_platform_memory_decommit               memory_decommit;
    func_ifb_platform_memory_page_size_small        memory_page_size_small;
    func_ifb_platform_memory_large_page_size        memory_page_size_large;
    func_ifb_platform_memory_allocation_granularity memory_allocation_granularity;
    func_ifb_platform_ticks                         ticks;
    func_ifb_platform_delta_time_ms                 delta_time_ms;    
    func_ifb_platform_sleep                         sleep;
    func_ifb_platform_process_id                    process_id;
    func_ifb_platform_thread_id                     thread_id;
};

#endif //IFB_PLATFORM_API_HPP