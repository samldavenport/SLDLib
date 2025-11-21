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

    bool                      win32_window_opengl3_update       (os_window_handle* window, os_window_events*  update);
    bool                      win32_window_opengl3_swap_buffers (os_window_handle* window);
    LPWNDCLASSA               win32_window_opengl3_get_class    (void);
    LRESULT CALLBACK          win32_window_opengl3_callback     (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    ImGuiContext*             win32_window_opengl3_imgui_init   (HWND handle);

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC bool
    win32_window_opengl3_create(
        os_window_handle*     window,
        const cchar*            title,
        const os_window_size* size,
        const os_window_pos*  position) {

        assert(
            window   != NULL &&
            title    != NULL &&
            size     != NULL &&
            position != NULL            
        );
        win32_window_clear_last_error();

        // get the class
        LPWNDCLASSA window_class = win32_window_opengl3_get_class();
        if (!window_class) {
            win32_window_set_last_error();            
            return(false);
        }

        // create the window
        HWND window_handle = CreateWindowA(
            window_class->lpszClassName,
            (LPCSTR)title,
            WS_OVERLAPPEDWINDOW,
            position->x,
            position->y,
            size->width,
            size->height,
            NULL,
            NULL,
            window_class->hInstance,
            NULL
        );
        if (!window_handle) {
            win32_window_set_last_error();            
            return(false);
        }

        // get the device context
        const HDC device_context = GetDC(window_handle);
        if (!device_context) {
            win32_window_set_last_error();            
            assert(CloseWindow(window_handle));
            return(false);
        }

        // init opengl
        const HGLRC gl_context = win32_opengl_init(device_context);
        if (!gl_context) {
            win32_window_set_last_error();            
            assert(CloseWindow (window_handle));
            assert(ReleaseDC   (window_handle, device_context));
            return(false);            
        }

        // init imgui
        const bool imgui_is_init = win32_imgui_init_opengl3(window_handle);
        if (!imgui_is_init) {
            win32_window_set_last_error();            
            assert (CloseWindow      (window_handle));
            assert (ReleaseDC        (window_handle, device_context));
            assert (wglDeleteContext (gl_context));
            return(false);            
        }
        
        window->val = window_handle;
        return(true);
    }

    SLD_API_OS_FUNC bool 
    win32_window_opengl3_set_viewport(
        os_window_handle*     window,
        const os_window_size* size,
        const os_window_pos*  position) {

        assert(
            window   != NULL &&
            size     != NULL &&
            position != NULL
        );

        glViewport(
            position->x,
            position->y,
            size->width,
            size->height);

        return(true);
    }

    SLD_API_OS_FUNC bool 
    win32_window_opengl3_set_clear_color(
        os_window_handle*      window,
        const os_window_color* color) {

        assert(window != NULL && color != NULL);

        color_f128_t normalized;
        color_u32_normalize(color, &normalized);

        glClearColor(
            normalized.r,
            normalized.g,
            normalized.b,
            normalized.a
        );

        return(true);
    }

    SLD_API_OS_FUNC bool 
    win32_window_opengl3_update(
        os_window_handle* window,
        os_window_events* update) {

        assert(window != NULL && update != NULL);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        win32_window_process_events(window, update); 
        return(true);
    }
    
    SLD_API_OS_FUNC bool 
    win32_window_opengl3_swap_buffers(
        os_window_handle* window) {

        assert(window);
        win32_window_clear_last_error();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        auto       window_handle  = (HWND)window->val;
        HDC        device_context = GetDC       (window_handle);
        const bool result         = SwapBuffers (device_context);
        if (!result) {
            win32_window_set_last_error();
            return(false);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        return(result);
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