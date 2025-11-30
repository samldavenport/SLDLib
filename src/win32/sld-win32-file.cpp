#pragma once

#include <Windows.h>
#include "sld-os.hpp"

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

namespace sld {

    //-------------------------------------------------------------------
    // GLOBALS
    //-------------------------------------------------------------------

    SLD_GLOBAL os_file_error _file_last_error;

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
    void            win32_file_set_last_error         (void);
    void            win32_file_clear_last_error       (void);
    const u64       win32_file_get_buffer_granularity (void);
    LPOVERLAPPED    win32_file_get_overlapped         (os_file_async* async);

    //-------------------------------------------------------------------
    // FILE METHODS
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC bool
    win32_file_open(
        os_file_handle*       file,
        const os_file_config* config,
        const cchar*            path) {

        // check args
        const bool can_open = (
            file   != NULL &&
            config != NULL &&
            path   != NULL
        );
        assert(can_open);
        win32_file_clear_last_error();

        win32_file_args_t file_args = {0};
        file_args.access          = 0;
        file_args.security        = NULL;
        file_args.template_handle = NULL;
        file_args.flags           = FILE_ATTRIBUTE_NORMAL; 

        // access
        const bool access_read  = (config->access_flags.val & os_file_access_flag_e_read);
        const bool access_write = (config->access_flags.val & os_file_access_flag_e_write);
        if (access_read)  file_args.access |= GENERIC_READ;
        if (access_write) file_args.access |= GENERIC_WRITE;

        // share        
        const bool share_read   = (config->share_flags.val & os_file_share_flag_e_read);
        const bool share_write  = (config->share_flags.val & os_file_share_flag_e_write);
        const bool share_delete = (config->share_flags.val & os_file_share_flag_e_delete);
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

        if (config->is_async) {
            file_args.flags |= FILE_FLAG_OVERLAPPED;
        }

        // create file
        file->val = CreateFile(
            path,
            file_args.access,
            file_args.share,
            file_args.security,
            file_args.mode,
            file_args.flags,
            file_args.template_handle
        );

        const bool did_create = (file->val != INVALID_HANDLE_VALUE);
        if (!did_create) {
            win32_file_set_last_error();
        }        
        return(did_create);
    }

    SLD_API_OS_FUNC bool
    win32_file_close(
        os_file_handle* file) {

        assert(file);

        win32_file_clear_last_error();

        const bool did_close = (bool)CloseHandle(file->val);
        if (!did_close) {
            win32_file_set_last_error();
        }

        file->val = INVALID_HANDLE_VALUE;
        return(did_close);
    }

    SLD_API_OS_FUNC u64
    win32_file_get_size(
        os_file_handle* file) {

        assert(file);

        win32_file_clear_last_error();

        LARGE_INTEGER size;
        const bool did_get_size = (bool)GetFileSizeEx(file->val, &size);

        if (!did_get_size) {
            size.QuadPart = OS_FILE_SIZE_INVALID;
            win32_file_set_last_error();
        }
        return(size.QuadPart);
    }

    SLD_API_OS_FUNC u64 
    win32_file_read(
        os_file_handle* file,
        os_file_buffer* buffer) {

        // check args and clear error
        assert(
            file           != NULL &&
            buffer         != NULL &&
            buffer->data   != NULL &&
            buffer->offset <  buffer->size
        );
        win32_file_clear_last_error();

        // set the pointer
        PLARGE_INTEGER file_pointer_new              = NULL;
        HANDLE         file_handle                   = file->val;
        DWORD          file_pointer_move_method      = FILE_BEGIN;
        LARGE_INTEGER  file_pointer_distance_to_move;
        file_pointer_distance_to_move.QuadPart = buffer->cursor;

        const bool did_set_pointer = (bool)SetFilePointerEx(
            file_handle,
            file_pointer_distance_to_move,
            file_pointer_new,
            file_pointer_move_method
        );
        if (!did_set_pointer) {
            win32_file_set_last_error();
            return(OS_FILE_SIZE_INVALID);
        }

        // do the read
        LPOVERLAPPED file_read_overlapped     = NULL;
        LPVOID       file_read_buffer         = (LPVOID)(buffer->data + buffer->offset);
        DWORD        file_read_size_requested = (buffer->size - buffer->offset); 
        DWORD        file_read_size_actual    = 0;

        const bool did_read = (bool)ReadFile(
            file_handle,
            file_read_buffer,
            file_read_size_requested,
            &file_read_size_actual,
            file_read_overlapped
        );

        // return the bytes read
        if (!did_read) {
            win32_file_set_last_error();
            file_read_size_actual = OS_FILE_SIZE_INVALID;
        }
        return(file_read_size_actual);
    }

