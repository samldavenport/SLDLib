#pragma once

#include "sld-win32.hpp"
#include "sld-win32-input.hpp"

namespace sld {

    SLD_API_OS_FUNC os_window_error
    win32_window_get_last_error(
        void) {

        return(_last_error_window);
    }

    SLD_API_OS_FUNC bool
    win32_window_destroy(
        const os_window_handle window) {

        //TODO
        assert(window);

        return(false);
    }

    SLD_API_OS_FUNC bool
    win32_window_show(
        const os_window_handle window) {

        assert(window);

        constexpr s32 cmd_show_true  = 1;
        const auto    window_handle  = (HWND)window;

        // we don't need the value from show window
        (void)ShowWindow(window_handle, cmd_show_true);

        return(true);
    }

    SLD_API_OS_FUNC bool
    win32_window_get_size(
        const os_window_handle window,
        os_window_size*        size) {
        
        assert(window != NULL && size != NULL);
        win32_window_clear_last_error();

        const auto window_handle  = (HWND)window;
        RECT       window_rect;

        const bool result = GetWindowRect(window_handle, &window_rect);
        size->width       = (window_rect.right  - window_rect.left);  
        size->height      = (window_rect.bottom - window_rect.top);  

        if (!result) {
            win32_window_set_last_error();
        }
        return(result);
    }

    SLD_API_OS_FUNC bool
    win32_window_get_position(
        const os_window_handle window,
        os_window_position*    position) {

        assert(window != NULL && position != NULL);
        win32_window_clear_last_error();

        const auto window_handle  = (HWND)window;
        RECT       window_rect;

        const bool result = GetWindowRect(window_handle, &window_rect);
        position->x = window_rect.left;
        position->y = window_rect.top;

        if (!result) {
            win32_window_set_last_error();
        }
        return(result);
    }

    SLD_API_OS_FUNC bool
    win32_window_open_file_dialog(
        os_window_handle*             window,
        os_window_dialog*      dialog) {

        // initialize the dialog
        OPENFILENAME ofn;       
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize     = sizeof(ofn);
        ofn.hwndOwner       = (HWND)window; 
        ofn.lpstrFile       = (LPSTR)dialog->selection_buffer_cstr;
        ofn.lpstrInitialDir = (LPCSTR)dialog->start;
        ofn.nMaxFile        = dialog->selection_buffer_size;
        ofn.lpstrTitle      = (LPCSTR)dialog->title; 
        ofn.nFilterIndex    = 1;
        ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOLONGNAMES | OFN_NONETWORKBUTTON | OFN_EXPLORER;
        ofn.lpstrFilter  = (dialog->filter == NULL || dialog->filter[0] == 0)
            ? "All Files\0*.*\0"
            : (LPCSTR)dialog->filter;

        // display the dialog
        dialog->did_select = GetOpenFileName(&ofn);
        if (dialog->did_select) {
            win32_window_set_last_error();
        }
        return(dialog->did_select);
    }

    SLD_API_OS_FUNC bool
    win32_window_save_file_dialog(
        const os_window_handle window,
        os_window_dialog* dialog) {

        assert(
            window != NULL &&
            dialog != NULL
        );

        win32_window_clear_last_error();

        // initialize the dialog
        OPENFILENAME ofn;       
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize     = sizeof(ofn);
        ofn.hwndOwner       = (HWND)window; 
        ofn.lpstrFile       = (LPSTR)dialog->selection_buffer_cstr;
        ofn.lpstrInitialDir = (LPCSTR)dialog->start;
        ofn.nMaxFile        = dialog->selection_buffer_size;
        ofn.lpstrTitle      = (LPCSTR)dialog->title; 
        ofn.nFilterIndex    = 1;
        ofn.Flags           = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
        ofn.lpstrFilter     = (dialog->filter == NULL || dialog->filter[0] == 0)
            ? "All Files\0*.*\0"
            : (LPCSTR)dialog->filter;

        // display the dialog
        dialog->did_select = GetSaveFileName(&ofn);

        if (dialog->did_select) {
            win32_window_set_last_error();
        }
        return(dialog->did_select);
    }

