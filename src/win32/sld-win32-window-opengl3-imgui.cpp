#pragma once

#include "sld-win32.hpp"

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

    ImGuiContext* win32_window_init_imgui   (HWND window_handle);
    HGLRC         win32_window_init_opengl  (HDC  device_context);
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

        // init opengl
        const HGLRC gl_context = win32_window_init_opengl(device_context);
        if (!gl_context) {
            win32_window_set_last_error();            
            assert(CloseWindow (window_handle));
            assert(ReleaseDC   (window_handle, device_context));
            return(false);            
        }

        // init imgui
        const ImGuiContext* imgui_is_init = win32_window_init_imgui(window_handle);
        if (!imgui_is_init) {
            win32_window_set_last_error();            
            assert (CloseWindow      (window_handle));
            assert (ReleaseDC        (window_handle, device_context));
            assert (wglDeleteContext (gl_context));
            return(false);            
        }
        
        return((os_window_handle)window_handle);
    }

    SLD_API_OS_FUNC bool 
    win32_window_set_viewport(
        const os_window_handle    window,
        const os_window_viewport* viewport) {

        assert(window != NULL && viewport != NULL);

        glViewport(
            viewport->pos_x,
            viewport->pos_y,
            viewport->width,
            viewport->height);

        return(true);
    }

    SLD_API_OS_FUNC bool 
    win32_window_set_clear_color(
        const os_window_handle window,
        const os_window_color*  color) {

        assert(window != NULL && color != NULL);

        constexpr f32 normal_factor = (1.0f/255.0f);
        const     f32 normal_r      = normal_factor * ((f32)color->r);
        const     f32 normal_g      = normal_factor * ((f32)color->g);
        const     f32 normal_b      = normal_factor * ((f32)color->b);
        const     f32 normal_a      = normal_factor * ((f32)color->a);

        glClearColor(
            normal_r,
            normal_g,
            normal_b,
            normal_a
        );

        return(true);
    }

    SLD_API_OS_FUNC bool 
    win32_window_frame_start(
        const os_window_handle window) {

        assert(window != NULL);

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

        glClear(GL_COLOR_BUFFER_BIT);
        return(result);
    }

    //-------------------------------------------------------------------
    // INTERNAL METHODS
    //-------------------------------------------------------------------

    SLD_API_OS_INTERNAL ImGuiContext*
    win32_window_init_imgui(
        HWND window_handle) {

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

    SLD_API_OS_INTERNAL HGLRC
    win32_window_init_opengl(
        HDC device_context) {

        //set our preferred format descriptor
        PIXELFORMATDESCRIPTOR preferred_format_descriptor = {0};
        preferred_format_descriptor.nSize      = sizeof(preferred_format_descriptor);
        preferred_format_descriptor.nVersion   = 1;
        preferred_format_descriptor.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        preferred_format_descriptor.iPixelType = PFD_TYPE_RGBA;
        preferred_format_descriptor.cColorBits = 32;
        
        // set the pixel format and initialize the context
        const s32   chosen_pixel_format   = ChoosePixelFormat (device_context, &preferred_format_descriptor);
        const bool  is_pixel_format_set   = SetPixelFormat    (device_context, chosen_pixel_format, &preferred_format_descriptor);
        const HGLRC gl_context            = wglCreateContext  (device_context);
        const bool  is_gl_context_current = wglMakeCurrent    (device_context, gl_context);

        // if initialized, return the context
        bool result = true;
        result &= is_pixel_format_set; 
        result &= is_gl_context_current; 
        return(result ? gl_context : NULL);
    };

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