    SLD_API_OS_FUNC u64 
    win32_file_write(
        os_file_handle* file,
        os_file_buffer* buffer) {

        // check args and clear error
        assert(
            file           != NULL &&
            buffer         != NULL &&
            buffer->data   != NULL &&
            buffer->offset <  buffer->size
        );
        win32_file_clear_last_error();

        // set the pointer
        PLARGE_INTEGER file_pointer_new              = NULL;
        HANDLE         file_handle                   = file->val;
        DWORD          file_pointer_move_method      = FILE_BEGIN;
        LARGE_INTEGER  file_pointer_distance_to_move;
        file_pointer_distance_to_move.QuadPart       = buffer->cursor;
        
        const bool did_set_pointer = (bool)SetFilePointerEx(
            file_handle,
            file_pointer_distance_to_move,
            file_pointer_new,
            file_pointer_move_method
        );
        if (!did_set_pointer) {
            win32_file_set_last_error();
            return(OS_FILE_SIZE_INVALID);
        }

        // do the write
        LPOVERLAPPED file_write_overlapped     = NULL;
        LPVOID       file_write_buffer         = (LPVOID)(buffer->data + buffer->offset);
        DWORD        file_write_size_requested = (buffer->size - buffer->offset); 
        DWORD        file_write_size_actual    = 0;

        const bool  did_write = (bool)ReadFile(
            file_handle,
            file_write_buffer,
            file_write_size_requested,
            &file_write_size_actual,
            file_write_overlapped
        );

        // return the bytes read
        if (!did_write) {
            win32_file_set_last_error();
            file_write_size_actual = OS_FILE_SIZE_INVALID;
        }
        return(file_write_size_actual);
    }

    //-------------------------------------------------------------------
    // ASYNC METHODS
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC bool
    win32_file_async_create(
        os_file_handle* file,
        os_file_async*  async) {

        assert(file != NULL && async != NULL);
        win32_file_clear_last_error();

        async->timeout_ms = 0;

        // initialize the overlapped structure
        LPOVERLAPPED overlapped = win32_file_get_overlapped(async);    
        ZeroMemory(overlapped, sizeof(OVERLAPPED));

        // create the handle for file events        
        LPSECURITY_ATTRIBUTES event_attributes    = NULL;
        BOOL                  event_manual_reset  = TRUE;
        BOOL                  event_initial_state = FALSE;
        LPCSTR                event_name          = NULL;
        overlapped->hEvent = CreateEvent(
            event_attributes,
            event_manual_reset,
            event_initial_state,
            event_name);
        
        const bool did_create = (overlapped->hEvent != NULL);
        if (!did_create) {
            win32_file_set_last_error();
        }

        return(did_create);
    }

    SLD_API_OS_FUNC bool
    win32_file_async_destroy(
        os_file_handle* file,
        os_file_async*  async) {

        assert(file != NULL && async != NULL);
        win32_file_clear_last_error();

        LPOVERLAPPED overlapped = win32_file_get_overlapped(async);    
        ZeroMemory(overlapped, sizeof(OVERLAPPED));

        const bool did_destroy = (bool)CloseHandle(overlapped->hEvent);
        if (!did_destroy) {
            win32_file_set_last_error();
        }

        return(did_destroy);
    }

