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

    // window
    os_window_create_f                os_window_create                 = win32_window_create;
    os_window_destroy_f               os_window_destroy                = win32_window_destroy;
    os_window_process_events_f        os_window_update                 = win32_window_update;
    os_window_swap_buffers_f          os_window_swap_buffers           = win32_window_swap_buffers;
    os_window_show_f                  os_window_show                   = win32_window_show;
    os_window_get_size_f              os_window_get_size               = win32_window_get_size;
    os_window_get_position_f          os_window_get_position           = win32_window_get_position;
    os_window_set_viewport_f          os_window_set_viewport           = win32_window_opengl3_set_viewport;
    os_window_set_clear_color_f       os_window_set_clear_color        = win32_window_opengl3_set_clear_color; 
    os_window_open_file_dialog_f      os_window_open_file_dialog       = win32_window_open_file_dialog; 
    os_window_save_file_dialog_f      os_window_save_file_dialog       = win32_window_save_file_dialog;
};