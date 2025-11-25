#ifndef SLD_OS_SYSTEM_HPP
#define SLD_OS_SYSTEM_HPP

#include "sld.hpp"

namespace sld {

    struct os_system_cpu_info;
    struct os_system_cpu_cache_info;
    struct os_system_memory_info;

    using os_system_get_cpu_info_f          = void         (*) (os_system_cpu_info&       cpu_info);
    using os_system_get_cpu_cache_info_f    = void         (*) (os_system_cpu_cache_info& cpu_cache_info);
    using os_system_get_memory_info_f       = void         (*) (os_system_memory_info&    memory_info);
    using os_system_time_ms_f               = const u64    (*) (void);
    using os_system_sleep_f                 = void         (*) (const u32 ms);
    using os_system_debug_print_f           = void         (*) (const cchar* debug_string);
    using os_system_get_working_directory_f = const cchar* (*) (void);

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