    SLD_API_OS_FUNC bool
    win32_file_async_read(
        os_file_handle* file,
        os_file_async*  async,
        os_file_buffer* buffer) {

        assert(
            file           != NULL &&
            async          != NULL &&
            buffer         != NULL &&            
            buffer->offset <  buffer->size
        );
        win32_file_clear_last_error();

        HANDLE       read_handle            = (HANDLE)file->val; 
        LPOVERLAPPED read_overlapped        = win32_file_get_overlapped(async);
        LPDWORD      read_bytes_transferred = NULL; // null because overlapped is tracking io
        LPVOID       read_data              = (LPVOID)(((addr)buffer->data) + buffer->offset);
        DWORD        read_size              = (buffer->size - buffer->offset);
        read_overlapped->Offset = buffer->cursor;

        const bool did_read = ReadFile(
            read_handle,
            read_data,
            read_size,
            read_bytes_transferred,
            read_overlapped
        );

        if (!did_read) {
            win32_file_set_last_error();
        }
        return(did_read);
    }
    
    SLD_API_OS_FUNC bool
    win32_file_async_write(
        os_file_handle* file,
        os_file_async*  async,
        os_file_buffer* buffer) {

        assert(
            file           != NULL &&
            async          != NULL &&
            buffer         != NULL &&            
            buffer->offset <  buffer->size
        );
        win32_file_clear_last_error();

        HANDLE       write_handle            = (HANDLE)file->val; 
        LPOVERLAPPED write_overlapped        = win32_file_get_overlapped(async);
        LPDWORD      write_bytes_transferred = NULL; // null because overlapped is tracking io
        LPVOID       write_data              = (LPVOID)(((addr)buffer->data) + buffer->offset);
        DWORD        write_size              = (buffer->size - buffer->offset);
        write_overlapped->Offset = buffer->cursor;

        const bool did_read = WriteFile(
            write_handle,
            write_data,
            write_size,
            write_bytes_transferred,
            write_overlapped
        );

        async->state.val = os_file_async_state_e_success;
        if (!did_read) {

            win32_file_set_last_error();
            async->state.val = (_file_last_error.val == os_file_error_e_io_pending)
                ? os_file_async_state_e_pending  
                : os_file_async_state_e_error;
        }
        return(did_read);
    }

    SLD_API_OS_FUNC u64
    win32_file_async_get_result(
        os_file_handle* file,
        os_file_async*  async) {

        assert(file != NULL && async != NULL);

        win32_file_clear_last_error();

        HANDLE       overlapped_file_handle       = file->val; 
        LPOVERLAPPED overlapped                   = win32_file_get_overlapped(async);
        DWORD        overlapped_bytes_transferred = 0;
        BOOL         overlapped_wait              = FALSE;
        BOOL         overlapped_result            = GetOverlappedResult(
            overlapped_file_handle,
            overlapped,
            &overlapped_bytes_transferred,
            overlapped_wait
        );

        if (!overlapped_result) {
            win32_file_set_last_error();
            async->state.val = (_file_last_error.val == os_file_error_e_io_pending)
                ? os_file_async_state_e_pending  
                : os_file_async_state_e_error;
            overlapped_bytes_transferred = OS_FILE_SIZE_INVALID;
        }
        return(overlapped_bytes_transferred);
    }

