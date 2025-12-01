#pragma once

#include "sld-win32.hpp"
#include "sld-os-file.hpp"

namespace sld {

    SLD_API_OS_FUNC bool
    win32_file_mapped_buffer_create(
        const os_file_handle   file_hnd,
        os_file_mapped_buffer* buffer) {

        assert(file_hnd != NULL && buffer != NULL);
        win32_file_clear_last_error();

        // get the file size
        LARGE_INTEGER file_size   = {0};
        if (!GetFileSizeEx(file_hnd, &file_size)) {
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
            file_hnd,
            file_size,
            file_pointer_new,
            file_pointer_move_method
        );
        (void)SetEndOfFile(file_hnd);

        // create the map
        static const LPSECURITY_ATTRIBUTES file_map_attributes    = NULL;
        static const DWORD                 file_map_protect       = FILE_MAP_ALL_ACCESS;
        static const DWORD                 file_map_max_size_high = 0;
        static const DWORD                 file_map_max_size_low  = 0;
        static const LPCSTR                file_map_name          = NULL;
        HANDLE file_map_handle = CreateFileMapping(
            file_hnd,
            file_map_attributes,
            file_map_protect,
            file_map_max_size_high,
            file_map_max_size_low,
            file_map_name
        );
        if (file_hnd) {
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
        buffer->map_handle = file_map_handle;
        buffer->data       = (byte*)file_data;
        buffer->size       = file_map_size;
        buffer->offset     = 0;
        buffer->cursor     = 0;
        
        return((os_file_map_handle)file_map_handle);
    }

    SLD_API_OS_FUNC bool
    win32_file_mapped_buffer_destroy(
        const os_file_handle     file_hnd,
        os_file_mapped_buffer*   buffer) {

        assert(
            file_hnd != NULL &&
            buffer   != NULL
        );

        win32_file_clear_last_error();

        // unmap the buffer
        const bool is_unmapped = (bool)UnmapViewOfFile(buffer->data);
        if (!is_unmapped) {
            win32_file_set_last_error();
            return(is_unmapped);
        }
        buffer->data = NULL;

        // close the map
        const bool is_closed = (bool)CloseHandle(buffer->map_handle);
        if (!is_closed) {
            win32_file_set_last_error();
            return(is_closed);
        }

        buffer->map_handle = NULL;        
        buffer->size       = 0;
        buffer->offset     = 0;
        buffer->cursor     = 0;
        return(is_unmapped);
    }

    SLD_API_OS_FUNC bool
    win32_file_mapped_buffer_read(
        const os_file_handle   file_hnd,
        os_file_mapped_buffer* buffer) {

        assert(
            file_hnd != NULL &&
            buffer   != NULL
        );
        win32_file_clear_last_error();
        
        // get the current file size
        HANDLE        file_handle = file_hnd;
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
            const bool is_closed = (bool)CloseHandle(buffer->map_handle);
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
        HANDLE             file_map_handle  = buffer->map_handle;
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
        buffer->map_handle = file_map_handle;
        buffer->data       = (byte*)file_data;
        buffer->size       = file_map_size;
        buffer->offset     = 0;
        buffer->cursor     = 0;
        return(true);
    }
    
    SLD_API_OS_FUNC bool
    win32_file_mapped_buffer_write(
        const os_file_handle   file_hnd,
        os_file_mapped_buffer* buffer) {

        assert(
            file_hnd != NULL &&
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
        const bool did_flush_buffers = (bool)FlushFileBuffers(file_hnd);
        if (!did_flush_buffers) {
            win32_file_set_last_error();
            return(did_flush_buffers);
        }

        return(true);        
    }
};