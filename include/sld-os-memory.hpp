#ifndef SLD_OS_MEMORY_HPP
#define SLD_OS_MEMORY_HPP

#include "sld.hpp"

namespace sld {

    using os_memory_alloc_f                = void* (*) (const u64 size);
    using os_memory_free_f                 = bool  (*) (void* start); 
    using os_memory_reserve_f              = void* (*) (void* start, const u64 size);
    using os_memory_release_f              = bool  (*) (void* start, const u64 size);
    using os_memory_commit_f               = void* (*) (void* start, const u64 size);
    using os_memory_decommit_f             = bool  (*) (void* start, const u64 size);
    using os_memory_align_to_page_f        = u64   (*) (const u64 size);
    using os_memory_align_to_granularity_f = u64   (*) (const u64 size);
    using os_memory_is_reserved_f          = bool  (*) (void* start);
    using os_memory_is_committed_f         = bool  (*) (void* start);
    using os_memory_mapping_destroy_f      = bool  (*) (void* start);
};

#endif //SLD_OS_MEMORY_HPP