    SLD_API_OS_FUNC u64
    win32_file_async_wait(
        os_file_handle* file,
        os_file_async*  async) {

        assert(file != NULL && async != NULL);

        win32_file_clear_last_error();

        LPOVERLAPPED overlapped                   = win32_file_get_overlapped(async);
        DWORD        overlapped_bytes_transferred = 0;

        HANDLE event_handle      = overlapped->hEvent;
        DWORD  event_timeout_ms  = async->timeout_ms; 
        DWORD  event_wait_result = WaitForSingleObject(
            event_handle,
            event_timeout_ms            
        );

        switch (event_wait_result) {

            // io complete
            case (WAIT_OBJECT_0): {
                HANDLE       overlapped_file_handle = file->val; 
                LPOVERLAPPED overlapped             = win32_file_get_overlapped(async);
                BOOL         overlapped_wait        = FALSE;
                BOOL         overlapped_result      = GetOverlappedResult(
                    overlapped_file_handle,
                    overlapped,
                    &overlapped_bytes_transferred,
                    overlapped_wait
                );
                async->state.val = os_file_async_state_e_success;

                if (!overlapped_result) {
                    win32_file_set_last_error();
                    overlapped_bytes_transferred = OS_FILE_SIZE_INVALID;
                    async->state.val = os_file_async_state_e_error;
                }
            } break;

            // io timeout
            case (WAIT_TIMEOUT): {
                win32_file_set_last_error();
                overlapped_bytes_transferred = OS_FILE_SIZE_INVALID;
                async->state.val             = os_file_async_state_e_timeout;
            } break;

            // io error
            default: {
                win32_file_set_last_error();
                overlapped_bytes_transferred = OS_FILE_SIZE_INVALID;
                async->state.val             = os_file_async_state_e_error;
            } break;
        }

        return(overlapped_bytes_transferred);
    }

    SLD_API_OS_FUNC bool
    win32_file_async_cancel(
        os_file_handle* file,
        os_file_async*  async) {

        assert(file != NULL && async != NULL);

        win32_file_clear_last_error();

        HANDLE       overlapped_file_handle = file->val;
        LPOVERLAPPED overlapped             = win32_file_get_overlapped(async);
        bool         did_cancel             = (bool)CancelIoEx(
            overlapped_file_handle,
            overlapped
        );

        async->state.val = os_file_async_state_e_success;
        if (!did_cancel) {
            win32_file_set_last_error();
            async->state.val = os_file_async_state_e_error;
        }
        return(did_cancel);
    }

