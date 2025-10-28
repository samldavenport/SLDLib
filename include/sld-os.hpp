#ifndef SLD_OS_HPP
#define SLD_OS_HPP

#include "sld.hpp"
#include "sld-geometry.hpp"
#include "sld-graphics.hpp"
#include "sld-input.hpp"

/**********************************************************************************/
/* OS                                                                             */
/**********************************************************************************/


#define    SLD_API_OS                          extern        // os api declaration
#define    SLD_API_OS_FUNC                     static        // os api function
#define    SLD_API_OS_INTERNAL                 inline static // internal os function

#ifndef    SLD_OS_FILE_ASYNC_CONTEXT_SIZE
#   define SLD_OS_FILE_ASYNC_CONTEXT_SIZE      1024
#endif
#define    SLD_OS_GRAPHICS_CONTEXT_OPENGL      0
#define    SLD_OS_GRAPHICS_CONTEXT_DIRECTX12   1
#ifndef    SLD_OS_GRAPHICS_CONTEXT_TYPE
#   define SLD_OS_GRAPHICS_CONTEXT_TYPE        SLD_OS_GRAPHICS_CONTEXT_OPENGL
#endif

namespace sld {

    struct os_handle_t : vptr_t { };
    struct os_error_t  : s32_t  { };
    struct os_flags_t  : u32_t  { };

    //-------------------------------------------------------------------
    // SYSTEM
    //-------------------------------------------------------------------

    struct os_system_cpu_info_t;
    struct os_system_cpu_cache_info_t;
    struct os_system_memory_info_t;

    using os_system_get_cpu_info_f       = void      (*) (os_system_cpu_info_t&       cpu_info);
    using os_system_get_cpu_cache_info_f = void      (*) (os_system_cpu_cache_info_t& cpu_cache_info);
    using os_system_get_memory_info_f    = void      (*) (os_system_memory_info_t&    memory_info);
    using os_system_time_ms_f            = const u64 (*) (void);
    using os_system_sleep_f              = void      (*) (const u32 ms);
    using os_system_debug_print_f        = void      (*) (const c8* debug_string);

    struct os_system_cpu_cache_info_t {
        u32 level;
        u32 total_size;
        u32 line_size;
    };

    struct os_system_cpu_info_t {
        u32 parent_core_number;
        u32 speed_mhz;
        u32 core_count_physical;
        u32 core_count_logical;
        u32 cache_levels;
    };

    struct os_system_memory_info_t {
        u32 page_size;
        u32 allocation_granularity;
    };

    //-------------------------------------------------------------------
    // MONITORS
    //-------------------------------------------------------------------

    struct os_monitor_handle_t : os_handle_t { };
    struct os_monitor_info_t;
    struct os_monitor_working_area_t;
    
    using os_monitor_count_f        = u32                 (*) (void);
    using os_monitor_working_area_f = void                (*) (os_monitor_working_area_t& monitor_working_area);
    using os_monitor_info_f         = void                (*) (os_monitor_info_t*         monitor_info);
    using os_monitor_primary_f      = os_monitor_handle_t (*) (void);
    using os_monitor_from_point_f   = os_monitor_handle_t (*) (const u32 x, const u32 y);

    struct os_monitor_working_area_t {
        u32 virtual_pixel_width;
        u32 virtual_pixel_height;
    };

    struct os_monitor_screen_size_t {
        u32 pixel_width;
        u32 pixel_height;
    };

    constexpr u32 OS_MONITOR_NAME_WIDTH = 32;
    struct os_monitor_info_t {
        os_monitor_handle_t handle;
        u32                 index;
        u32                 pixel_width;
        u32                 pixel_height;
        u32                 position_x;
        u32                 position_y;
        cchar               name_cstr[OS_MONITOR_NAME_WIDTH];
    };

    //-------------------------------------------------------------------
    // WINDOW
    //-------------------------------------------------------------------

    struct os_window_handle_t      : os_handle_t { };
    struct os_window_error_t       : os_error_t  { };
    struct os_window_event_flags_t : os_flags_t  { }; 
    struct os_window_update_t;
    struct os_window_dialog_t;

    using  os_window_size_t  = dims_u32_size_t;
    using  os_window_pos_t   = dims_u32_pos_t;
    using  os_window_color_t = color_u32_t;

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
        struct {
            cchar* buffer;
            u64    size;
        } path_start;
        struct {
            cchar* buffer;
            u64    size;
        } path_selection;
        bool did_select;
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

    //-------------------------------------------------------------------
    // MEMORY
    //-------------------------------------------------------------------

