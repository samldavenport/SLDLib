#pragma once

#include "sld-os.hpp"

#include "sld-win32-file-async.cpp"
#include "sld-win32-file-buffer.cpp"
#include "sld-win32-file-internal.cpp"
#include "sld-win32-file.cpp"

#include "sld-win32-system.cpp"
#include "sld-win32-memory.cpp"
#include "sld-win32-window.cpp"
#include "sld-win32-thread.cpp"
#include "sld-win32-monitor.cpp"

#if (SLD_OS_WINDOW_GRAPHICS_CONTEXT == os_window_graphics_context_e_opengl)
#   include "sld-win32-opengl3.cpp"
#   if (SLD_OS_WINDOW_GUI_CONTEXT == os_window_gui_context_e_imgui)
#       include "sld-win32-imgui.cpp"
#   endif
#endif

#include "sld-win32-window-opengl3.cpp"