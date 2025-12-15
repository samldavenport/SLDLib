#pragma once

#include <Windows.h>
#include "sld-os.hpp"
#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // API METHODS
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC os_file_handle
    win32_file_open(
        const os_file_config* config) {

        constexpr u32 create_mode_count = 4;
        assert(
            config && 
            config->mode < create_mode_count
        );
        win32_file_clear_last_error();

        LPSECURITY_ATTRIBUTES security        = NULL;
        HANDLE                template_handle = NULL;
        DWORD                 access          = 0;
        DWORD                 share           = 0;
        DWORD                 mode            = 0;
        DWORD                 flags           = FILE_ATTRIBUTE_NORMAL;

        // access
        access |= (config->access_flags.test(os_file_access_flag_read))  ? GENERIC_READ  : 0;
        access |= (config->access_flags.test(os_file_access_flag_write)) ? GENERIC_WRITE : 0;

        // share        
        share |= (config->share_flags.test(os_file_share_flag_read))   ? FILE_SHARE_READ   : 0;
        share |= (config->share_flags.test(os_file_share_flag_write))  ? FILE_SHARE_WRITE  : 0;
        share |= (config->share_flags.test(os_file_share_flag_delete)) ? FILE_SHARE_DELETE : 0;

        // mode
        constexpr DWORD create_mode_array[create_mode_count] = {
            CREATE_NEW,    // os_file_mode_create_new 
            OPEN_EXISTING, // os_file_mode_open_existing
            OPEN_ALWAYS,   // os_file_mode_open_always
            CREATE_ALWAYS  // os_file_mode_overwrite_existing
        };
        mode = create_mode_array[config->mode];

        // overlapped / async
        flags |= config->is_async ? FILE_FLAG_OVERLAPPED : 0;  

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