    using os_memory_reserve_f              = void* (*) (void* start, const u64 size);
    using os_memory_release_f              = bool  (*) (void* start, const u64 size);
    using os_memory_commit_f               = void* (*) (void* start, const u64 size);
    using os_memory_decommit_f             = bool  (*) (void* start, const u64 size);
    using os_memory_align_to_page_f        = u64   (*) (const u64 size);
    using os_memory_align_to_granularity_f = u64   (*) (const u64 size);
    using os_memory_is_reserved_f          = bool  (*) (void* start);
    using os_memory_is_committed_f         = bool  (*) (void* start);

    //-------------------------------------------------------------------
    // FILES
    //-------------------------------------------------------------------

    constexpr u32 OS_FILE_PATH_SIZE = 256;

    struct os_file_handle_t : os_handle_t { };
    struct os_file_flags_t  : os_flags_t  { };
    struct os_file_error_t  : os_error_t  { };

    struct os_file_access_flags_t : u8_t { };
    struct os_file_share_flags_t  : u8_t { };
    struct os_file_mode_t         : u8_t { };
    struct os_file_config_t;

    struct os_file_buffer_t;
    struct os_file_callback_context_t;
    struct os_file_os_context_t;
    struct os_file_async_context_t;

    struct os_file_path_cstr_t {
        cchar buffer[OS_FILE_PATH_SIZE];
    };

