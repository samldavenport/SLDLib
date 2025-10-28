#pragma once

#include "sld-os.hpp"

#include "sld-win32-file.cpp"
#include "sld-win32-system.cpp"
#include "sld-win32-memory.cpp"
#include "sld-win32-window-opengl3.cpp"
#include "sld-win32-window.cpp"
#include "sld-win32-thread.cpp"
#include "sld-win32-monitor.cpp"

namespace sld {

    // system
    os_system_get_cpu_info_f         os_system_get_cpu_info         = win32_system_get_cpu_info;
    os_system_get_cpu_cache_info_f   os_system_get_cpu_cache_info   = win32_system_get_cpu_cache_info;
    os_system_get_memory_info_f      os_system_get_memory_info      = win32_system_get_memory_info;
    os_system_time_ms_f              os_system_time_ms              = win32_system_time_ms;
    os_system_sleep_f                os_system_sleep                = win32_system_sleep;
    os_system_debug_print_f          os_system_debug_print          = win32_system_debug_print;

    // monitors
    os_monitor_count_f               os_monitor_count               = win32_monitor_count;
    os_monitor_working_area_f        os_monitor_working_area        = win32_monitor_working_area; 
    os_monitor_info_f                os_monitor_info                = win32_monitor_info;
    os_monitor_primary_f             os_monitor_primary             = win32_monitor_primary;
    os_monitor_from_point_f          os_monitor_from_point          = win32_monitor_from_point;

    // window
    os_window_create_f               os_window_create               = win32_window_create;
    os_window_destroy_f              os_window_destroy              = win32_window_destroy;
    os_window_update_f               os_window_update               = win32_window_update;
    os_window_swap_buffers_f         os_window_swap_buffers         = win32_window_swap_buffers;
    os_window_show_f                 os_window_show                 = win32_window_show;
    os_window_get_size_f             os_window_get_size             = win32_window_get_size;
    os_window_get_position_f         os_window_get_position         = win32_window_get_position;
    os_window_set_viewport_f         os_window_set_viewport         = win32_window_opengl3_set_viewport;
    os_window_set_clear_color_f      os_window_set_clear_color      = win32_window_opengl3_set_clear_color; 
    os_window_open_file_dialog_f     os_window_open_file_dialog     = win32_window_open_file_dialog; 
    os_window_save_file_dialog_f     os_window_save_file_dialog     = win32_window_save_file_dialog;

    // memory
    os_memory_reserve_f              os_memory_reserve              = win32_memory_reserve;
    os_memory_release_f              os_memory_release              = win32_memory_release;
    os_memory_commit_f               os_memory_commit               = win32_memory_commit;
    os_memory_decommit_f             os_memory_decommit             = win32_memory_decommit;
    os_memory_align_to_page_f        os_memory_align_to_page        = win32_memory_align_to_page;
    os_memory_align_to_granularity_f os_memory_align_to_granularity = win32_memory_align_to_granularity;
    os_memory_is_reserved_f          os_memory_is_reserved          = win32_memory_is_reserved;
    os_memory_is_committed_f         os_memory_is_committed         = win32_memory_is_committed;
    
    // files
    os_file_open_f                   os_file_open                   = win32_file_open; 
    os_file_size_f                   os_file_size                   = win32_file_size; 
    os_file_read_f                   os_file_read                   = win32_file_read; 
    os_file_write_f                  os_file_write                  = win32_file_write; 
    os_file_read_async_f             os_file_read_async             = win32_file_read_async; 
    os_file_write_async_f            os_file_write_async            = win32_file_write_async;
    os_file_get_working_directory_f  os_file_get_working_directory  = win32_file_get_working_directory; 
};