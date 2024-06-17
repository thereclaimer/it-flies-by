#ifndef ITFLIESBY_PLATFORM_API_HPP
#define ITFLIESBY_PLATFORM_API_HPP

#include "itfliesby-types.hpp"

typedef memory 
(*func_itfliesby_platform_memory_allocate)(
    u64 size);

typedef void 
(*func_itfliesby_platform_memory_free)(
    handle, 
    u64);

typedef  u64 
(*func_itfliesby_platform_file_size)(
    handle file_path,
    b8     terminate);

typedef handle
(*func_itfliesby_platform_file_open)(   
    const char* file_path,
    b8  create_new);

typedef void
(*func_itfliesby_platform_file_close)(
    handle file_handle);

typedef void
(*func_itfliesby_platform_file_write)(
    handle file_handle,
    u64    offset,
    u64    allocated_buffer_size,
    memory allocated_buffer);

typedef void
(*func_itfliesby_platform_file_read)(
    handle file_handle,
    u64    offset,
    u64    allocated_buffer_size,
    memory allocated_buffer,
    b8     terminate);

typedef u64
(*func_itfliesby_platform_ticks)(
    void);

typedef f64
(*func_itfliesby_platform_delta_time_ms)(
    u64 ticks_before,
    u64 ticks_after);

typedef void
(*func_itfliesby_platform_sleep)(
    u64 time_ms);

struct ItfliesbyPlatformApi  {
    func_itfliesby_platform_file_size        file_size;
    func_itfliesby_platform_file_read        file_read;
    func_itfliesby_platform_file_write       file_write;
    func_itfliesby_platform_file_open        file_open;
    func_itfliesby_platform_file_close       file_close;
    func_itfliesby_platform_memory_allocate  memory_allocate;
    func_itfliesby_platform_memory_free      memory_free;
    func_itfliesby_platform_ticks            ticks;
    func_itfliesby_platform_delta_time_ms    delta_time_ms;    
    func_itfliesby_platform_sleep            sleep;
};

#endif //ITFLIESBY_PLATFORM_API_HPP