    using os_file_async_callback_f        = void                  (*) (const void* data, const os_file_error_t error, const u32 bytes_transferred);
    using os_file_open_f                  = const os_file_error_t (*) (os_file_handle_t&      file_handle, const c8* path, const os_file_config_t& config);
    using os_file_size_f                  = const os_file_error_t (*) (const os_file_handle_t file_handle, u64& size);
    using os_file_read_f                  = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t& buffer);    
    using os_file_write_f                 = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t& buffer);    
    using os_file_read_async_f            = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t& buffer, os_file_async_context_t& context);    
    using os_file_write_async_f           = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t& buffer, os_file_async_context_t& context);    
    using os_file_get_working_directory_f = const os_file_error_t (*) (os_file_path_cstr_t& file_path); 

    struct os_file_buffer_t {
        byte* data;
        u64   size;
        u64   length;
        u64   cursor;
        u64   transferred;
    };

    struct os_file_os_context_t {
        byte data[SLD_OS_FILE_ASYNC_CONTEXT_SIZE];                 
    };

    struct os_file_callback_context_t {
        os_file_async_callback_f func;
        void*                    data;
    };

    struct os_file_async_context_t {
        os_file_os_context_t*       os;
        os_file_callback_context_t* callback;
    };

    struct os_file_context_t {
        os_file_handle_t handle;
        u32              bytes_transferred;
    };

    enum os_file_access_flag_e {
        os_file_access_flag_e_none           = 0,        
        os_file_access_flag_e_read           = bit_value(0),        
        os_file_access_flag_e_write          = bit_value(1)
    };

    enum os_file_share_flag_e {
        os_file_share_flag_e_none   = 0 ,
        os_file_share_flag_e_read   = bit_value(0),
        os_file_share_flag_e_write  = bit_value(1),
        os_file_share_flag_e_delete = bit_value(2)
    };

    enum os_file_mode_e {
        os_file_mode_e_create_new         = 0, 
        os_file_mode_e_open_existing      = 1, 
        os_file_mode_e_open_always        = 2, 
        os_file_mode_e_overwrite_existing = 3 
    };

    struct os_file_config_t {
        os_file_access_flags_t access_flags;
        os_file_share_flags_t  share_flags;
        os_file_mode_t         mode;
        bool                   is_async;
    };

    enum os_file_error_e {
        os_file_error_e_success             = 0,
        os_file_error_e_unknown             = 1,
        os_file_error_e_invalid_args        = 2,
        os_file_error_e_invalid_handle      = 3,
        os_file_error_e_invalid_disk        = 4,
        os_file_error_e_invalid_device      = 5,
        os_file_error_e_invalid_buffer      = 6,
        os_file_error_e_invalid_file        = 7,
        os_file_error_e_sharing_violation   = 8,
        os_file_error_e_already_exists      = 9,
        os_file_error_e_not_found           = 10,
        os_file_error_e_access_denied       = 11,
        os_file_error_e_pipe_busy           = 12,
        os_file_error_e_reached_end_of_file = 13,
        os_file_error_e_broken_pipe         = 14,
        os_file_error_e_no_data             = 15,
        os_file_error_e_more_data           = 16,
        os_file_error_e_io_incomplete       = 17,
        os_file_error_e_io_pending          = 18,
        os_file_error_e_operation_aborted   = 19,
        os_file_error_e_disk_io_failure     = 20,
        os_file_error_e_disk_corrupt        = 21,
        os_file_error_e_device_not_ready    = 22,
        os_file_error_e_out_of_memory       = 23,
        os_file_error_e_device_failure      = 24
    };

    static inline bool
    os_file_buffer_validate(
        const os_file_buffer_t& buffer) {
        
        bool is_valid = true;
        is_valid &= (buffer.data   != NULL);
        is_valid &= (buffer.size   != 0);
        is_valid &= (buffer.length <= buffer.size);
        return(is_valid);
    }

    //-------------------------------------------------------------------
    // THREADS
    //-------------------------------------------------------------------

    struct os_thread_handle_t           : os_handle_t { };
    struct os_thread_mutex_handle_t     : os_handle_t { };
    struct os_thread_condition_handle_t : os_handle_t { };
    struct os_thread_error_t            : os_error_t  { };

    struct os_thread_callback_data_t;
    struct os_thread_context_t;

    using os_thread_callback_function_f   = void (*) (os_thread_context_t& context);

    using os_thread_create_f              = const os_thread_error_t (*) (os_thread_handle_t&            thread_handle);
    using os_thread_destroy_f             = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_exit_f                = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_sleep_f               = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);     
    using os_thread_yield_f               = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_join_f                = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_mutex_create_f        = const os_thread_error_t (*) (os_thread_mutex_handle_t&      mutex_handle);
    using os_thread_mutex_destroy_f       = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_lock_f          = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_unlock_f        = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_try_lock_f      = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_condition_create_f    = const os_thread_error_t (*) (void);
    using os_thread_condition_destroy_f   = const os_thread_error_t (*) (void);
    using os_thread_condition_wait_f      = const os_thread_error_t (*) (void);
    using os_thread_condition_signal_f    = const os_thread_error_t (*) (void);
    using os_thread_condition_broadcast_f = const os_thread_error_t (*) (void);

    struct os_thread_callback_data_t {
        void* ptr;
        u64   size;
    };

    struct os_thread_context_t {
        os_thread_callback_function_f function;
        os_thread_callback_data_t     data;
    };

    enum os_thread_error_e {
        os_thread_error_e_success = 1,
        os_thread_error_e_unknown = -1
    };

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API_OS os_system_get_cpu_info_f         os_system_get_cpu_info;
    SLD_API_OS os_system_get_cpu_cache_info_f   os_system_get_cpu_cache_info;
    SLD_API_OS os_system_get_memory_info_f      os_system_get_memory_info;
    SLD_API_OS os_system_time_ms_f              os_system_time_ms;
    SLD_API_OS os_system_sleep_f                os_system_sleep;
    SLD_API_OS os_system_debug_print_f          os_system_debug_print;

    SLD_API_OS os_monitor_count_f               os_monitor_count;
    SLD_API_OS os_monitor_working_area_f        os_monitor_working_area;
    SLD_API_OS os_monitor_info_f                os_monitor_info;
    SLD_API_OS os_monitor_primary_f             os_monitor_primary;
    SLD_API_OS os_monitor_from_point_f          os_monitor_from_point; 

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

    SLD_API_OS os_memory_reserve_f              os_memory_reserve;
    SLD_API_OS os_memory_release_f              os_memory_release;
    SLD_API_OS os_memory_commit_f               os_memory_commit;
    SLD_API_OS os_memory_decommit_f             os_memory_decommit;
    SLD_API_OS os_memory_align_to_page_f        os_memory_align_to_page;
    SLD_API_OS os_memory_align_to_granularity_f os_memory_align_to_granularity;
    SLD_API_OS os_memory_is_reserved_f          os_memory_is_reserved;
    SLD_API_OS os_memory_is_committed_f         os_memory_is_committed;

    SLD_API_OS os_file_open_f                   os_file_open;
    SLD_API_OS os_file_size_f                   os_file_size;
    SLD_API_OS os_file_read_f                   os_file_read;
    SLD_API_OS os_file_write_f                  os_file_write;
    SLD_API_OS os_file_read_async_f             os_file_read_async;
    SLD_API_OS os_file_write_async_f            os_file_write_async;
    SLD_API_OS os_file_get_working_directory_f  os_file_get_working_directory;

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
    SLD_API_OS os_thread_mutex_try_lock_f       os_thread_mutex_try_lock;
    SLD_API_OS os_thread_condition_create_f     os_thread_condition_create;
    SLD_API_OS os_thread_condition_destroy_f    os_thread_condition_destroy;
    SLD_API_OS os_thread_condition_wait_f       os_thread_condition_wait;
    SLD_API_OS os_thread_condition_signal_f     os_thread_condition_signal;
    SLD_API_OS os_thread_condition_broadcast_f  os_thread_condition_broadcast;
};

#endif //SLD_OS_HPP