#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {
    
    SLD_API_OS_FUNC void*
    win32_memory_alloc(
        const u64 size) {

        assert(size != 0);

        void* mem = VirtualAlloc(
            NULL,                     // no starting address
            size,                     // size
            MEM_RESERVE | MEM_COMMIT, // type
            PAGE_READWRITE            // security
        );

        return(mem);
    }

    SLD_API_OS_FUNC bool
    win32_memory_free(
        void* start) {

        assert(start != NULL);

        const bool is_free = VirtualFree(
            start,
            0,
            MEM_RELEASE   
        );

        return(is_free);
    }

    void*
    win32_memory_reserve(
        void*     start,
        const u64 size) {

        void* memory = (void*)VirtualAlloc(
            start,
            size,
            MEM_RESERVE,
            PAGE_NOACCESS
        );

        return(memory);
    }

    SLD_API_OS_FUNC bool
    win32_memory_release(
        void*     start,
        const u64 size) {

        bool result = VirtualFree(
            start,
            0, // size isn't needed for this api
            MEM_RELEASE
        );

        if (!result) {
            const u32 error = GetLastError();
        }

        return(result);
    }

    SLD_API_OS_FUNC void*
    win32_memory_commit(
        void*     start,
        const u64 size) {

        void* memory = (void*)VirtualAlloc(
            start,
            size,
            MEM_COMMIT,
            PAGE_READWRITE
        );

        return(memory);
    }

    SLD_API_OS_FUNC bool
    win32_memory_decommit(
        void*     start,
        const u64 size) {

        bool result = VirtualFree(
            start,
            size,
            MEM_DECOMMIT
        );

        return(result);
    }

    SLD_API_OS_FUNC u64
    win32_memory_align_to_page(
        const u64 size) {

        SLD_API_OS_FUNC SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);

        const u64 size_aligned = size_align_pow_2(size, sys_info.dwPageSize);
        return(size_aligned);
    }

    SLD_API_OS_FUNC u64
    win32_memory_align_to_granularity(
        const u64 size) {

        SLD_API_OS_FUNC SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);

        const u64 size_aligned = size_align_pow_2(size, sys_info.dwAllocationGranularity);
        return(size_aligned);
    }

    SLD_API_OS_FUNC bool
    win32_memory_is_reserved(
        void* start) {

        if (!start) return(false);

        MEMORY_BASIC_INFORMATION memory_info;
        const SIZE_T result = VirtualQuery(
            start,
            &memory_info,
            sizeof(memory_info)
        );

        const bool is_reserved = (memory_info.State == MEM_RESERVE);
        return(is_reserved); 
    }

    SLD_API_OS_FUNC bool
    win32_memory_is_committed(
        void* start) {

        if (!start) return(false);

        MEMORY_BASIC_INFORMATION memory_info;
        const SIZE_T result = VirtualQuery(
            start,
            &memory_info,
            sizeof(memory_info)
        );

        const bool is_committed = (memory_info.State == MEM_COMMIT);
        return(is_committed); 
    }


};