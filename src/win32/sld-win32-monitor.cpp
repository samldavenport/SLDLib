#pragma once

#include <Windows.h>
#include "sld-os.hpp"
#include "sld-cstr.hpp"

namespace sld {

    struct win32_monitor_enumerator_t {
        u32                count;
        u32                index;
        os_monitor_info* monitor_info;        
    };

    static BOOL CALLBACK
    win32_monitor_enum_callback(
        HMONITOR h_monitor,
        HDC      hdc_monitor,
        LPRECT   lprc_monitor,
        LPARAM   dw_data){

        constexpr u32 info_size = sizeof(MONITORINFOEX);
        constexpr u32 name_size = sizeof(MONITORINFOEX::szDevice);

        // cast the data and check if we should enumerate
        win32_monitor_enumerator_t* enumerator = (win32_monitor_enumerator_t*)dw_data;
        const bool should_enumerate = (
            enumerator != NULL && 
            enumerator->index < enumerator->count
        );

        MONITORINFOEX win32_monitor_info;
        win32_monitor_info.cbSize = info_size;

        cstr_t win32_monitor_name;
        win32_monitor_name.chars = NULL;
        win32_monitor_name.size  = name_size; 

        cstr_t sld_monitor_name;
        sld_monitor_name.chars = NULL;
        sld_monitor_name.size  = name_size; 

        if (should_enumerate) {

            // get the info for the current monitor handle
            GetMonitorInfo(h_monitor, (MONITORINFO*)&win32_monitor_info);

            // copy the win32 info to our structure
            os_monitor_info& monitor_info = enumerator->monitor_info[enumerator->index];            
            monitor_info.os_handle    = h_monitor;
            monitor_info.index        = enumerator->index;
            monitor_info.pixel_width  = (win32_monitor_info.rcMonitor.right  - win32_monitor_info.rcMonitor.left); 
            monitor_info.pixel_height = (win32_monitor_info.rcMonitor.bottom - win32_monitor_info.rcMonitor.top); 
            monitor_info.position_x   = win32_monitor_info.rcMonitor.left;  
            monitor_info.position_y   = win32_monitor_info.rcMonitor.top;
            
            sld_monitor_name.chars = monitor_info.name_cstr;
            (void)cstr_copy_from(&sld_monitor_name, (cchar*)win32_monitor_info.szDevice, name_size);

            // update the index
            ++enumerator->index;
        }        
        
        return(should_enumerate);
    }

    static u32
    win32_monitor_count(
        void) {

        const u32 count = GetSystemMetrics(SM_CMONITORS);
        return(count);
    }

    static void
    win32_monitor_working_area(
        os_monitor_working_area& monitor_working_area) {

        monitor_working_area.virtual_pixel_width  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        monitor_working_area.virtual_pixel_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    }

    static void
    win32_monitor_info(
        os_monitor_info* monitor_info) {

        if (!monitor_info) return;

        win32_monitor_enumerator_t enumerator;
        enumerator.count        = GetSystemMetrics(SM_CMONITORS);
        enumerator.index        = 0;
        enumerator.monitor_info = monitor_info;

        (void)EnumDisplayMonitors(
            NULL,
            NULL,
            win32_monitor_enum_callback,
            (LPARAM)&enumerator
        );

    }

    static os_monitor_t
    win32_monitor_primary(
        void) {

        POINT        origin    = { 0, 0 };
        HMONITOR     h_monitor = MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY);
        os_monitor_t monitor   = { h_monitor };
        return(monitor);
    }

    static os_monitor_t
    win32_monitor_from_point(
        const u32 x,
        const u32 y) {

        LONG     long_x  = *(LONG*)&x;
        LONG     long_y  = *(LONG*)&y;
        POINT    point   = { long_x, long_y };
        HMONITOR h_monitor = MonitorFromPoint(point, MONITOR_DEFAULTTOPRIMARY);

        os_monitor_t monitor = { h_monitor };
        return(monitor);
    }
};