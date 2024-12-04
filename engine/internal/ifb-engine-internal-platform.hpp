#ifndef IFB_ENGINE_INTERNAL_PLATFORM_HPP
#define IFB_ENGINE_INTERNAL_PLATFORM_HPP

#include "ifb-engine.hpp"
#include "ifb-engine-internal-memory.hpp"

struct IFBEnginePlatformSystemInfo {
    ifb_u32 page_size;
    ifb_u32 allocation_granularity;
};

struct IFBEnginePlatformMemoryReservation {
    ifb_address start;
    ifb_u32     size;
};

#define IFB_ENGINE_PLATFORM_WINDOW_TITLE "It Flies By"

struct IFBEnginePlatformWindow {
    ifb_u32    width;
    ifb_u32    height;
    ifb_u32    position_x;
    ifb_u32    position_y;
    ifb_b32    quit_received;
};

struct IFBEnginePlatformMonitorInfo {
    ifb_u32 width;
    ifb_u32 height;
    ifb_u32 refresh_hz;
}; 

struct IFBEnginePlatform {
    struct {
        ifb_handle system_info;
        ifb_handle memory_reservation;
        ifb_handle window;
        ifb_handle monitor;
    } global_handles;
};

namespace ifb_engine {

    inline IFBEnginePlatform*                  platform_global_pointer                    (const ifb_handle handle) { return(ifb_engine_memory_global_pointer_type(handle,IFBEnginePlatform));}
    inline IFBEnginePlatformSystemInfo*        platform_global_pointer_system_info        (const ifb_handle handle) { return(ifb_engine_memory_global_pointer_type(handle,IFBEnginePlatformSystemInfo));}
    inline IFBEnginePlatformMemoryReservation* platform_global_pointer_memory_reservation (const ifb_handle handle) { return(ifb_engine_memory_global_pointer_type(handle,IFBEnginePlatformMemoryReservation));}
    inline IFBEnginePlatformWindow*            platform_global_pointer_window             (const ifb_handle handle) { return(ifb_engine_memory_global_pointer_type(handle,IFBEnginePlatformWindow));}
    inline IFBEnginePlatformMonitorInfo*       platform_global_pointer_monitor_info       (const ifb_handle handle) { return(ifb_engine_memory_global_pointer_type(handle,IFBEnginePlatformMonitorInfo));}
};

//api
namespace ifb_engine {

    //system
    ifb_global funcptr_ifb_platform_system_page_size              platform_system_page_size;
    ifb_global funcptr_ifb_platform_system_allocation_granularity platform_system_allocation_granularity;
    ifb_global funcptr_ifb_platform_system_time_ms                platform_system_time_ms;
    ifb_global funcptr_ifb_platform_system_sleep                  platform_system_sleep;
    
    //window
    ifb_global funcptr_ifb_platform_window_create           platform_window_create;
    ifb_global funcptr_ifb_platform_window_destroy          platform_window_destroy;
    ifb_global funcptr_ifb_platform_window_frame_start      platform_window_frame_start;
    ifb_global funcptr_ifb_platform_window_frame_render     platform_window_frame_render;
    ifb_global funcptr_ifb_platform_window_show             platform_window_show;
    ifb_global funcptr_ifb_platform_window_opengl_init      platform_window_opengl_init;
    ifb_global funcptr_ifb_platform_window_imgui_init       platform_window_imgui_init;

    //monitor
    ifb_global funcptr_ifb_platform_monitor_info            platform_monitor_info;

    //memory
    ifb_global funcptr_ifb_platform_memory_reserve          platform_memory_reserve;
    ifb_global funcptr_ifb_platform_memory_release          platform_memory_release;
    ifb_global funcptr_ifb_platform_memory_commit           platform_memory_commit;
};


namespace ifb_engine {

    const ifb_handle platform_initialize(IFBPlatformApi& platform_api_ref);

    ifb_void platform_get_system_info  (IFBEnginePlatformSystemInfo* system_info_ptr);
    ifb_void platform_get_monitor_info (IFBEnginePlatformMonitorInfo* monitor_info_ptr);

};

#endif //IFB_ENGINE_INTERNAL_PLATFORM_HPP