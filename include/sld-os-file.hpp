#ifndef SLD_OS_FILE_HPP
#define SLD_OS_FILE_HPP

#include "sld.hpp"

#ifndef    SLD_OS_FILE_SIZE_IO
#   define SLD_OS_FILE_SIZE_IO 1024
#endif

namespace sld {

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr u32   OS_FILE_SIZE_IO        = SLD_OS_FILE_SIZE_IO;
    constexpr u64   OS_FILE_SIZE_INVALID   = 0xFFFFFFFFFFFFFFFF; 
    constexpr void* OS_FILE_HANDLE_INVALID = NULL;

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    enum os_file_error_e       : s32;
    enum os_file_access_flag_e : u32;
    enum os_file_share_flag_e  : u32;
    enum os_file_mode_e        : u32;
    enum os_file_async_state_e : u32;

    using os_file_handle       = void*;
    using os_file_map_handle   = void*;
    using os_file_error        = s32;
    using os_file_access_flags = u32;
    using os_file_share_flags  = u32;
    using os_file_mode         = u32;
    using os_file_async_state  = u32;

    struct os_file_config;
    struct os_file_async;
    struct os_file_buffer;
    struct os_file_mapped_buffer;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    // error
    SLD_API_OS os_file_error os_file_get_last_error        (void);

    // file
    SLD_API_OS os_file_handle os_file_open                  (const os_file_config* config);
    SLD_API_OS bool           os_file_close                 (const os_file_handle file_hnd);
    SLD_API_OS u64            os_file_get_size              (const os_file_handle file_hnd);
    SLD_API_OS u64            os_file_read                  (const os_file_handle file_hnd, os_file_buffer* buffer);
    SLD_API_OS u64            os_file_write                 (const os_file_handle file_hnd, os_file_buffer* buffer);

    // async
    SLD_API_OS bool           os_file_async_create          (const os_file_handle file_hnd, os_file_async* async);
    SLD_API_OS bool           os_file_async_destroy         (const os_file_handle file_hnd, os_file_async* async);
    SLD_API_OS u64            os_file_async_get_result      (const os_file_handle file_hnd, os_file_async* async);
    SLD_API_OS u64            os_file_async_wait            (const os_file_handle file_hnd, os_file_async* async);      
    SLD_API_OS bool           os_file_async_cancel          (const os_file_handle file_hnd, os_file_async* async);
    SLD_API_OS bool           os_file_async_read            (const os_file_handle file_hnd, os_file_async* async, os_file_buffer* buffer);    
    SLD_API_OS bool           os_file_async_write           (const os_file_handle file_hnd, os_file_async* async, os_file_buffer* buffer);    

    // buffer
    SLD_API_OS bool           os_file_mapped_buffer_create  (const os_file_handle file_hnd, os_file_mapped_buffer* mapped_buffer);
    SLD_API_OS bool           os_file_mapped_buffer_destroy (const os_file_handle file_hnd, os_file_mapped_buffer* mapped_buffer);
    SLD_API_OS bool           os_file_mapped_buffer_read    (const os_file_handle file_hnd, os_file_mapped_buffer* mapped_buffer);
    SLD_API_OS bool           os_file_mapped_buffer_write   (const os_file_handle file_hnd, os_file_mapped_buffer* mapped_buffer);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct os_file_async {
        os_file_async_state   state;
        u32                   timeout_ms;
        byte                  data[SLD_OS_FILE_SIZE_IO];                 
    };

    struct os_file_buffer {
        byte* data;
        u64   size;
        u64   offset;
        u64   cursor;
    };

    struct os_file_config {
        cchar*               path;
        os_file_mode         mode;
        os_file_access_flags access_flags;
        os_file_share_flags  share_flags;
        bool                 is_async;
    };

    struct os_file_mapped_buffer {
        os_file_map_handle map_handle;        
        byte*              data;
        u64                size;
        u64                offset;
        u64                cursor;
    };

    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum os_file_access_flag_e : u32 {
        os_file_access_flag_e_none          = 0,        
        os_file_access_flag_e_read          = bit_value(0),        
        os_file_access_flag_e_write         = bit_value(1)
    };
    enum os_file_share_flag_e : u32 {
        os_file_share_flag_e_none           = 0,
        os_file_share_flag_e_read           = bit_value(0),
        os_file_share_flag_e_write          = bit_value(1),
        os_file_share_flag_e_delete         = bit_value(2)
    };
    enum os_file_mode_e : u32 {
        os_file_mode_e_create_new           = 0, 
        os_file_mode_e_open_existing        = 1, 
        os_file_mode_e_open_always          = 2, 
        os_file_mode_e_overwrite_existing   = 3 
    };
    enum os_file_async_state_e : u32 {
        os_file_async_state_e_success       = 0,
        os_file_async_state_e_error         = 1,
        os_file_async_state_e_pending       = 2,
        os_file_async_state_e_timeout       = 3
    };
    enum os_file_error_e : s32 {
        os_file_error_e_success             =  1,
        os_file_error_e_unknown             = -1,
        os_file_error_e_invalid_args        = -2,
        os_file_error_e_invalid_handle      = -3,
        os_file_error_e_invalid_disk        = -4,
        os_file_error_e_invalid_device      = -5,
        os_file_error_e_invalid_buffer      = -6,
        os_file_error_e_invalid_file        = -7,
        os_file_error_e_sharing_violation   = -8,
        os_file_error_e_already_exists      = -9,
        os_file_error_e_not_found           = -10,
        os_file_error_e_access_denied       = -11,
        os_file_error_e_pipe_busy           = -12,
        os_file_error_e_reached_end_of_file = -13,
        os_file_error_e_broken_pipe         = -14,
        os_file_error_e_no_data             = -15,
        os_file_error_e_more_data           = -16,
        os_file_error_e_io_incomplete       = -17,
        os_file_error_e_io_pending          = -18,
        os_file_error_e_operation_aborted   = -19,
        os_file_error_e_disk_io_failure     = -20,
        os_file_error_e_disk_corrupt        = -21,
        os_file_error_e_device_not_ready    = -22,
        os_file_error_e_out_of_memory       = -23,
        os_file_error_e_device_failure      = -24
    };
};

#endif //SLD_OS_FILE_HPP