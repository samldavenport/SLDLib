#pragma once

#include <Windows.h>
#include "sld-os.hpp"

#define win32_system_get_cpu_info          os_system_get_cpu_info
#define win32_system_get_cpu_cache_info    os_system_get_cpu_cache_info
#define win32_system_get_memory_info       os_system_get_memory_info
#define win32_system_time_ms               os_system_time_ms
#define win32_system_sleep                 os_system_sleep
#define win32_system_debug_print           os_system_debug_print
#define win32_system_get_working_directory os_system_get_working_directory

namespace sld {

    constexpr u32 WIN32_WORKING_DIRECTORY_SIZE = 32;

    SLD_API_OS_FUNC void
    win32_system_get_cpu_info(
        os_system_cpu_info& cpu_info) {


    }

    SLD_API_OS_FUNC void
    win32_system_get_cpu_cache_info(
        os_system_cpu_cache_info& cpu_cache_info) {

    }
    
    SLD_API_OS_FUNC void
    win32_system_get_memory_info(
        os_system_memory_info& memory_info) {

        SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);

        ULONGLONG sys_mem;
        GetPhysicallyInstalledSystemMemory(&sys_mem);

        assert(sys_mem <= 0xFFFFFFFF);

        memory_info.allocation_granularity = sys_info.dwAllocationGranularity;
        memory_info.page_size              = sys_info.dwPageSize;
        memory_info.installed_ram_size_kb  = (u32)sys_mem;
    }
    
    SLD_API_OS_FUNC const u64
    win32_system_time_ms(
        void) {

        return(0);
    }
    
    SLD_API_OS_FUNC void
    win32_system_sleep(
        const u32 ms) {

    }
    
    SLD_API_OS_FUNC void
    win32_system_debug_print(
        const cchar* debug_string) {

    }
    
    SLD_API_OS_FUNC const cchar*
    win32_system_get_working_directory(
        void) {

        static cchar directory[WIN32_WORKING_DIRECTORY_SIZE];

        const DWORD result      = GetCurrentDirectory(WIN32_WORKING_DIRECTORY_SIZE, (LPSTR)directory);
        const bool  did_succeed = (result != 0) && (result <= WIN32_WORKING_DIRECTORY_SIZE);

        assert(did_succeed);
        return(directory);
    }
};