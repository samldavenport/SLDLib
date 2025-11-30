#ifndef SLD_OS_SYSTEM_HPP
#define SLD_OS_SYSTEM_HPP

#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct os_system_cpu_info;
    struct os_system_cpu_cache_info;
    struct os_system_memory_info;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    SLD_API_OS void         os_system_get_cpu_info          (os_system_cpu_info&       cpu_info);
    SLD_API_OS void         os_system_get_cpu_cache_info    (os_system_cpu_cache_info& cpu_cache_info);
    SLD_API_OS void         os_system_get_memory_info       (os_system_memory_info&    memory_info);
    SLD_API_OS const u64    os_system_time_ms               (void);
    SLD_API_OS void         os_system_sleep                 (const u32 ms);
    SLD_API_OS void         os_system_debug_print           (const cchar* debug_string);
    SLD_API_OS const cchar* os_system_get_working_directory (void);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct os_system_cpu_cache_info {
        u32 level;
        u32 total_size;
        u32 line_size;
    };

    struct os_system_cpu_info {
        u32 parent_core_number;
        u32 speed_mhz;
        u32 core_count_physical;
        u32 core_count_logical;
        u32 cache_levels;
    };

    struct os_system_memory_info {
        u32 page_size;
        u32 allocation_granularity;
        u32 installed_ram_size_kb;
    };
};

#endif //SLD_OS_SYSTEM_HPP