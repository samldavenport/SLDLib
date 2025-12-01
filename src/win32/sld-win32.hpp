#ifndef SLD_WIN32_HPP
#define SLD_WIN32_HPP

#include <Windows.h>
#include <sld-os.hpp>

namespace sld {

    //-------------------------------------------------------------------
    // GLOBALS
    //-------------------------------------------------------------------

    SLD_GLOBAL os_file_error _file_last_error;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    // window
    SLD_API_OS_INTERNAL LRESULT CALLBACK win32_window_callback_base        (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    SLD_API_OS_INTERNAL LRESULT CALLBACK win32_window_callback_imgui       (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    SLD_API_OS_INTERNAL void             win32_window_init_class_base      (LPWNDCLASSA window_class);
    SLD_API_OS_INTERNAL void             win32_window_init_class_imgui     (LPWNDCLASSA window_class);

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

#define win32_file_get_last_error        os_file_get_last_error
#define win32_file_open                  os_file_open
#define win32_file_close                 os_file_close
#define win32_file_get_size              os_file_get_size
#define win32_file_read                  os_file_read
#define win32_file_write                 os_file_write
#define win32_file_async_create          os_file_async_create
#define win32_file_async_destroy         os_file_async_destroy
#define win32_file_async_get_result      os_file_async_get_result
#define win32_file_async_wait            os_file_async_wait
#define win32_file_async_cancel          os_file_async_cancel
#define win32_file_async_read            os_file_async_read
#define win32_file_async_write           os_file_async_write
#define win32_file_mapped_buffer_create  os_file_mapped_buffer_create
#define win32_file_mapped_buffer_destroy os_file_mapped_buffer_destroy
#define win32_file_mapped_buffer_read    os_file_mapped_buffer_read
#define win32_file_mapped_buffer_write   os_file_mapped_buffer_write

#endif //SLD_WIN32_HPP