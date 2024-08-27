#ifndef IFB_ENGINE_PLATFORM_HPP
#define IFB_ENGINE_PLATFORM_HPP

#include <ifb.hpp>

global IFBPlatformApi ifb_platform_api;

#define ifb_engine_platform_file_size                      ifb_platform_api.file_size
#define ifb_engine_platform_file_read                      ifb_platform_api.file_read
#define ifb_engine_platform_file_write                     ifb_platform_api.file_write
#define ifb_engine_platform_file_open                      ifb_platform_api.file_open
#define ifb_engine_platform_file_close                     ifb_platform_api.file_close
#define ifb_engine_platform_memory_reserve_small_pages     ifb_platform_api.memory_reserve_small_pages
#define ifb_engine_platform_memory_reserve_large_pages     ifb_platform_api.memory_reserve_large_pages
#define ifb_engine_platform_memory_reserve                 ifb_platform_api.memory_reserve
#define ifb_engine_platform_memory_release                 ifb_platform_api.memory_release
#define ifb_engine_platform_memory_commit                  ifb_platform_api.memory_commit
#define ifb_engine_platform_memory_decommit                ifb_platform_api.memory_decommit
#define ifb_engine_platform_memory_page_size_small         ifb_platform_api.memory_page_size_small
#define ifb_engine_platform_memory_page_size_large         ifb_platform_api.memory_page_size_large
#define ifb_engine_platform_memory_allocation_granularity  ifb_platform_api.memory_allocation_granularity
#define ifb_engine_platform_ticks                          ifb_platform_api.ticks
#define ifb_engine_platform_delta_time_ms                  ifb_platform_api.delta_time_ms
#define ifb_engine_platform_sleep                          ifb_platform_api.sleep
#define ifb_engine_platform_process_id                     ifb_platform_api.process_id
#define ifb_engine_platform_thread_id                      ifb_platform_api.thread_id

inline void
ifb_engine_platform_validate(
    IFBPlatformApi platform) {

    ifb_assert(platform.file_size);
    ifb_assert(platform.file_read);
    ifb_assert(platform.file_write);
    ifb_assert(platform.file_open);
    ifb_assert(platform.file_close);
    ifb_assert(platform.memory_reserve_small_pages);
    ifb_assert(platform.memory_reserve_large_pages);
    ifb_assert(platform.memory_reserve);
    ifb_assert(platform.memory_release);
    ifb_assert(platform.memory_commit);
    ifb_assert(platform.memory_decommit);
    ifb_assert(platform.memory_page_size_small);
    ifb_assert(platform.memory_page_size_large);
    ifb_assert(platform.memory_allocation_granularity);
    ifb_assert(platform.ticks);
    ifb_assert(platform.delta_time_ms);
    ifb_assert(platform.sleep);
    ifb_assert(platform.process_id);
    ifb_assert(platform.thread_id);

    ifb_platform_api = platform;
}

#endif //IFB_ENGINE_PLATFORM_HPP