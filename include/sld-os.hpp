#ifndef SLD_OS_HPP
#define SLD_OS_HPP

#include "sld.hpp"
#include "sld-os-window.hpp"
#include "sld-os-file.hpp"
#include "sld-os-system.hpp"
#include "sld-os-monitor.hpp"
#include "sld-os-memory.hpp"
#include "sld-os-thread.hpp"

/**********************************************************************************/
/* OS                                                                             */
/**********************************************************************************/

#define    SLD_API_OS                          extern // os api declaration
#define    SLD_API_OS_FUNC                     static // os api function
#define    SLD_API_OS_INTERNAL                 static // internal os function

#define    SLD_OS_GRAPHICS_CONTEXT_OPENGL      0
#define    SLD_OS_GRAPHICS_CONTEXT_DIRECTX12   1
#ifndef    SLD_OS_GRAPHICS_CONTEXT_TYPE
#   define SLD_OS_GRAPHICS_CONTEXT_TYPE        SLD_OS_GRAPHICS_CONTEXT_OPENGL
#endif

namespace sld {

    //-------------------------------------------------------------------
    // SYSTEM
    //-------------------------------------------------------------------

    SLD_API_OS os_system_get_cpu_info_f          os_system_get_cpu_info;
    SLD_API_OS os_system_get_cpu_cache_info_f    os_system_get_cpu_cache_info;
    SLD_API_OS os_system_get_memory_info_f       os_system_get_memory_info;
    SLD_API_OS os_system_time_ms_f               os_system_time_ms;
    SLD_API_OS os_system_sleep_f                 os_system_sleep;
    SLD_API_OS os_system_debug_print_f           os_system_debug_print;
    SLD_API_OS os_system_get_working_directory_f os_system_get_working_directory;

    //-------------------------------------------------------------------
    // MONITORS
    //-------------------------------------------------------------------

    SLD_API_OS os_monitor_count_f               os_monitor_count;
    SLD_API_OS os_monitor_working_area_f        os_monitor_working_area;
    SLD_API_OS os_monitor_info_f                os_monitor_info;
    SLD_API_OS os_monitor_primary_f             os_monitor_primary;
    SLD_API_OS os_monitor_from_point_f          os_monitor_from_point; 

    //-------------------------------------------------------------------
    // WINDOW
    //-------------------------------------------------------------------

    SLD_API_OS os_window_create_f               os_window_create; 
    SLD_API_OS os_window_update_f               os_window_update; 
    SLD_API_OS os_window_swap_buffers_f         os_window_swap_buffers; 
    SLD_API_OS os_window_destroy_f              os_window_destroy; 
    SLD_API_OS os_window_show_f                 os_window_show; 
    SLD_API_OS os_window_get_size_f             os_window_get_size; 
    SLD_API_OS os_window_get_position_f         os_window_get_position; 
    SLD_API_OS os_window_set_viewport_f         os_window_set_viewport;  
    SLD_API_OS os_window_set_clear_color_f      os_window_set_clear_color;  
    SLD_API_OS os_window_open_file_dialog_f     os_window_open_file_dialog;
    SLD_API_OS os_window_save_file_dialog_f     os_window_save_file_dialog;

    //-------------------------------------------------------------------
    // MEMORY
    //-------------------------------------------------------------------

    SLD_API_OS os_memory_reserve_f              os_memory_reserve;
    SLD_API_OS os_memory_release_f              os_memory_release;
    SLD_API_OS os_memory_commit_f               os_memory_commit;
    SLD_API_OS os_memory_decommit_f             os_memory_decommit;
    SLD_API_OS os_memory_align_to_page_f        os_memory_align_to_page;
    SLD_API_OS os_memory_align_to_granularity_f os_memory_align_to_granularity;
    SLD_API_OS os_memory_is_reserved_f          os_memory_is_reserved;
    SLD_API_OS os_memory_is_committed_f         os_memory_is_committed;
    SLD_API_OS os_memory_alloc_f                os_memory_alloc;
    SLD_API_OS os_memory_free_f                 os_memory_free;


    //-------------------------------------------------------------------
    // FILES
    //-------------------------------------------------------------------

    SLD_API_OS os_file_open_f                   os_file_open;
    SLD_API_OS os_file_close_f                  os_file_close;
    SLD_API_OS os_file_read_f                   os_file_read;
    SLD_API_OS os_file_write_f                  os_file_write;
    SLD_API_OS os_file_get_size_f               os_file_get_size;

    //-------------------------------------------------------------------
    // THREADS
    //-------------------------------------------------------------------

    SLD_API_OS os_thread_create_f               os_thread_create;
    SLD_API_OS os_thread_destroy_f              os_thread_destroy;
    SLD_API_OS os_thread_exit_f                 os_thread_exit;
    SLD_API_OS os_thread_sleep_f                os_thread_sleep;
    SLD_API_OS os_thread_yield_f                os_thread_yield;
    SLD_API_OS os_thread_join_f                 os_thread_join;
    SLD_API_OS os_thread_mutex_create_f         os_thread_mutex_create;
    SLD_API_OS os_thread_mutex_destroy_f        os_thread_mutex_destroy;
    SLD_API_OS os_thread_mutex_lock_f           os_thread_mutex_lock;
    SLD_API_OS os_thread_mutex_unlock_f         os_thread_mutex_unlock;
    SLD_API_OS os_thread_mutexry_lock_f       os_thread_mutexry_lock;
    SLD_API_OS os_thread_condition_create_f     os_thread_condition_create;
    SLD_API_OS os_thread_condition_destroy_f    os_thread_condition_destroy;
    SLD_API_OS os_thread_condition_wait_f       os_thread_condition_wait;
    SLD_API_OS os_thread_condition_signal_f     os_thread_condition_signal;
    SLD_API_OS os_thread_condition_broadcast_f  os_thread_condition_broadcast;
};

#endif //SLD_OS_HPP