#ifndef SLD_OS_FILE_HPP
#define SLD_OS_FILE_HPP

#include "sld.hpp"

namespace sld {

    constexpr u32 OS_FILE_PATH_SIZE          = 256;
    constexpr u32 OS_FILE_ASYNC_CONTEXT_SIZE = 1024;

    using  os_file_handle_t       = vptr_t;
    using  os_file_error_t        = s32_t;
    using  os_file_access_flags_t = u8_t;
    using  os_file_share_flags_t  = u8_t;
    using  os_file_mode_t         = u8_t;
    struct os_file_config_t;
    struct os_file_buffer_t;
    struct os_file_callback_context_t;
    struct os_file_os_context_t;
    struct os_file_async_context_t;
    struct os_file_path_cstr_t;

    using os_file_async_callback_f        = void                  (*) (const void* data, const os_file_error_t error, const u32 bytes_transferred);
    using os_file_open_f                  = const os_file_error_t (*) (os_file_handle_t&      file_handle, const cchar* path, const os_file_config_t* config);
    using os_file_size_f                  = const os_file_error_t (*) (const os_file_handle_t file_handle, u64& size);
    using os_file_read_f                  = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t* buffer);    
    using os_file_write_f                 = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t* buffer);    
    using os_file_read_async_f            = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t* buffer, os_file_async_context_t* context);    
    using os_file_write_async_f           = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t* buffer, os_file_async_context_t* context);    
    using os_file_get_working_directory_f = const os_file_error_t (*) (os_file_path_cstr_t& file_path); 

    struct os_file_buffer_t {
        byte* data;
        u64   size;
        u64   cursor;
        u64   transferred;
    };

    struct os_file_os_context_t {
        byte data[OS_FILE_ASYNC_CONTEXT_SIZE];                 
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

    struct os_file_path_cstr_t {
        cchar buffer[OS_FILE_PATH_SIZE];
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
        return(is_valid);
    }
};

#endif //SLD_OS_FILE_HPP