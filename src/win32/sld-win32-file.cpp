#pragma once

#include <Windows.h>
#include "sld-os.hpp"

#ifdef     SLD_OS_FILE_IO_SIZE
#   undef  SLD_OS_FILE_IO_SIZE
#   define SLD_OS_FILE_IO_SIZE sizeof(OVERLAPPED)
#endif

namespace sld {

    struct test_t : structured_type_t<void*> {
 
    };

    //-------------------------------------------------------------------
    // GLOBALS
    //-------------------------------------------------------------------

    SLD_GLOBAL os_file_error_t _file_error;

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    struct win32_file_args_t {
        LPSECURITY_ATTRIBUTES security;
        HANDLE                template_handle;
        DWORD                 access;
        DWORD                 share;
        DWORD                 mode;
        DWORD                 flags;
    };

    // internal methods
    void                   win32_file_clear_last_error       (void);
    void                   win32_file_set_last_error         (void);
    const u64              win32_file_get_buffer_granularity (void);
    LPOVERLAPPED           win32_file_get_overlapped         (os_file_io_t* io);

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC bool
    win32_file_open(
        os_file_t*              file,
        const os_file_config_t* config,
        const cchar*            path) {

        win32_file_clear_last_error();
        
        // check args
        const bool can_open = (
            file   != NULL &&
            config != NULL &&
            path   != NULL
        );
        assert(can_open);

        win32_file_args_t file_args = {0};
        file_args.access          = 0;
        file_args.security        = NULL;
        file_args.template_handle = NULL;
        file_args.flags           = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED; 

        // access
        const bool access_read  = (config->access_flags & os_file_access_flag_e_read);
        const bool access_write = (config->access_flags & os_file_access_flag_e_write);
        if (access_read)  file_args.access |= GENERIC_READ;
        if (access_write) file_args.access |= GENERIC_WRITE;

        // share        
        const bool share_read   = (bool)(config->share_flags & os_file_share_flag_e_read);
        const bool share_write  = (bool)(config->share_flags & os_file_share_flag_e_write);
        const bool share_delete = (bool)(config->share_flags & os_file_share_flag_e_delete);
        if (share_read)   file_args.share  |= FILE_SHARE_READ;
        if (share_write)  file_args.share  |= FILE_SHARE_WRITE;
        if (share_delete) file_args.share  |= FILE_SHARE_DELETE;

        // mode
        switch (config->mode.val) {
            case(os_file_mode_e_create_new):          file_args.mode = CREATE_NEW;    break;
            case(os_file_mode_e_open_existing):       file_args.mode = OPEN_EXISTING; break;
            case(os_file_mode_e_open_always):         file_args.mode = OPEN_ALWAYS;   break;
            case(os_file_mode_e_overwrite_existing):  file_args.mode = CREATE_ALWAYS; break;
            default:                                  file_args.mode = CREATE_NEW;    break;
        }

        // create file
        file->handle = CreateFile(
            path,
            file_args.access,
            file_args.share,
            file_args.security,
            file_args.mode,
            file_args.flags,
            file_args.template_handle
        );

        const bool did_create = (file->handle != INVALID_HANDLE_VALUE);
        if (!did_create) {
            win32_file_set_last_error();
        }        
        return(did_create);
    }

    SLD_API_OS_FUNC bool
    win32_file_close(
        os_file_t* file) {

        assert(file);

        win32_file_clear_last_error();

        const bool did_close = (bool)CloseHandle(file->handle);
        if (!did_close) {
            win32_file_set_last_error();
        }

        file->handle = INVALID_HANDLE_VALUE;
        return(did_close);
    }

    SLD_API_OS_FUNC bool
    win32_file_create_io(
        os_file_t*    file,
        os_file_io_t* io) {

        assert(file != NULL && io != NULL);
        win32_file_clear_last_error();

        // initialize the overlapped structure
        LPOVERLAPPED overlapped = win32_file_get_overlapped(io);    
        ZeroMemory(overlapped, sizeof(OVERLAPPED));
        overlapped->hEvent = CreateEvent(NULL, true, false, NULL);
        
        const bool did_create = (overlapped->hEvent != NULL);
        if (!did_create) {
            win32_file_set_last_error();
        }

        return(did_create);
    }

    SLD_API_OS_FUNC bool
    win32_file_destroy_io(
        os_file_t*    file,
        os_file_io_t* io) {

        assert(file != NULL && io != NULL);
        win32_file_clear_last_error();

        LPOVERLAPPED overlapped = win32_file_get_overlapped(io);    
        ZeroMemory(overlapped, sizeof(OVERLAPPED));

        const bool did_destroy = (bool)CloseHandle(overlapped->hEvent);
        if (!did_destroy) {
            win32_file_set_last_error();
        }

        return(did_destroy);
    }

