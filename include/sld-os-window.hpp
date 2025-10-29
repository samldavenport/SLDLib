#ifndef SLD_OS_WINDOW_HPP
#define SLD_OS_WINDOW_HPP

#include "sld.hpp"
#include "sld-geometry.hpp"
#include "sld-graphics.hpp"
#include "sld-input.hpp"

namespace sld {

    using  os_window_handle_t      = vptr_t;
    using  os_window_error_t       = s32_t;
    using  os_window_event_flags_t = u32_t; 
    using  os_window_size_t        = dims_u32_size_t;
    using  os_window_pos_t         = dims_u32_pos_t;
    using  os_window_color_t       = color_u32_t;
    struct os_window_update_t;
    struct os_window_dialog_t;

    using os_window_create_f             = const os_window_error_t (*) (os_window_handle_t&      window_handle, const c8* title, const os_window_size_t& size, const os_window_pos_t& position);
    using os_window_set_viewport_f       = const os_window_error_t (*) (const os_window_handle_t window_handle, const os_window_size_t& size, const os_window_pos_t& position); 
    using os_window_update_f             = const os_window_error_t (*) (const os_window_handle_t window_handle, os_window_update_t&           update);
    using os_window_get_size_f           = const os_window_error_t (*) (const os_window_handle_t window_handle, os_window_size_t&             size);
    using os_window_get_position_f       = const os_window_error_t (*) (const os_window_handle_t window_handle, os_window_pos_t&              position);
    using os_window_set_clear_color_f    = const os_window_error_t (*) (const os_window_handle_t window_handle, const os_window_color_t&      color);
    using os_window_destroy_f            = const os_window_error_t (*) (const os_window_handle_t window_handle);
    using os_window_swap_buffers_f       = const os_window_error_t (*) (const os_window_handle_t window_handle);
    using os_window_show_f               = const os_window_error_t (*) (const os_window_handle_t window_handle);
    using os_window_open_file_dialog_f   = const os_window_error_t (*) (const os_window_handle_t window_handle, os_window_dialog_t& dialog);
    using os_window_save_file_dialog_f   = const os_window_error_t (*) (const os_window_handle_t window_handle, os_window_dialog_t& dialog);


    struct os_window_update_t {
        input_keyboard_t*       keyboard;
        os_window_event_flags_t events;
    };

    struct os_window_dialog_t {
        const cchar* filter;
        const cchar* start;
        const cchar* title;
        cchar*       selection_buffer_cstr;
        u64          selection_buffer_size;
        bool         did_select;
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
        os_window_error_e_success                = 1,
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