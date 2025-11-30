#ifndef SLD_OS_WINDOW_HPP
#define SLD_OS_WINDOW_HPP

#include "sld.hpp"
#include "sld-geometry.hpp"
#include "sld-graphics.hpp"
#include "sld-input.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    using os_window_graphics_context = u32;
    using os_window_gui_context      = u32; 

    struct os_window_handle;
    struct os_window_error;
    struct os_window_size;
    struct os_window_pos;
    struct os_window_config;
    struct os_window_color;
    struct os_window_event_flags;
    struct os_window_handle;
    struct os_window_events;
    struct os_window_dialog;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    SLD_API_OS os_window_error os_window_get_last_error   (void);
    SLD_API_OS bool            os_window_create           (os_window_handle* window_hnd, const cchar* title, const os_window_size* size, const os_window_pos* position);
    SLD_API_OS bool            os_window_set_viewport     (os_window_handle* window_hnd, const os_window_size*  size, const os_window_pos* position); 
    SLD_API_OS bool            os_window_process_events   (os_window_handle* window_hnd, os_window_events*      events);
    SLD_API_OS bool            os_window_get_size         (os_window_handle* window_hnd, os_window_size*        size);
    SLD_API_OS bool            os_window_get_position     (os_window_handle* window_hnd, os_window_pos*         position);
    SLD_API_OS bool            os_window_set_clear_color  (os_window_handle* window_hnd, const os_window_color* color);
    SLD_API_OS bool            os_window_open_file_dialog (os_window_handle* window_hnd, os_window_dialog* dialog);
    SLD_API_OS bool            os_window_save_file_dialog (os_window_handle* window_hnd, os_window_dialog* dialog);
    SLD_API_OS bool            os_window_destroy          (os_window_handle* window_hnd);
    SLD_API_OS bool            os_window_swap_buffers     (os_window_handle* window_hnd);
    SLD_API_OS bool            os_window_show             (os_window_handle* window_hnd);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct os_window_handle      : vptr_t          { };
    struct os_window_error       : s32_t           { };
    struct os_window_size        : dims_u32_size_t { };
    struct os_window_pos         : dims_u32_pos_t  { };
    struct os_window_color       : color_u32_t     { };
    struct os_window_event_flags : u32_t           { };

    struct os_window_events {
        input_keyboard_t*     keyboard;
        os_window_event_flags events;
    };

    struct os_window_dialog {
        cchar* filter;
        cchar* start;
        cchar* title;
        cchar* selection_buffer_cstr;
        u64    selection_buffer_size;
        bool   did_select;
    };

    struct os_window_config {
        const cchar*               title;
        os_window_size             size;
        os_window_pos              position;
        os_window_graphics_context graphics_context; 
        os_window_gui_context      gui_context; 
    };

    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum os_window_graphics_context_e {
        os_window_graphics_context_e_none       = 0,
        os_window_graphics_context_e_opengl     = 1,
        os_window_graphics_context_e_directx12  = 2,
    };
    enum os_window_gui_context_e {
        os_window_gui_context_e_none            = 0,
        os_window_gui_context_e_imgui           = 1,
    };
    enum os_window_event_flag_e {
        os_window_event_e_none                  = 0,
        os_window_event_e_quit                  = bit_value(0),
        os_window_event_e_destroyed             = bit_value(1),
        os_window_event_e_moved                 = bit_value(2),
        os_window_event_e_resized               = bit_value(3),
        os_window_event_e_key_down              = bit_value(4),
        os_window_event_e_key_up                = bit_value(5),
        os_window_event_e_mouse_move            = bit_value(6),
        os_window_event_e_mouse_left_down       = bit_value(7),
        os_window_event_e_mouse_left_up         = bit_value(8),
        os_window_event_e_mouse_left_dbl_click  = bit_value(9),
        os_window_event_e_mouse_right_down      = bit_value(10),
        os_window_event_e_mouse_right_up        = bit_value(11),
        os_window_event_e_mouse_right_dbl_click = bit_value(12),
        os_window_event_e_mouse_wheel           = bit_value(13)
    };
    enum os_window_error_e {
        os_window_error_e_success                =  1,
        os_window_error_e_unknown                = -1,
        os_window_error_e_resource_not_found     = -2,
        os_window_error_e_access_denied          = -3,
        os_window_error_e_system_out_of_memory   = -4,
        os_window_error_e_general_out_of_memory  = -5,
        os_window_error_e_invalid_args           = -6,
        os_window_error_e_invalid_handle         = -7,
        os_window_error_e_invalid_class          = -8,
        os_window_error_e_class_already_exists   = -9,
        os_window_error_e_invalid_device_context = -10,
        os_window_error_e_invalid_thread         = -11,
        os_window_error_e_invalid_resource       = -12,
        os_window_error_e_quota_exceeded         = -13
    };
};

#endif //SLD_OS_WINDOW_HPP