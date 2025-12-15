#pragma once

#include "sld-win32.hpp"

namespace sld {

    SLD_API_OS_FUNC bool
    win32_file_async_create(
        const os_file_handle file,
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
        const os_file_handle file,
        os_file_async*       async) {

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
        const os_file_handle file,
        os_file_async*       async,
        os_file_buffer*      buffer) {

        assert(
            file           != NULL &&
            async          != NULL &&
            buffer         != NULL &&            
            buffer->offset <  buffer->size
        );
        win32_file_clear_last_error();

        HANDLE       read_handle            = (HANDLE)file; 
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
        const os_file_handle file,
        os_file_async*       async,
        os_file_buffer*      buffer) {

        assert(
            file           != NULL &&
            async          != NULL &&
            buffer         != NULL &&            
            buffer->offset <  buffer->size
        );
        win32_file_clear_last_error();

        HANDLE       write_handle            = (HANDLE)file; 
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

        async->state = os_file_async_state_success;
        if (!did_read) {

            win32_file_set_last_error();
            async->state = (_last_error_file == os_file_error_io_pending)
                ? os_file_async_state_pending  
                : os_file_async_state_error;
        }
        return(did_read);
    }

    SLD_API_OS_FUNC u64
    win32_file_async_get_result(
        const os_file_handle file,
        os_file_async*       async) {

        assert(file != NULL && async != NULL);

        win32_file_clear_last_error();

        HANDLE       overlapped_file_handle       = file; 
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
            async->state = (_last_error_file == os_file_error_io_pending)
                ? os_file_async_state_pending  
                : os_file_async_state_error;
            overlapped_bytes_transferred = OS_FILE_SIZE_INVALID;
        }
        return(overlapped_bytes_transferred);
    }

    SLD_API_OS_FUNC u64
    win32_file_async_wait(
        const os_file_handle file,
        os_file_async*       async) {

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
                HANDLE       overlapped_file_handle = file; 
                LPOVERLAPPED overlapped             = win32_file_get_overlapped(async);
                BOOL         overlapped_wait        = FALSE;
                BOOL         overlapped_result      = GetOverlappedResult(
                    overlapped_file_handle,
                    overlapped,
                    &overlapped_bytes_transferred,
                    overlapped_wait
                );
                async->state = os_file_async_state_success;

                if (!overlapped_result) {
                    win32_file_set_last_error();
                    overlapped_bytes_transferred = OS_FILE_SIZE_INVALID;
                    async->state = os_file_async_state_error;
                }
            } break;

            // io timeout
            case (WAIT_TIMEOUT): {
                win32_file_set_last_error();
                overlapped_bytes_transferred = OS_FILE_SIZE_INVALID;
                async->state             = os_file_async_state_timeout;
            } break;

            // io error
            default: {
                win32_file_set_last_error();
                overlapped_bytes_transferred = OS_FILE_SIZE_INVALID;
                async->state             = os_file_async_state_error;
            } break;
        }

        return(overlapped_bytes_transferred);
    }

    SLD_API_OS_FUNC bool
    win32_file_async_cancel(
        const os_file_handle file,
        os_file_async*       async) {

        assert(file != NULL && async != NULL);

        win32_file_clear_last_error();

        HANDLE       overlapped_file_handle = file;
        LPOVERLAPPED overlapped             = win32_file_get_overlapped(async);
        bool         did_cancel             = (bool)CancelIoEx(
            overlapped_file_handle,
            overlapped
        );

        async->state = os_file_async_state_success;
        if (!did_cancel) {
            win32_file_set_last_error();
            async->state = os_file_async_state_error;
        }
        return(did_cancel);
    }
};