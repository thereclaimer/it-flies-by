#ifndef IFB_PLATFORM_SYSTEM_HPP
#define IFB_PLATFORM_SYSTEM_HPP

using namespace ifb;

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

namespace ifb::platform {

    struct system_info_memory_t;
    struct system_info_cpu_cache_t;
    struct system_info_cpu_t;
};

/**********************************************************************************/
/* SYSTEM API                                                                     */
/**********************************************************************************/

namespace ifb::platform {

    struct system_info_cpu_cache_t {
        u32 size_total;
        u32 size_line;
    };

    struct system_info_cpu_t {
        u32                     parent_core_number;
        u32                     speed_mhz;
        u32                     core_count_physical;
        u32                     core_count_logical;
        system_info_cpu_cache_t cache_l1;
        system_info_cpu_cache_t cache_l2;
        system_info_cpu_cache_t cache_l3;
    };

    struct system_info_memory_t {
        u32 page_size;
        u32 allocation_granularity;
    };

    typedef const b8 (*api_system_get_info_cpu_f)    (system_info_cpu_t*    cpu_info);
    typedef const b8 (*api_system_get_info_memory_f) (system_info_memory_t* memory_info);
    typedef const ms (*api_system_time_ms_f)         (void);
    typedef void     (*api_system_sleep_f)           (const u32   ms);
    typedef void     (*api_system_debug_print_f)     (const utf8* debug_string);

};


#endif //IFB_PLATFORM_SYSTEM_HPP