    SLD_API_OS_FUNC bool
    win32_file_create_map(
        os_file_t*         file,
        os_file_vbuffer_t* vbuffer) {

        assert(
            file          != NULL &&
            vbuffer       != NULL &&
            vbuffer->map  == NULL &&
            vbuffer->data == NULL
        );

        // get the file size
        HANDLE        file_handle = (HANDLE)file->handle;
        LARGE_INTEGER file_size   = {0};
        if (!GetFileSizeEx(file->handle, &file_size)) {
            win32_file_set_last_error();
            return(false);
        }

        // use the larger size and align
        // by the granularity
        u64 file_buffer_size = (file_size.QuadPart > vbuffer->size)
            ? file_size.QuadPart
            : vbuffer->size;
        static const u64 granularity = win32_file_get_buffer_granularity();
        file_buffer_size             = size_align_pow_2(file_buffer_size, granularity);

        // set the end of the file
        static const PLARGE_INTEGER file_pointer_new         = NULL;
        static const DWORD          file_pointer_move_method = FILE_BEGIN; 
        (void)SetFilePointerEx(
            file_handle,
            file_buffer_size,
            file_pointer_new,
            file_pointer_move_method
        );
        (void)SetEndOfFile(file_handle);

        // create the map
        static const LPSECURITY_ATTRIBUTES file_map_attributes    = NULL;
        static const DWORD                 file_map_protect       = FILE_MAP_ALL_ACCESS;
        static const DWORD                 file_map_max_size_high = 0;
        static const DWORD                 file_map_max_size_low  = 0;
        static const LPCSTR                file_map_name          = NULL;
        HANDLE file_map_handle = CreateFileMapping(
            file_handle,
            file_map_attributes,
            file_map_protect,
            file_map_max_size_high,
            file_map_max_size_low,
            file_map_name
        );
        if (!file_map_handle) {
            win32_file_set_last_error();
            return(false);
        } 

        // map the file to the buffer
        static const DWORD file_data_access = PAGE_READWRITE;
        static const DWORD file_offset_high = 0;
        static const DWORD file_offset_low  = 0;
        PVOID file_data = MapViewOfFile(
            file_map_handle,
            file_data_access,
            file_offset_high,
            file_offset_low,
        );
        if (!file_data) {
            win32_file_set_last_error();
            (void)CloseHandle(file_map_handle);
            return(false);
        }

        // set the buffer properties
        vbuffer->map    = file_map_handle;
        vbuffer->data   = file_data;
        vbuffer->size   = file_buffer_size;
        vbuffer->offset = 0;
        vbuffer->cursor = 0;
        assert(
            vbuffer->map  != NULL
            vbuffer->data != NULL
            vbuffer->size != 0
        );
        return(true);
    }

    SLD_API_OS_FUNC bool
    win32_file_map_to_buffer(
        os_file_map_t*    map,
        os_file_buffer_t* buffer
    );

    SLD_API_OS_FUNC bool 
    win32_file_destroy_virtual_buffer(
        os_file_t*         file,
        os_file_vbuffer_t* vbuffer) {

        assert(file != NULL && vbuffer != NULL);

        UnmapViewOfFile(vbuffer->data);
        CloseHandle(vbuffer->map);
    }

    SLD_API_OS_FUNC os_file_error_t
    win32_file_get_last_error(
        void) {

        return(_file_error);
    }

    SLD_API_OS_FUNC bool
    win32_file_io_read(
        os_file_t*        file,
        os_file_io_t*     io,
        os_file_buffer_t* buffer) {

        assert(
            file           != NULL &&
            io             != NULL &&
            buffer         != NULL &&            
            buffer->offset <  buffer->size
        );
        win32_file_clear_last_error();

        HANDLE       read_handle            = (HANDLE)file->handle; 
        LPOVERLAPPED read_overlapped        = win32_file_get_overlapped(io);
        LPDWORD      read_bytes_transferred = NULL; // null because overlapped is tracking io
        LPVOID       read_data              = (LPVOID)(((addr)buffer->data) + buffer->offset);
        DWORD        read_size              = (buffer->size - buffer->offset);
        overlapped->Offset = buffer->cursor;

        const bool did_read = ReadFile(
            read_handle,
            read_overlapped,
            read_bytes_transferred,
            read_data,
            read_size
        );

        if (!did_read) {
            win32_file_set_last_error();
        }
        return(did_read);
    }
    
