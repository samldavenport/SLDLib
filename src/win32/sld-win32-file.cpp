#pragma once

#include <Windows.h>
#include "sld-os.hpp"

#ifdef     SLD_OS_FILE_IO_SIZE
#   undef  SLD_OS_FILE_IO_SIZE
#   define SLD_OS_FILE_IO_SIZE sizeof(OVERLAPPED)
#endif

namespace sld {

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

    const os_file_error_t  win32_file_get_error_code         (const DWORD error_code);
    const os_file_error_t& win32_file_error_success          (void);
    const u64              win32_file_get_buffer_granularity (void);
    LPOVERLAPPED           win32_file_get_overlapped         (os_file_t* file);

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC const bool
    win32_file_open(
        os_file_t*              file,
        const os_file_path_t*   path,
        const os_file_config_t* config) {

        // check args
        bool can_open = (
            file         != NULL &&
            path         != NULL &&
            path->buffer != NULL &&
            config       != NULL
        );
        assert(can_open);

        win32_file_args_t file_args = {0};
        file_args.access          = 0;
        file_args.security        = NULL;
        file_args.template_handle = NULL;
        file_args.flags           = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED; 

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

        // create file
        HANDLE win32_file_handle = = CreateFile(
            (LPCSTR)path,
            file_args.access,
            file_args.share,
            file_args.security,
            file_args.mode,
            file_args.flags,
            file_args.template_handle
        );

        // set file pointer
        LARGE_INTEGER buffer_granularity;
        buffer_granularity.QuadPart = win32_file_get_buffer_granularity();
        bool did_set_pointer = true;
        did_set_pointer &= SetFilePointerEx(
            win32_file_handle,  // hFile
            buffer_granularity, // liDistanceToMove
            NULL,               // lpNewFilePointer
            FILE_BEGIN          // dwMoveMethod
        ); 
        did_set_pointer &= SetEndOfFile(win32_file_handle);
        assert(did_set_pointer);

        // create the map
        HANDLE win32_map_handle = CreateFileMapping(
            win32_file_handle, // hFile
            NULL,              // lpFileMappingAttributes
            PAGE_READWRITE,    // flProtect
            0,                 // dwMaximumSizeHigh
            0,                 // dwMaximumSizeLow
            NULL               // lpName
        );
        if (win32_map_handle == NULL) {
            const DWORD win32_error = GetLastError();
            const bool did_close    = CloseHandle(win32_file_handle);
            file->error             = win32_file_get_error_code(win32_error);
            return(false);
        }

        // map the virtual buffer
        void* buffer_memory = MapViewOfFile(
            win32_map_handle,    // hFileMappingObject
            FILE_MAP_ALL_ACCESS, // dwDesiredAccess
            0,                   // dwFileOffsetHigh
            0,                   // dwFileOffsetLow
            0                    // dwNumberOfBytesToMap
        );
        if (memory == NULL) {
            const DWORD win32_error = GetLastError();
            file->error             = win32_file_get_error_code(win32_error);
            const bool did_close    = (
                CloseHandle(win32_file_handle) &&
                CloseHandle(win32_map_handle)
            );
            assert(did_close);
            return(false);
        }   

        // zero the overlapped structure
        LPOVERLAPPED overlapped = win32_file_get_overlapped(file);    
        ZeroMemory(overlapped, sizeof(OVERLAPPED));

        // initialize the file structure and return
        file->handle.val                 = win32_file_handle;
        file->map.val                    = win32_map_handle;
        file->error                      = win32_file_error_success(); 
        file->vbuffer.data        = buffer_memory;
        file->vbuffer.size        = buffer_granularity.QuadPart;
        file->vbuffer.cursor      = 0;
        file->vbuffer.transferred = 0;
        bool did_initialize = (
            file->handle.val          != NULL &&
            file->map.val             != NULL &&
            file->vbuffer.data != NULL
        );
        assert(did_initialize);
        return(true);
    }

    SLD_API_OS_FUNC bool
    win32_file_virtual_buffer_flush(
        os_file_t*                file,
        os_file_virtual_buffer_t* virtual_buffer) {

    }

    SLD_API_OS_FUNC u64
    win32_file_get_size(
        os_file_t* file) {

        assert(file);

        static LPDWORD win32_file_size_high = NULL;
        
        LARGE_INTEGER size_large_int = {0};
        const bool    is_valid       = GetFileSizeEx((HANDLE)file->handle.val, size_large_int);
        const DWORD   win32_error    = GetLastError();

        u64 size = 0;
        if (is_valid) {
            file->error = win32_file_error_success();
            size        = (u64)size_large_int.QuadPart;               
        } 
        else {
            file->error = win32_file_get_error_code(win32_error);
            size        = OS_FILE_INVALID_SIZE 
        }

        return(size);
    }

