#ifndef SLD_OS_SYSTEM_HPP
#define SLD_OS_SYSTEM_HPP

#include "sld.hpp"

namespace sld {

    struct os_system_cpu_info_t;
    struct os_system_cpu_cache_info_t;
    struct os_system_memory_info_t;

    using os_system_get_cpu_info_f          = void         (*) (os_system_cpu_info_t&       cpu_info);
    using os_system_get_cpu_cache_info_f    = void         (*) (os_system_cpu_cache_info_t& cpu_cache_info);
    using os_system_get_memory_info_f       = void         (*) (os_system_memory_info_t&    memory_info);
    using os_system_time_ms_f               = const u64    (*) (void);
    using os_system_sleep_f                 = void         (*) (const u32 ms);
    using os_system_debug_print_f           = void         (*) (const cchar* debug_string);
    using os_system_get_working_directory_f = const cchar* (*) (void);

    struct os_system_cpu_cache_info_t {
        u32 level;
        u32 total_size;
        u32 line_size;
    };

    struct os_system_cpu_info_t {
        u32 parent_core_number;
        u32 speed_mhz;
        u32 core_count_physical;
        u32 core_count_logical;
        u32 cache_levels;
    };

    struct os_system_memory_info_t {
        u32 page_size;
        u32 allocation_granularity;
    };
};

#endif //SLD_OS_SYSTEM_HPP