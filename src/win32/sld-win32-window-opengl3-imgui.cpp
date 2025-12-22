#pragma once

#include "sld-win32.hpp"

#include <GL/wglew.h>
#include <GL/wgl.h>
#include <GL/wglext.h>

extern IMGUI_IMPL_API LRESULT 
ImGui_ImplWin32_WndProcHandler(
    HWND   hWnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam);

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    LRESULT CALLBACK
    win32_window_callback(
        HWND   handle,
        UINT   message,
        WPARAM w_param,
        LPARAM l_param);

    LPWNDCLASSA   win32_window_get_class    (void);
    
    //-------------------------------------------------------------------
    // API METHODS
    //-------------------------------------------------------------------

    SLD_API_OS_FUNC os_window_handle
    win32_window_create(
        const os_window_config* config) {

        assert(config != NULL);
        win32_window_clear_last_error();

        // get the class
        LPWNDCLASSA window_class = win32_window_get_class();
        if (!window_class) {
            win32_window_set_last_error();            
            return(false);
        }

        // create the window
        HWND window_handle = CreateWindowA(
            window_class->lpszClassName,
            (LPCSTR)config->title,
            WS_OVERLAPPEDWINDOW,
            config->position.x,
            config->position.y,
            config->size.width,
            config->size.height,
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


        // init imgui
        // const ImGuiContext* imgui_is_init = win32_window_init_imgui(window_handle);
        // if (!imgui_is_init) {
        //     win32_window_set_last_error();            
        //     assert (CloseWindow      (window_handle));
        //     assert (ReleaseDC        (window_handle, device_context));
        //     return(false);            
        // }
        
        return((os_window_handle)window_handle);
    }

    SLD_API_OS os_window_gl_context
    win32_window_init_opengl(
        const os_window_handle window) {

        // get the device context
        assert(window);
        const HWND window_handle  = (HWND)window;
        const HDC  device_context = GetDC(window_handle);

        //set our preferred format descriptor
        PIXELFORMATDESCRIPTOR preferred_format_descriptor = {0};
        preferred_format_descriptor.nSize      = sizeof(preferred_format_descriptor);
        preferred_format_descriptor.nVersion   = 1;
        preferred_format_descriptor.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        preferred_format_descriptor.iPixelType = PFD_TYPE_RGBA;
        preferred_format_descriptor.cColorBits = 32;
        
        // set the pixel format
        const s32   chosen_pixel_format   = ChoosePixelFormat (device_context, &preferred_format_descriptor);
        const bool  is_pixel_format_set   = SetPixelFormat    (device_context, chosen_pixel_format, &preferred_format_descriptor);
        
        // create a dummy context so we can initialize glew
        const HGLRC gl_context_dummy            = wglCreateContext (device_context);
        const bool  gl_context_dummy_is_current = wglMakeCurrent   (device_context, gl_context_dummy); 

        // initialize glew
        glewExperimental = true;
        const bool glew_is_init = (glewInit() == GLEW_OK); 
        assert(
            gl_context_dummy != NULL    &&
            gl_context_dummy_is_current &&
            glew_is_init
        );

        // create actual context
        const s32   wgl_attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB,
            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        const HGLRC gl_context_actual            = wglCreateContextAttribsARB (device_context, NULL, wgl_attribs);
        const bool  gl_context_actual_is_current = wglMakeCurrent             (device_context, gl_context_actual);
        const bool  gl_context_dummy_is_deleted  = wglDeleteContext           (gl_context_dummy);
        assert(
            gl_context_actual != NULL    &&
            gl_context_actual_is_current &&
            gl_context_dummy_is_deleted
        );
        return((os_window_gl_context)gl_context_actual);
    }

    SLD_API_OS_FUNC ImGuiContext*
    win32_window_init_imgui(
        const os_window_handle window_handle) {

        const HWND win32_window = (HWND)window_handle;
        // initialize context
        ImGuiContext* imgui_context = IMGUI_CHECKVERSION() ? ImGui::CreateContext() : NULL;  
        if (imgui_context) {

            // initialize win32/opengl methods
            bool did_init_impl = true;
            did_init_impl &= ImGui_ImplWin32_InitForOpenGL (window_handle);
            did_init_impl &= ImGui_ImplOpenGL3_Init        ("#version 330");

            if (!did_init_impl) {
                ImGui::DestroyContext(imgui_context);
                imgui_context = NULL;
            }
        }

        return(imgui_context);        
    }


    SLD_API_OS_FUNC bool 
    win32_window_frame_start(
        const os_window_handle window) {

        assert(window != NULL);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        return(true);
    }
    
    SLD_API_OS_FUNC bool 
    win32_window_frame_render(
        const os_window_handle window) {

        assert(window);
        win32_window_clear_last_error();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        HDC        device_context = GetDC       ((HWND)window);
        const bool result         = SwapBuffers (device_context);
        if (!result) {
            win32_window_set_last_error();
            return(false);
        }

        return(result);
    }

    //-------------------------------------------------------------------
    // INTERNAL METHODS
    //-------------------------------------------------------------------

    SLD_API_OS_INTERNAL LPWNDCLASSA
    win32_window_get_class(
        void) {

        static WNDCLASSA   window_class;
        static LPWNDCLASSA window_class_ptr = NULL;

        if (!window_class_ptr) {

            // register the window class
            window_class.lpfnWndProc   = (WNDPROC)win32_window_callback;
            window_class.hInstance     = GetModuleHandle(NULL);  
            window_class.lpszClassName = "SLD Win32 Opengl3 ImGui Window";
            window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

            const DWORD result = RegisterClass(&window_class);
            window_class_ptr   = (result == 0)
                ? NULL 
                : &window_class;                
        }

        return(window_class_ptr);
    }
    
    SLD_API_OS_INTERNAL LRESULT CALLBACK
    win32_window_callback(
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