    SLD_API_OS_FUNC const os_file_error_t
    win32_file_read(
        const os_file_handle_t handle,
        os_file_buffer_t*      buffer) {

        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(overlapped));
        overlapped.Offset = buffer->cursor;

        BOOL result = ReadFile(
            (HANDLE)handle.val,            // hFile
            (LPVOID)buffer->data,          // lpBuffer
            (DWORD)buffer->size,           // nNumberOfBytesToRead
            (LPDWORD)&buffer->transferred, // lpNumberOfBytesRead
            &overlapped                    // lpOverlapped
        );

        // get the last error
        DWORD      win32_error   = GetLastError();
        const bool io_is_pending = !result && (win32_error == ERROR_IO_PENDING); 
        
        // if its pending, try waiting
        if (io_is_pending) {
            const DWORD wait_result = WaitForSingleObject((HANDLE)handle.val, INFINITE);
            win32_error = (wait_result == WAIT_OBJECT_0) 
                ? ERROR_SUCCESS
                : GetLastError();

        }
        // otherwise, get the last error
        else {
            win32_error = result ? ERROR_SUCCESS : GetLastError(); 
        }

        // translate the error
        buffer->cursor += buffer->transferred;
        const os_file_error_t error = win32_file_get_error_code(win32_error);
        return(error);
    }

    SLD_API_OS_FUNC const os_file_error_t
    win32_file_write(
        const os_file_handle_t handle,
        os_file_buffer_t*      buffer) {

        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(overlapped));
        overlapped.Offset = buffer->cursor;

        // write the file
        const BOOL result = WriteFile(
            (HANDLE)handle.val,           // hFile,
            (LPVOID)buffer->data,          // lpBuffer,
            (DWORD)buffer->size,           // nNumberOfBytesToWrite,
            (LPDWORD)&buffer->transferred, // lpNumberOfBytesWritten,
            &overlapped                   // lpOverlapped
        );

        // get the last error
        DWORD      win32_error   = GetLastError();
        const bool io_is_pending = !result && (win32_error == ERROR_IO_PENDING); 
        
        // if its pending, try waiting
        if (io_is_pending) {
            const DWORD wait_result = WaitForSingleObject((HANDLE)handle.val, INFINITE);
            win32_error = (wait_result == WAIT_OBJECT_0) 
                ? ERROR_SUCCESS
                : GetLastError();
        }
        // otherwise, get the last error
        else {
            win32_error = result ? ERROR_SUCCESS : GetLastError(); 
        }

        buffer->cursor += buffer->transferred;

        // translate the error
        const os_file_error_t error = win32_file_get_error_code(win32_error);
        return(error);
    }

    SLD_API_OS_FUNC const os_file_error_t
    win32_file_read_async(
        const os_file_handle_t   handle,
        os_file_buffer_t*        buffer,
        os_file_async_context_t* context) {

        os_file_os_context_t* os_context = context->os;

        LPOVERLAPPED overlapped = (LPOVERLAPPED)os_context->data;
        overlapped->Pointer     = (PVOID)context->callback;
        overlapped->Offset      = buffer->cursor;

        const bool result = ReadFileEx(
            (HANDLE)handle.val,       // hFile 
            (LPVOID)buffer->data,      // lpBuffer 
            (DWORD)buffer->size,       // nNumberOfBytesToRead 
            overlapped,               // lpOverlapped 
            win32_file_async_callback // lpCompletionRoutine 
        );

        const DWORD win32_error = GetLastError();

        const os_file_error_t error = (result)
            ? win32_file_error_success()
            : win32_file_get_error_code(win32_error);
        return(error);
    }

    SLD_API_OS_FUNC const os_file_error_t
    win32_file_write_async(
        const os_file_handle_t   handle,
        os_file_buffer_t*        buffer,
        os_file_async_context_t* context) {

        os_file_os_context_t* os_context = context->os;

        LPOVERLAPPED overlapped = (LPOVERLAPPED)os_context->data;
        overlapped->Pointer     = (PVOID)context->callback;
        overlapped->Offset      = buffer->cursor;

        const bool result = WriteFileEx(
            (HANDLE)handle.val,       // hFile 
            (LPVOID)buffer->data,     // lpBuffer 
            (DWORD)buffer->size,      // nNumberOfBytesToRead 
            overlapped,               // lpOverlapped 
            win32_file_async_callback // lpCompletionRoutine 
        );

        const DWORD win32_error = GetLastError();

        const os_file_error_t error = (result)
            ? win32_file_error_success()
            : win32_file_get_error_code(win32_error);
        return(error);
    }

    SLD_API_OS_INTERNAL const os_file_error_t
    win32_file_get_working_directory(
        os_file_path_cstr_t& file_path) {

        constexpr u32 buffer_size = sizeof(os_file_path_cstr_t::buffer);
        
        DWORD result = GetCurrentDirectory(buffer_size, (LPSTR)file_path.buffer);

        const bool did_succeed = (
            result != 0 ||
            result <= buffer_size
        );

        const os_file_error_t error_code = (did_succeed)
            ? win32_file_error_success()
            : win32_file_get_error_code(result);
        
        return(error_code);
    }

    SLD_API_OS_FUNC bool
    win32_file_dialog_open(
        const cchar*             path,
        const os_window_handle_t window_handle) {

        return(false);

    }


    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_API_OS_INTERNAL const os_file_error_t
    win32_file_get_error_code(
        const DWORD win32_error) {

        os_file_error_t error;

        switch (win32_error) {
            case (ERROR_SUCCESS):              { error.val = os_file_error_e_success;             return(error); }
            case (ERROR_INVALID_PARAMETER):    { error.val = os_file_error_e_invalid_args;        return(error); }
            case (ERROR_INVALID_HANDLE):       { error.val = os_file_error_e_invalid_handle;      return(error); }
            case (ERROR_SECTOR_NOT_FOUND):     { error.val = os_file_error_e_invalid_disk;        return(error); }
            case (ERROR_DEVICE_NOT_CONNECTED): { error.val = os_file_error_e_invalid_device;      return(error); }
            case (ERROR_INVALID_USER_BUFFER):  { error.val = os_file_error_e_invalid_buffer;      return(error); }
            case (ERROR_FILE_INVALID):         { error.val = os_file_error_e_invalid_file;        return(error); }
            case (ERROR_SHARING_VIOLATION):    { error.val = os_file_error_e_sharing_violation;   return(error); }
            case (ERROR_ALREADY_EXISTS):       { error.val = os_file_error_e_already_exists;      return(error); }
            case (ERROR_FILE_EXISTS):          { error.val = os_file_error_e_already_exists;      return(error); }
            case (ERROR_FILE_NOT_FOUND):       { error.val = os_file_error_e_not_found;           return(error); }
            case (ERROR_ACCESS_DENIED):        { error.val = os_file_error_e_access_denied;       return(error); }
            case (ERROR_PIPE_BUSY):            { error.val = os_file_error_e_pipe_busy;           return(error); }
            case (ERROR_HANDLE_EOF):           { error.val = os_file_error_e_reached_end_of_file; return(error); }
            case (ERROR_BROKEN_PIPE):          { error.val = os_file_error_e_broken_pipe;         return(error); }
            case (ERROR_NO_DATA):              { error.val = os_file_error_e_no_data;             return(error); }
            case (ERROR_MORE_DATA):            { error.val = os_file_error_e_more_data;           return(error); }
            case (ERROR_IO_INCOMPLETE):        { error.val = os_file_error_e_io_incomplete;       return(error); }
            case (ERROR_IO_PENDING):           { error.val = os_file_error_e_io_pending;          return(error); }
            case (ERROR_OPERATION_ABORTED):    { error.val = os_file_error_e_operation_aborted;   return(error); }
            case (ERROR_CRC):                  { error.val = os_file_error_e_disk_io_failure;     return(error); }
            case (ERROR_DISK_CORRUPT):         { error.val = os_file_error_e_disk_corrupt;        return(error); }
            case (ERROR_NOT_READY):            { error.val = os_file_error_e_device_not_ready;    return(error); }
            case (ERROR_GEN_FAILURE):          { error.val = os_file_error_e_device_failure;      return(error); }
            case (ERROR_NOT_ENOUGH_MEMORY):    { error.val = os_file_error_e_out_of_memory;       return(error); }
            default:                           { error.val = os_file_error_e_unknown;             return(error); }
        }

        return(error);
    }

    SLD_API_OS_INTERNAL const os_file_error_t&
    win32_file_error_success(
        void) {

        static const os_file_error_t error = { os_file_error_e_success };
        return(error);
    }

    SLD_API_OS_INTERNAL void
    win32_file_async_callback(
        DWORD        error_code,
        DWORD        bytes_transferred,
        LPOVERLAPPED overlapped) {

        if (!overlapped) return;

        auto context = (os_file_callback_context_t*)overlapped->Pointer;

        const os_file_error_t error = win32_file_get_error_code(error_code);

        context->func(context->data, error, bytes_transferred);
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
        os_file_t* file) {

        LPOVERLAPPED overlapped = (LPOVERLAPPED)file->io.data;
        return(overlapped);
    }
};