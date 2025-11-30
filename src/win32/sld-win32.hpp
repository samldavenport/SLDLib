#ifndef SLD_WIN32_HPP
#define SLD_WIN32_HPP

#include <Windows.h>

namespace sld {

    LRESULT CALLBACK win32_window_callback_base      (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    LRESULT CALLBACK win32_window_callback_imgui    (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);

    void win32_window_init_class_base      (LPWNDCLASSA window_class);
    void win32_window_init_class_opengl    (LPWNDCLASSA window_class);
    void win32_window_init_class_directx12 (LPWNDCLASSA window_class);
};

#endif //SLD_WIN32_HPP