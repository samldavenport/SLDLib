#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // API METHODS
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC os_file_handle
    win32_file_open(
        const os_file_config* config) {

        assert(config);
        win32_file_clear_last_error();

        LPSECURITY_ATTRIBUTES security        = NULL;
        HANDLE                template_handle = NULL;
        DWORD                 access          = 0;
        DWORD                 share           = 0;
        DWORD                 mode            = 0;
        DWORD                 flags           = FILE_ATTRIBUTE_NORMAL;

        // access
        const bool access_read  = (config->access_flags & os_file_access_flag_e_read);
        const bool access_write = (config->access_flags & os_file_access_flag_e_write);
        if (access_read)  access |= GENERIC_READ;
        if (access_write) access |= GENERIC_WRITE;

        // share        
        const bool share_read   = (config->share_flags & os_file_share_flag_e_read);
        const bool share_write  = (config->share_flags & os_file_share_flag_e_write);
        const bool share_delete = (config->share_flags & os_file_share_flag_e_delete);
        if (share_read)   share |= FILE_SHARE_READ;
        if (share_write)  share |= FILE_SHARE_WRITE;
        if (share_delete) share |= FILE_SHARE_DELETE;

        // mode
        switch (config->mode) {
            case(os_file_mode_e_create_new):         mode = CREATE_NEW;    break;
            case(os_file_mode_e_open_existing):      mode = OPEN_EXISTING; break;
            case(os_file_mode_e_open_always):        mode = OPEN_ALWAYS;   break;
            case(os_file_mode_e_overwrite_existing): mode = CREATE_ALWAYS; break;
            default:                                 mode = CREATE_NEW;    break;
        }

        if (config->is_async) {
            flags |= FILE_FLAG_OVERLAPPED;
        }

        // create file
        HANDLE win32_handle = CreateFile(
            config->path,
            access,
            share,
            security,
            mode,
            flags,
            template_handle
        );

        const bool did_create = (win32_handle != INVALID_HANDLE_VALUE);
        if (!did_create) {
            win32_file_set_last_error();
            win32_handle = OS_FILE_HANDLE_INVALID;
        }        

        return((os_file_handle)win32_handle);
    }

    SLD_API_OS_FUNC bool
    win32_file_close(
        const os_file_handle file) {

        assert(file);
        win32_file_clear_last_error();

        const bool did_close = (bool)CloseHandle(file);
        if (!did_close) {
            win32_file_set_last_error();
        }

        return(did_close);
    }

    SLD_API_OS_FUNC u64
    win32_file_get_size(
        const os_file_handle file) {

        assert(file);

        win32_file_clear_last_error();

        LARGE_INTEGER size;
        const bool did_get_size = (bool)GetFileSizeEx(file, &size);

        if (!did_get_size) {
            size.QuadPart = OS_FILE_SIZE_INVALID;
            win32_file_set_last_error();
        }
        return(size.QuadPart);
    }

    SLD_API_OS_FUNC u64 
    win32_file_read(
        const os_file_handle file,
        os_file_buffer*      buffer) {

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
        DWORD          file_pointer_move_method      = FILE_BEGIN;
        LARGE_INTEGER  file_pointer_distance_to_move;
        file_pointer_distance_to_move.QuadPart = buffer->cursor;

        const bool did_set_pointer = (bool)SetFilePointerEx(
            file,
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
            file,
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
        const os_file_handle file,
        os_file_buffer*      buffer) {

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
        DWORD          file_pointer_move_method      = FILE_BEGIN;
        LARGE_INTEGER  file_pointer_distance_to_move;
        file_pointer_distance_to_move.QuadPart       = buffer->cursor;
        
        const bool did_set_pointer = (bool)SetFilePointerEx(
            file,
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

        const bool did_write = (bool)ReadFile(
            file,
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
};