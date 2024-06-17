#ifndef IFB_ENGINE_PLATFORM_HPP
#define IFB_ENGINE_PLATFORM_HPP

#include <ifb.hpp>

global IFBPlatformApi ifb_platform_api;

#define ifb_engine_platform_file_size         ifb_platform_api.file_size
#define ifb_engine_platform_file_read         ifb_platform_api.file_read
#define ifb_engine_platform_file_write        ifb_platform_api.file_write
#define ifb_engine_platform_file_open         ifb_platform_api.file_open
#define ifb_engine_platform_file_close        ifb_platform_api.file_close
#define ifb_engine_platform_memory_allocate   ifb_platform_api.memory_allocate
#define ifb_engine_platform_memory_free       ifb_platform_api.memory_free
#define ifb_engine_platform_ticks             ifb_platform_api.ticks
#define ifb_engine_platform_delta_time_ms     ifb_platform_api.delta_time_ms
#define ifb_engine_platform_sleep             ifb_platform_api.sleep

inline void
ifb_engine_platform_validate(
    IFBPlatformApi platform) {

    ifb_assert(platform.file_size);
    ifb_assert(platform.file_read);
    ifb_assert(platform.file_write);
    ifb_assert(platform.file_open);
    ifb_assert(platform.file_close);
    ifb_assert(platform.memory_allocate);
    ifb_assert(platform.memory_free);
    ifb_assert(platform.ticks);
    ifb_assert(platform.delta_time_ms);
    ifb_assert(platform.sleep);

    ifb_platform_api = platform;
}

#endif //IFB_ENGINE_PLATFORM_HPP