#ifndef SLD_OS_WINDOW_HPP
#define SLD_OS_WINDOW_HPP

#include "sld.hpp"
#include "sld-geometry.hpp"
#include "sld-graphics.hpp"
#include "sld-input.hpp"

#ifndef SLD_OS_WINDOW_GRAPHICS_CONTEXT
#   define SLD_OS_WINDOW_GRAPHICS_CONTEXT 0
#endif 
#ifndef SLD_OS_WINDOW_GUI_CONTEXT
#   define SLD_OS_WINDOW_GUI_CONTEXT 0
#endif 


namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    using os_window_handle           = void*;
    using os_window_graphics_context = u32;
    using os_window_gui_context      = u32; 
    using os_window_error            = s32;
    using os_window_event_type       = u32;
    using os_window_keycode          = input_keycode;

    struct os_window_size;
    struct os_window_position;
    struct os_window_config;
    struct os_window_color;
    struct os_window_event;
    struct os_window_event_list;
    struct os_window_dialog;
    struct os_window_viewport;
    struct os_window_color;
    struct os_window_mouse_position;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    // error 
    SLD_API_OS os_window_error os_window_get_last_error   (void);

    // graphics/gui specific
    SLD_API_OS os_window_handle os_window_create           (const os_window_config*   config);
    SLD_API_OS bool             os_window_set_viewport     (const os_window_handle window, const os_window_viewport* viewport); 
    SLD_API_OS bool             os_window_set_clear_color  (const os_window_handle window, const os_window_color*    color);
    SLD_API_OS bool             os_window_frame_start      (const os_window_handle window);
    SLD_API_OS bool             os_window_frame_render     (const os_window_handle window);

    // common    
    SLD_API_OS bool            os_window_process_events   (const os_window_handle window, os_window_event_list* event_list);
    SLD_API_OS bool            os_window_get_size         (const os_window_handle window, os_window_size*       size);
    SLD_API_OS bool            os_window_get_position     (const os_window_handle window, os_window_position*   position);
    SLD_API_OS bool            os_window_open_file_dialog (const os_window_handle window, os_window_dialog*     dialog);
    SLD_API_OS bool            os_window_save_file_dialog (const os_window_handle window, os_window_dialog*     dialog);
    SLD_API_OS bool            os_window_destroy          (const os_window_handle window);
    SLD_API_OS bool            os_window_show             (const os_window_handle window);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct os_window_position {
        u32 x;
        u32 y;
    };

    struct os_window_mouse_position {
        u32 x;
        u32 y;        
    };

    struct os_window_size {
        u32 width;
        u32 height;
    };

    struct os_window_viewport {
        u32 width;
        u32 height; 
        u32 pos_x;
        u32 pos_y;
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
        cchar*             title;
        os_window_size     size;
        os_window_position position;
    };

    struct os_window_event_list {
        u32              capacity;
        u32              count;
        os_window_event* array;
    };

    struct os_window_color {
        byte r;
        byte g;
        byte b;
        byte a;
    };

    struct os_window_event {
        os_window_event_type type;
        union {
            os_window_position       window_position;
            os_window_size           window_size;
            os_window_keycode        keycode;
            os_window_mouse_position mouse_position;
        };
    };

    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum os_window_graphics_context_e {
        os_window_graphics_context_e_none            = 0,
        os_window_graphics_context_e_opengl          = 1,
        os_window_graphics_context_e_directx12       = 2
    };
    enum os_window_gui_context_e {
        os_window_gui_context_e_none                 = 0,
        os_window_gui_context_e_imgui                = 1
    };
    enum os_window_event_type_e {
        os_window_event_type_e_unknown               = 0,
        os_window_event_type_e_quit                  = 1,
        os_window_event_type_e_destroyed             = 2,
        os_window_event_type_e_moved                 = 3,
        os_window_event_type_e_resized               = 4,
        os_window_event_type_e_key_down              = 5,
        os_window_event_type_e_key_up                = 6,
        os_window_event_type_e_mouse_move            = 7,
        os_window_event_type_e_mouse_left_down       = 8,
        os_window_event_type_e_mouse_left_up         = 9,
        os_window_event_type_e_mouse_left_dbl_click  = 10,
        os_window_event_type_e_mouse_right_down      = 11,
        os_window_event_type_e_mouse_right_up        = 12,
        os_window_event_type_e_mouse_right_dbl_click = 13,
        os_window_event_type_e_mouse_wheel           = 14
    };
    enum os_window_error_e {
        os_window_error_e_success                    =  1,
        os_window_error_e_unknown                    = -1,
        os_window_error_e_resource_not_found         = -2,
        os_window_error_e_access_denied              = -3,
        os_window_error_e_system_out_of_memory       = -4,
        os_window_error_e_general_out_of_memory      = -5,
        os_window_error_e_invalid_args               = -6,
        os_window_error_e_invalid_handle             = -7,
        os_window_error_e_invalid_class              = -8,
        os_window_error_e_class_already_exists       = -9,
        os_window_error_e_invalid_device_context     = -10,
        os_window_error_e_invalid_thread             = -11,
        os_window_error_e_invalid_resource           = -12,
        os_window_error_e_quota_exceeded             = -13
    };
};

#endif //SLD_OS_WINDOW_HPP