    SLD_API_OS_FUNC bool
    win32_file_io_write(
        const os_file_t   file,
        os_file_io_t*     io,
        os_file_buffer_t* buffer) {

        assert(
            file           != NULL &&
            io             != NULL &&
            buffer         != NULL &&            
            buffer->offset <  buffer->size
        );
        win32_file_clear_last_error();

        HANDLE       write_handle            = (HANDLE)file->handle; 
        LPOVERLAPPED write_overlapped        = win32_file_get_overlapped(io);
        LPDWORD      write_bytes_transferred = NULL; // null because overlapped is tracking io
        LPVOID       write_data              = (LPVOID)(((addr)buffer->data) + buffer->offset);
        DWORD        write_size              = (buffer->size - buffer->offset);
        overlapped->Offset = buffer->cursor;

        const bool did_read = WriteFile(
            write_handle,
            write_overlapped,
            write_bytes_transferred,
            write_data,
            write_size
        );

        if (!did_read) {
            win32_file_set_last_error();
        }
        return(did_read);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_API_OS_INTERNAL void
    win32_file_clear_last_error(
        void) {
        
        _file_error = os_file_error_e_success;
    }

    SLD_API_OS_INTERNAL void
    win32_file_set_last_error(
        void) {

        const DWORD win32_error = GetLastError();

        switch (win32_error) {
            case (ERROR_SUCCESS):              { _file_error = os_file_error_e_success;             }
            case (ERROR_INVALID_PARAMETER):    { _file_error = os_file_error_e_invalid_args;        }
            case (ERROR_INVALID_HANDLE):       { _file_error = os_file_error_e_invalid_handle;      }
            case (ERROR_SECTOR_NOT_FOUND):     { _file_error = os_file_error_e_invalid_disk;        }
            case (ERROR_DEVICE_NOT_CONNECTED): { _file_error = os_file_error_e_invalid_device;      }
            case (ERROR_INVALID_USER_BUFFER):  { _file_error = os_file_error_e_invalid_buffer;      }
            case (ERROR_FILE_INVALID):         { _file_error = os_file_error_e_invalid_file;        }
            case (ERROR_SHARING_VIOLATION):    { _file_error = os_file_error_e_sharing_violation;   }
            case (ERROR_ALREADY_EXISTS):       { _file_error = os_file_error_e_already_exists;      }
            case (ERROR_FILE_EXISTS):          { _file_error = os_file_error_e_already_exists;      }
            case (ERROR_FILE_NOT_FOUND):       { _file_error = os_file_error_e_not_found;           }
            case (ERROR_ACCESS_DENIED):        { _file_error = os_file_error_e_access_denied;       }
            case (ERROR_PIPE_BUSY):            { _file_error = os_file_error_e_pipe_busy;           }
            case (ERROR_HANDLE_EOF):           { _file_error = os_file_error_e_reached_end_of_file; }
            case (ERROR_BROKEN_PIPE):          { _file_error = os_file_error_e_broken_pipe;         }
            case (ERROR_NO_DATA):              { _file_error = os_file_error_e_no_data;             }
            case (ERROR_MORE_DATA):            { _file_error = os_file_error_e_more_data;           }
            case (ERROR_IO_INCOMPLETE):        { _file_error = os_file_error_e_io_incomplete;       }
            case (ERROR_IO_PENDING):           { _file_error = os_file_error_e_io_pending;          }
            case (ERROR_OPERATION_ABORTED):    { _file_error = os_file_error_e_operation_aborted;   }
            case (ERROR_CRC):                  { _file_error = os_file_error_e_disk_io_failure;     }
            case (ERROR_DISK_CORRUPT):         { _file_error = os_file_error_e_disk_corrupt;        }
            case (ERROR_NOT_READY):            { _file_error = os_file_error_e_device_not_ready;    }
            case (ERROR_GEN_FAILURE):          { _file_error = os_file_error_e_device_failure;      }
            case (ERROR_NOT_ENOUGH_MEMORY):    { _file_error = os_file_error_e_out_of_memory;       }
            default:                           { _file_error = os_file_error_e_unknown;             }
        }
    }

    SLD_API_OS_INTERNAL const u64
    win32_file_get_buffer_granularity(
        void) {

        static u64 granularity = 0;

        if (granularity == 0) {
            SYSTEM_INFO sys_info;
            GetSystemInfo(&sys_info);
            granularity = sys_info.dwAllocationGranularity;
        }

        assert(granularity != 0);
        return(granularity);
    }

    SLD_API_OS_INTERNAL LPOVERLAPPED
    win32_file_get_overlapped(
        os_file_io_t* io) {

        LPOVERLAPPED overlapped = (LPOVERLAPPED)io->data;
        assert(overlapped);
        return(overlapped);
    }

    //-------------------------------------------------------------------
    // FUNCTION POINTERS
    //-------------------------------------------------------------------

    os_file_open_f           os_file_open           = win32_file_open; 
    //os_file_io_read_f        os_file_io_read           = win32_file_read; 
    //os_file_io_write_f       os_file_io_write          = win32_file_write; 
    //os_file_read_async_f     os_file_read_async     = win32_file_read_async; 
    //os_file_write_async_f    os_file_write_async    = win32_file_write_async; 
    //os_file_io_check_async_f os_file_io_check_async = win32_file_io_check_async; 
    //os_file_io_wait_f        os_file_io_wait        = win32_file_io_wait; 
    // os_file_get_size_f       os_file_get_size       = win32_file_get_size; 
    //os_file_close_f          os_file_close          = win32_file_close; 
};