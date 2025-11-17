#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {

    constexpr u32 WIN32_WORKING_DIRECTORY_SIZE = 32;


    SLD_API_OS_FUNC void
    win32_system_get_cpu_info(
        os_system_cpu_info_t& cpu_info) {


    }

    SLD_API_OS_FUNC void
    win32_system_get_cpu_cache_info(
        os_system_cpu_cache_info_t& cpu_cache_info) {

    }
    
    SLD_API_OS_FUNC void
    win32_system_get_memory_info(
        os_system_memory_info_t& memory_info) {

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