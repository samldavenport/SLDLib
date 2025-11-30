#ifndef SLD_OS_MEMORY_HPP
#define SLD_OS_MEMORY_HPP

#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    SLD_API_OS void* os_memory_alloc                (const u64 size);
    SLD_API_OS bool  os_memory_free                 (void* start); 
    SLD_API_OS void* os_memory_reserve              (void* start, const u64 size);
    SLD_API_OS bool  os_memory_release              (void* start, const u64 size);
    SLD_API_OS void* os_memory_commit               (void* start, const u64 size);
    SLD_API_OS bool  os_memory_decommit             (void* start, const u64 size);
    SLD_API_OS u64   os_memory_align_to_page        (const u64 size);
    SLD_API_OS u64   os_memory_align_to_granularity (const u64 size);
    SLD_API_OS bool  os_memory_is_reserved          (void* start);
    SLD_API_OS bool  os_memory_is_committed         (void* start);
    SLD_API_OS bool  os_memory_mapping_destroy      (void* start);
};

#endif //SLD_OS_MEMORY_HPP