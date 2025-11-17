#pragma once

#include <Windows.h>

#include "sld-os.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    // directx12
    const os_window_error_t  win32_window_directx12_update       (const os_window_handle_t window_handle, os_window_update_t&   update);
    const os_window_error_t  win32_window_directx12_swap_buffers (const os_window_handle_t window_handle);
    LPWNDCLASSA              win32_window_directx12_get_class    (void);
    LRESULT CALLBACK         win32_window_directx12_callback     (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    ImGuiContext*            win32_window_directx12_imgui_init   (HWND handle);

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC const os_window_error_t
    win32_window_directx12_create(
        os_window_handle_t&         window_handle,
        const c8*                   title,
        const os_window_size_t&     size,
        const os_window_pos_t& position) {

        os_window_error_t error = {os_window_error_e_success};
        return(error);
    }

    SLD_API_OS_FUNC const os_window_error_t
    win32_window_directx12_update(
        const os_window_handle_t window_handle,
        os_window_update_t&      update) {

    }
    
    SLD_API_OS_FUNC inline const os_window_error_t
    win32_window_directx12_swap_buffers(
        const os_window_handle_t window_handle) {

    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_API_OS_INTERNAL LPWNDCLASSA 
    win32_window_directx12_get_class(
        void) {

        static WNDCLASSA   window_class;
        static LPWNDCLASSA window_class_ptr = NULL;

        if (!window_class_ptr) {

            // register the window class
            window_class.lpfnWndProc   = (WNDPROC)win32_window_callback_opengl3;
            window_class.hInstance     = GetModuleHandle(NULL);  
            window_class.lpszClassName = "SLD Win32 DirectX12 Window";
            window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

            const DWORD result = RegisterClass(&window_class);
            window_class_ptr   = (result == ERROR_SUCCESS)
                ? &window_class 
                : NULL;                
            }
        }

        return(window_class_ptr);
    }
    

    SLD_API_OS_INTERNAL LRESULT CALLBACK
    win32_window_directx12_callback(
        HWND   handle,
        UINT   message,
        WPARAM w_param,
        LPARAM l_param) {

        LRESULT result = ImGui_ImplWin32_WndProcHandler(
            handle,
            message,
            w_param,
            l_param            
        );

        if (result) return(true);

        result = DefWindowProc(
            handle,
            message,
            w_param,
            l_param
        );

        return(result);
    }
};