    SLD_API_OS_FUNC bool 
    win32_window_process_events(
        const os_window_handle window,
        os_window_event_list*  event_list) {

        assert(
            event_list           != NULL &&
            event_list->capacity != 0    &&
            event_list->array    != NULL
        );

        MSG  msg;
        const UINT msg_filter_min = 0;
        const UINT msg_filter_max = 0;
        const UINT msg_remove     = PM_REMOVE;

        for (
              event_list->count = 0;
              event_list->count <= event_list->capacity;
            ++event_list->count) {

            const bool has_message = PeekMessage(
                &msg,
                NULL,
                msg_filter_min,
                msg_filter_max,
                msg_remove                
            );

            if (!has_message) break;

            // cache the event and update the count
            os_window_event& event = event_list->array[event_list->count];
            ++event_list->count;

            // check window-specific events
            if (msg.hwnd == window) {

                switch (msg.message) {
                    case WM_KEYDOWN:
                    case WM_SYSKEYDOWN: {
                        event.type    = os_window_event_type_e_key_down;
                        event.keycode = win32_input_get_keycode(msg.wParam);
                    } break;
                    case WM_KEYUP:
                    case WM_SYSKEYUP: {
                        event.type = os_window_event_type_e_key_up;
                        event.keycode = win32_input_get_keycode(msg.wParam);
                    } break;
                    case WM_MOVE: {
                        event.type              = os_window_event_type_e_moved;
                        event.window_position.x = LOWORD(msg.lParam);   
                        event.window_position.y = HIWORD(msg.lParam);   
                    } break;
                    case WM_SIZE: {
                        event.type               = os_window_event_type_e_resized;
                        event.window_size.width  = LOWORD(msg.lParam);   
                        event.window_size.height = HIWORD(msg.lParam);  
                    } break;
                    case WM_MOUSEMOVE: {
                        event.type             = os_window_event_type_e_mouse_move;
                        event.mouse_position.x = LOWORD(msg.lParam);   
                        event.mouse_position.y = HIWORD(msg.lParam);
                    } break;
                    case WM_LBUTTONDOWN:   { event.type = os_window_event_type_e_mouse_left_down;       } break;
                    case WM_LBUTTONUP:     { event.type = os_window_event_type_e_mouse_left_up;         } break;
                    case WM_LBUTTONDBLCLK: { event.type = os_window_event_type_e_mouse_left_dbl_click;  } break;
                    case WM_RBUTTONDOWN:   { event.type = os_window_event_type_e_mouse_right_down;      } break;
                    case WM_RBUTTONUP:     { event.type = os_window_event_type_e_mouse_right_up;        } break;
                    case WM_RBUTTONDBLCLK: { event.type = os_window_event_type_e_mouse_right_dbl_click; } break;
                    default:               { event.type = os_window_event_type_e_none;                  } break;
                }
            }

            // check thread-wide events
            else {
                switch (msg.message) {
              
                    case (WM_QUIT): { event.type = os_window_event_type_e_quit; } break; 
                    default: break;
                }
            }

            // handle the message
            (void)TranslateMessage(&msg);
            (void)DispatchMessage  (&msg);
        }

        return(true);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_API_OS_INTERNAL void
    win32_window_set_last_error(
        void) {
        
        const DWORD win32_error = GetLastError();
        switch (win32_error) {

            case (ERROR_FILE_NOT_FOUND):          { _last_error_window = os_window_error_e_resource_not_found;     break; } // Resource file (icon/cursor/etc.) not found.
            case (ERROR_ACCESS_DENIED):           { _last_error_window = os_window_error_e_access_denied;          break; } // Operation not permitted (e.g., cross-process window manipulation).
            case (ERROR_NOT_ENOUGH_MEMORY):       { _last_error_window = os_window_error_e_system_out_of_memory;   break; } // System ran out of memory.
            case (ERROR_OUTOFMEMORY):             { _last_error_window = os_window_error_e_general_out_of_memory;  break; } // Out of memory (general resource exhaustion).
            case (ERROR_INVALID_PARAMETER):       { _last_error_window = os_window_error_e_invalid_args;           break; } // Invalid argument/flag passed to an API.
            case (ERROR_INVALID_WINDOW_HANDLE):   { _last_error_window = os_window_error_e_invalid_handle;         break; } // The window handle (HWND) is invalid.
            case (ERROR_CANNOT_FIND_WND_CLASS):   { _last_error_window = os_window_error_e_invalid_class;          break; } // Window class not registered.
            case (ERROR_CLASS_ALREADY_EXISTS):    { _last_error_window = os_window_error_e_class_already_exists;   break; } // Attempted to register an already-registered window class.
            case (ERROR_DC_NOT_FOUND):            { _last_error_window = os_window_error_e_invalid_device_context; break; } // Invalid or already released device context (DC).
            case (ERROR_INVALID_THREAD_ID):       { _last_error_window = os_window_error_e_invalid_thread;         break; } // Thread ID is invalid / has no message queue.
            case (ERROR_RESOURCE_NAME_NOT_FOUND): { _last_error_window = os_window_error_e_invalid_resource;       break; } // Resource (cursor, icon, etc.) not found.
            case (ERROR_NOT_ENOUGH_QUOTA):        { _last_error_window = os_window_error_e_quota_exceeded;         break; } // System/user quota limit exceeded.
            default:                              { _last_error_window = os_window_error_e_unknown;                break; } // General/unknown error
        }
    }

    SLD_API_OS_INTERNAL void
    win32_window_clear_last_error(
        void) {

        _last_error_window = os_window_error_e_success;
    }
};