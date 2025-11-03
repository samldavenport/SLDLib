#pragma once

#include <Windows.h>
#include "sld-os.hpp"
#include "sld-win32-window.cpp"
#include "sld-win32-imgui.cpp"
#include "sld-win32-opengl3.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    const os_window_error_t win32_window_opengl3_update       (const os_window_handle_t window_handle, os_window_update_t&   update);
    const os_window_error_t win32_window_opengl3_swap_buffers (const os_window_handle_t window_handle);
    LPWNDCLASSA             win32_window_opengl3_get_class    (void);
    LRESULT CALLBACK        win32_window_opengl3_callback     (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    ImGuiContext*           win32_window_opengl3_imgui_init   (HWND handle);

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC const os_window_error_t
    win32_window_opengl3_create(
        os_window_handle_t&     window_handle,
        const cchar*            title,
        const os_window_size_t& size,
        const os_window_pos_t&  position) {

        os_window_error_t error = { os_window_error_e_success };

        // get the class
        LPWNDCLASSA window_class = win32_window_opengl3_get_class();
        if (!window_class) {
            error = win32_window_error_get_last();
            return(error);
        }

        // create the window
        window_handle.val = CreateWindowA(
            window_class->lpszClassName,
            (LPCSTR)title,
            WS_OVERLAPPEDWINDOW,
            position.x,
            position.y,
            size.width,
            size.height,
            NULL,
            NULL,
            window_class->hInstance,
            NULL
        );

        const HDC   device_context = GetDC((HWND)window_handle.val);
        const HGLRC gl_context     = win32_opengl_init(device_context);
        const bool  imgui_is_init  = win32_imgui_init_opengl3((HWND)window_handle.val);

        // return the error code
        error = (window_handle.val == INVALID_HANDLE_VALUE)
            ? win32_window_error_get_last ()
            : win32_window_error_success  ();
        return(error);
    }

    SLD_API_OS_FUNC const os_window_error_t 
    win32_window_opengl3_set_viewport(
        const os_window_handle_t window_handle,
        const os_window_size_t&  size,
        const os_window_pos_t&   position) {

        glViewport(
            position.x,
            position.y,
            size.width,
            size.height);

        const os_window_error_t result = { os_window_error_e_success };
        return(result);
    }

    SLD_API_OS_FUNC const os_window_error_t 
    win32_window_opengl3_set_clear_color(
        const os_window_handle_t window_handle,
        const os_window_color_t& color) {

        color_f128_t normalized;
        color_u32_normalize(color, normalized);

        glClearColor(
            normalized.r,
            normalized.g,
            normalized.b,
            normalized.a
        );

        const os_window_error_t result = { os_window_error_e_success };
        return(result);
    }

    SLD_API_OS_FUNC const os_window_error_t
    win32_window_opengl3_update(
        const os_window_handle_t window_handle,
        os_window_update_t&      update) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        const os_window_error_t error = win32_window_process_events(window_handle, update); 
        return(error);
    }
    
    SLD_API_OS_FUNC inline const os_window_error_t
    win32_window_opengl3_swap_buffers(
        const os_window_handle_t window_handle) {

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
        HDC device_context = GetDC       ((HWND)window_handle.val);
        const bool result  = SwapBuffers (device_context);

        const os_window_error_t error = (result == true)
            ? win32_window_error_success()
            : win32_window_error_get_last();

        glClear(GL_COLOR_BUFFER_BIT);

        return(error);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_API_OS_INTERNAL LPWNDCLASSA 
    win32_window_opengl3_get_class(
        void) {

        static WNDCLASSA   window_class;
        static LPWNDCLASSA window_class_ptr = NULL;

        if (!window_class_ptr) {

            // register the window class
            window_class.lpfnWndProc   = (WNDPROC)win32_window_opengl3_callback;
            window_class.hInstance     = GetModuleHandle(NULL);  
            window_class.lpszClassName = "SLD Win32 Opengl3 Window";
            window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

            const DWORD result = RegisterClass(&window_class);
            window_class_ptr   = (result == 0)
                ? NULL 
                : &window_class;                
            }

        return(window_class_ptr);
    }

    SLD_API_OS_INTERNAL LRESULT CALLBACK
    win32_window_opengl3_callback(
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

        switch(message) {
            case (WM_CLOSE): { PostQuitMessage(0); } break;
            default: break;
        };

        result = DefWindowProc(
            handle,
            message,
            w_param,
            l_param
        );

        return(result);
    }
};