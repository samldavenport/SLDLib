#pragma once

#include "sld-win32.hpp"

namespace sld {

    LRESULT CALLBACK
    win32_window_callback_base(
        HWND   handle,
        UINT   message,
        WPARAM w_param,
        LPARAM l_param) {

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
    }

    LRESULT CALLBACK
    win32_window_callback_imgui(
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

        result = win32_window_callback_base(
            handle,
            message,
            w_param,
            l_param            
        );

        return(result);
    }
};