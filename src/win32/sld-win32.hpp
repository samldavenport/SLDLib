#ifndef SLD_WIN32_HPP
#define SLD_WIN32_HPP

#include <Windows.h>
#include <sld-os.hpp>

namespace sld {

    //-------------------------------------------------------------------
    // GLOBALS
    //-------------------------------------------------------------------

    SLD_GLOBAL os_file_error   _last_error_file;
    SLD_GLOBAL os_window_error _last_error_window;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    // window
    SLD_API_OS_INTERNAL LRESULT CALLBACK win32_window_callback_base        (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    SLD_API_OS_INTERNAL LRESULT CALLBACK win32_window_callback_imgui       (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    SLD_API_OS_INTERNAL void             win32_window_init_class_base      (LPWNDCLASSA window_class);
    SLD_API_OS_INTERNAL void             win32_window_init_class_imgui     (LPWNDCLASSA window_class);
    SLD_API_OS_INTERNAL void             win32_window_clear_last_error     (void);
    SLD_API_OS_INTERNAL void             win32_window_set_last_error       (void);

    // file
    SLD_API_OS_INTERNAL void             win32_file_set_last_error         (void);
    SLD_API_OS_INTERNAL void             win32_file_clear_last_error       (void);
    SLD_API_OS_INTERNAL const u64        win32_file_get_buffer_granularity (void);
    SLD_API_OS_INTERNAL LPOVERLAPPED     win32_file_get_overlapped         (os_file_async* async);
};

#ifdef     SLD_OS_FILE_SIZE_IO
#   undef  SLD_OS_FILE_SIZE_IO
#   define SLD_OS_FILE_SIZE_IO sizeof(OVERLAPPED)
#endif

#define win32_system_get_cpu_info          os_system_get_cpu_info
#define win32_system_get_cpu_cache_info    os_system_get_cpu_cache_info
#define win32_system_get_memory_info       os_system_get_memory_info
#define win32_system_time_ms               os_system_time_ms
#define win32_system_sleep                 os_system_sleep
#define win32_system_debug_print           os_system_debug_print
#define win32_system_get_working_directory os_system_get_working_directory

#define win32_monitor_count                os_monitor_count
#define win32_monitor_get_working_area     os_monitor_get_working_area
#define win32_monitor_get_info             os_monitor_get_info
#define win32_monitor_get_primary          os_monitor_get_primary
#define win32_monitor_from_point           os_monitor_from_point

#define win32_file_get_last_error          os_file_get_last_error
#define win32_file_open                    os_file_open
#define win32_file_close                   os_file_close
#define win32_file_get_size                os_file_get_size
#define win32_file_read                    os_file_read
#define win32_file_write                   os_file_write
#define win32_file_async_create            os_file_async_create
#define win32_file_async_destroy           os_file_async_destroy
#define win32_file_async_get_result        os_file_async_get_result
#define win32_file_async_wait              os_file_async_wait
#define win32_file_async_cancel            os_file_async_cancel
#define win32_file_async_read              os_file_async_read
#define win32_file_async_write             os_file_async_write
#define win32_file_mapped_buffer_create    os_file_mapped_buffer_create
#define win32_file_mapped_buffer_destroy   os_file_mapped_buffer_destroy
#define win32_file_mapped_buffer_read      os_file_mapped_buffer_read
#define win32_file_mapped_buffer_write     os_file_mapped_buffer_write

#define win32_window_get_last_error        os_window_get_last_error
#define win32_window_create                os_window_create
#define win32_window_init_opengl           os_window_init_opengl
#define win32_window_init_imgui            os_window_init_imgui
#define win32_window_set_viewport          os_window_set_viewport
#define win32_window_set_clear_color       os_window_set_clear_color
#define win32_window_frame_start           os_window_frame_start
#define win32_window_frame_render          os_window_frame_render
#define win32_window_process_events        os_window_process_events
#define win32_window_get_size              os_window_get_size
#define win32_window_get_position          os_window_get_position
#define win32_window_open_file_dialog      os_window_open_file_dialog
#define win32_window_save_file_dialog      os_window_save_file_dialog
#define win32_window_destroy               os_window_destroy
#define win32_window_show                  os_window_show

#define win32_memory_alloc                os_memory_alloc
#define win32_memory_free                 os_memory_free
#define win32_memory_reserve              os_memory_reserve
#define win32_memory_release              os_memory_release
#define win32_memory_commit               os_memory_commit
#define win32_memory_decommit             os_memory_decommit
#define win32_memory_align_to_page        os_memory_align_to_page
#define win32_memory_align_to_granularity os_memory_align_to_granularity
#define win32_memory_is_reserved          os_memory_is_reserved
#define win32_memory_is_committed         os_memory_is_committed
#define win32_memory_mapping_destroy      os_memory_mapping_destroy

#endif //SLD_WIN32_HPP