    //-------------------------------------------------------------------
    // BUFFER METHODS
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC bool
    win32_file_mapped_buffer_create(
        os_file_handle*               file_hnd,
        os_file_map_handle*           map_hnd,
        os_file_buffer*               buffer) {

        assert(file_hnd != NULL && map_hnd != NULL && buffer != NULL);
        win32_file_clear_last_error();

        // get the file size
        HANDLE        file_handle = (HANDLE)file_hnd->val;
        LARGE_INTEGER file_size   = {0};
        if (!GetFileSizeEx(file_handle, &file_size)) {
            win32_file_set_last_error();
            return(false);
        }

        // align size by the granularity
        static const u64 granularity = win32_file_get_buffer_granularity();
        file_size.QuadPart           = size_align_pow_2(file_size.QuadPart, granularity);

        // set the end of the file
        static const PLARGE_INTEGER file_pointer_new         = NULL;
        static const DWORD          file_pointer_move_method = FILE_BEGIN; 
        (void)SetFilePointerEx(
            file_handle,
            file_size,
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
        if (!map_hnd->val) {
            win32_file_set_last_error();
            return(false);
        } 

        // map the file to the buffer
        static const DWORD file_data_access = FILE_MAP_ALL_ACCESS;
        static const DWORD file_offset_high = 0;
        static const DWORD file_offset_low  = 0;
        SIZE_T             file_map_size    = file_size.QuadPart;
        PVOID file_data = MapViewOfFile(
            file_map_handle,
            file_data_access,
            file_offset_high,
            file_offset_low,
            file_map_size
        );

        if (!file_data) {
            win32_file_set_last_error();
            assert(CloseHandle(file_map_handle));
            return(false);
        }

        // set properties 
        map_hnd->val      = file_map_handle;

        buffer->data      = (byte*)file_data;
        buffer->size      = file_map_size;
        buffer->offset    = 0;
        buffer->cursor    = 0;
        
        return(true);
    }

    SLD_API_OS_FUNC bool
    win32_file_mapped_buffer_destroy(
        os_file_handle*     file_hnd,
        os_file_map_handle* map_hnd,
        os_file_buffer*     buffer) {

        assert(file_hnd != NULL && map_hnd != NULL && buffer != NULL);

        win32_file_clear_last_error();

        // unmap the buffer
        const bool is_unmapped = (bool)UnmapViewOfFile(buffer->data);
        if (!is_unmapped) {
            win32_file_set_last_error();
            return(is_unmapped);
        }
        buffer->data = NULL;

        // close the map
        const bool is_closed = (bool)CloseHandle(map_hnd->val);
        if (!is_closed) {
            win32_file_set_last_error();
            return(is_closed);
        }
        map_hnd->val = NULL;
        
        buffer->size      = 0;
        buffer->offset    = 0;
        buffer->cursor    = 0;
        return(is_unmapped);
    }

    SLD_API_OS_FUNC bool
    win32_file_mapped_buffer_read(
        os_file_handle*     file_hnd,
        os_file_map_handle* map_hnd,
        os_file_buffer*     buffer) {

        assert(
            file_hnd != NULL &&
            map_hnd  != NULL &&
            buffer   != NULL
        );
        win32_file_clear_last_error();
        
        // get the current file size
        HANDLE        file_handle = file_hnd->val;
        LARGE_INTEGER file_size   = {0};
        if (!GetFileSizeEx(file_handle, &file_size)) {
            win32_file_set_last_error();
            return(false);
        }

        // check to see if we need to remap
        const bool need_remap = (
            file_size.QuadPart != buffer->size   &&
            file_size.QuadPart <  buffer->cursor &&
            buffer->cursor     >  buffer->size
        );
        if (need_remap) {

            // unmap the buffer
            const bool is_unmapped = (bool)UnmapViewOfFile(buffer->data);
            if (!is_unmapped) {
                win32_file_set_last_error();
                return(is_unmapped);
            }
            buffer->data = NULL;

            // close the map
            const bool is_closed = (bool)CloseHandle(map_hnd->val);
            if (!is_closed) {
                win32_file_set_last_error();
                return(is_closed);
            }

            // align size by the granularity
            static const u64 granularity = win32_file_get_buffer_granularity();
            file_size.QuadPart           = size_align_pow_2(file_size.QuadPart, granularity);

            // set the end of the file
            static const PLARGE_INTEGER file_pointer_new         = NULL;
            static const DWORD          file_pointer_move_method = FILE_BEGIN; 
            (void)SetFilePointerEx(
                file_handle,
                file_size,
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
        }
        
        // map the file to the buffer
        HANDLE             file_map_handle  = map_hnd->val;
        static const DWORD file_data_access = FILE_MAP_ALL_ACCESS;
        static const DWORD file_offset_high = 0;
        static const DWORD file_offset_low  = 0;
        SIZE_T             file_map_size    = file_size.QuadPart;
        PVOID file_data = MapViewOfFile(
            file_map_handle,
            file_data_access,
            file_offset_high,
            file_offset_low,
            file_map_size
        );

        if (!file_data) {
            win32_file_set_last_error();
            assert(CloseHandle(file_map_handle));
            return(false);
        }

        // set the properties
        map_hnd->val   = file_map_handle;
        buffer->data   = (byte*)file_data;
        buffer->size   = file_map_size;
        buffer->offset = 0;
        buffer->cursor = 0;
        return(true);
    }
    
    SLD_API_OS_FUNC bool
    win32_file_mapped_buffer_write(
        os_file_handle*     file_hnd,
        os_file_map_handle* map_hnd,
        os_file_buffer*     buffer) {

        assert(
            file_hnd != NULL &&
            map_hnd  != NULL &&
            buffer   != NULL
        );
        win32_file_clear_last_error();

        // flush the view of the file
        const bool did_flush_view = (bool)FlushViewOfFile(
            (LPVOID)buffer->data,
            buffer->size);
        if (!did_flush_view) {
            win32_file_set_last_error();
            return(did_flush_view);
        }

        // flush the file buffers
        const bool did_flush_buffers = (bool)FlushFileBuffers(file_hnd->val);
        if (!did_flush_buffers) {
            win32_file_set_last_error();
            return(did_flush_buffers);
        }

        return(true);        
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_API_OS_INTERNAL void
    win32_file_set_last_error(
        void) {

        const DWORD win32_error = GetLastError();

        switch (win32_error) {
            case (ERROR_SUCCESS):              { _file_last_error.val = os_file_error_e_success;             }
            case (ERROR_INVALID_PARAMETER):    { _file_last_error.val = os_file_error_e_invalid_args;        }
            case (ERROR_INVALID_HANDLE):       { _file_last_error.val = os_file_error_e_invalid_handle;      }
            case (ERROR_SECTOR_NOT_FOUND):     { _file_last_error.val = os_file_error_e_invalid_disk;        }
            case (ERROR_DEVICE_NOT_CONNECTED): { _file_last_error.val = os_file_error_e_invalid_device;      }
            case (ERROR_INVALID_USER_BUFFER):  { _file_last_error.val = os_file_error_e_invalid_buffer;      }
            case (ERROR_FILE_INVALID):         { _file_last_error.val = os_file_error_e_invalid_file;        }
            case (ERROR_SHARING_VIOLATION):    { _file_last_error.val = os_file_error_e_sharing_violation;   }
            case (ERROR_ALREADY_EXISTS):       { _file_last_error.val = os_file_error_e_already_exists;      }
            case (ERROR_FILE_EXISTS):          { _file_last_error.val = os_file_error_e_already_exists;      }
            case (ERROR_FILE_NOT_FOUND):       { _file_last_error.val = os_file_error_e_not_found;           }
            case (ERROR_ACCESS_DENIED):        { _file_last_error.val = os_file_error_e_access_denied;       }
            case (ERROR_PIPE_BUSY):            { _file_last_error.val = os_file_error_e_pipe_busy;           }
            case (ERROR_HANDLE_EOF):           { _file_last_error.val = os_file_error_e_reached_end_of_file; }
            case (ERROR_BROKEN_PIPE):          { _file_last_error.val = os_file_error_e_broken_pipe;         }
            case (ERROR_NO_DATA):              { _file_last_error.val = os_file_error_e_no_data;             }
            case (ERROR_MORE_DATA):            { _file_last_error.val = os_file_error_e_more_data;           }
            case (ERROR_IO_INCOMPLETE):        { _file_last_error.val = os_file_error_e_io_incomplete;       }
            case (ERROR_IO_PENDING):           { _file_last_error.val = os_file_error_e_io_pending;          }
            case (ERROR_OPERATION_ABORTED):    { _file_last_error.val = os_file_error_e_operation_aborted;   }
            case (ERROR_CRC):                  { _file_last_error.val = os_file_error_e_disk_io_failure;     }
            case (ERROR_DISK_CORRUPT):         { _file_last_error.val = os_file_error_e_disk_corrupt;        }
            case (ERROR_NOT_READY):            { _file_last_error.val = os_file_error_e_device_not_ready;    }
            case (ERROR_GEN_FAILURE):          { _file_last_error.val = os_file_error_e_device_failure;      }
            case (ERROR_NOT_ENOUGH_MEMORY):    { _file_last_error.val = os_file_error_e_out_of_memory;       }
            default:                           { _file_last_error.val = os_file_error_e_unknown;             }
        }
    }

    SLD_API_OS_INTERNAL void
    win32_file_clear_last_error(
        void) {
        
        _file_last_error.val = os_file_error_e_success;
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
        os_file_async* async) {

        LPOVERLAPPED overlapped = (LPOVERLAPPED)async->data;
        assert(overlapped);
        return(overlapped);
    }

    //-------------------------------------------------------------------
    // FUNCTION POINTERS
    //-------------------------------------------------------------------


};