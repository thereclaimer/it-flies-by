#pragma once

#include "itfliesby-platform-win32.hpp"
#include "itfliesby-platform-win32-api.cpp"
#include "itfliesby-platform-win32-user-input.cpp"

ItfliesbyPlatformWin32Window game_window;

internal void
itfliesby_platform_win32_monitor_dimensions() {

    HMONITOR monitor_handle = MonitorFromWindow(game_window.window_handle,MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitor_info = {0};
    monitor_info.cbSize = sizeof(monitor_info);
    GetMonitorInfo(monitor_handle,&monitor_info);

    RECT window_rect = monitor_info.rcMonitor;

    game_window.monitor_dimensions.width  = window_rect.right  - window_rect.left; 
    game_window.monitor_dimensions.height = window_rect.bottom - window_rect.top; 
}

internal void
itfliesby_platform_win32_toggle_full_screen() {

    //https://stackoverflow.com/questions/2382464/win32-full-screen-and-hiding-taskbar
    
    game_window.full_screen = !game_window.full_screen;

    if (game_window.full_screen) {

        //we need to save the current window dimensions
        game_window.window_style    = GetWindowLong(game_window.window_handle, GWL_STYLE);
        game_window.window_ex_style = GetWindowLong(game_window.window_handle, GWL_EXSTYLE);

        SetWindowLong(
            game_window.window_handle,
            GWL_STYLE,
            game_window.window_style & ~(WS_CAPTION | WS_THICKFRAME)
        );
    
        SetWindowLong(
            game_window.window_handle,
            GWL_EXSTYLE,
            game_window.window_ex_style & ~(
                WS_EX_DLGMODALFRAME | 
                WS_EX_WINDOWEDGE |
                WS_EX_CLIENTEDGE |
                WS_EX_STATICEDGE
                )
        );

        HMONITOR monitor_handle = MonitorFromWindow(game_window.window_handle,MONITOR_DEFAULTTONEAREST);
        MONITORINFO monitor_info = {0};
        monitor_info.cbSize = sizeof(monitor_info);
        GetMonitorInfo(monitor_handle,&monitor_info);

        RECT window_rect = monitor_info.rcMonitor;
        SetWindowPos(
            game_window.window_handle, 
            NULL, 
            window_rect.left,
            window_rect.top,
            window_rect.right,
            window_rect.bottom,
                    SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);


        game_window.window_dimensions.width  = (window_rect.right - window_rect.left);
        game_window.window_dimensions.height = (window_rect.bottom - window_rect.top);
    }
    else {
        
        //we need to update the window with the saved dimesions
        SetWindowLong(game_window.window_handle, GWL_STYLE,   game_window.window_style);
        SetWindowLong(game_window.window_handle, GWL_EXSTYLE, game_window.window_ex_style);
        //restore maximized
        SendMessage(game_window.window_handle, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    }
}


internal void
itfliesby_platform_win32_process_pending_messages(
    HWND window_handle) {

    game_window.user_input.mouse_x_delta = 0;
    game_window.user_input.mouse_y_delta = 0;
    game_window.user_input.mouse_wheel   = 0;

    //if we have a set of pending messages, loop through them until we have processed
    //them all
    MSG window_message = {0};
    while (PeekMessage(&window_message, 0,0,0, PM_REMOVE)) {

        //basically, we want all user input to be processed before Windows has a chance to intercept and dispatch it
        switch (window_message.message) {

            //keyboard input
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN: {

                u32 itfliesby_keycode = itfliesby_platform_win32_translate_user_keycode((u32)window_message.wParam);
                USER_INPUT_SET_KEY(game_window.user_input, itfliesby_keycode);

                switch((u32)window_message.wParam) {
                    case VK_F11: {
                        itfliesby_platform_win32_toggle_full_screen();
                    } break;
                }

            } break;

            case WM_KEYUP:
            case WM_SYSKEYUP: {

                u32 itfliesby_keycode = itfliesby_platform_win32_translate_user_keycode((u32)window_message.wParam);
                USER_INPUT_CLEAR_KEY(game_window.user_input, itfliesby_keycode);
            } break;

            //mouse inputs
            case WM_LBUTTONDOWN: {

                game_window.user_input.mouse_button_left = true;
            } break;

            case WM_RBUTTONDOWN: {
 
                game_window.user_input.mouse_button_right = true;
            } break;

            case WM_LBUTTONUP: {

                game_window.user_input.mouse_button_left = false;
            } break;

            case WM_RBUTTONUP: {

                game_window.user_input.mouse_button_right = false;
            } break;

            case WM_MOUSEMOVE: {

                //this will persist between mouse move events
                local POINT previous_cursor_position;

                //get the current mouse position and update delta mouse position
                POINT current_cursor_position = {0};
                GetCursorPos(&current_cursor_position);

                game_window.user_input.mouse_x_current = current_cursor_position.x;
                game_window.user_input.mouse_y_current = current_cursor_position.y;
                game_window.user_input.mouse_x_delta   = (current_cursor_position.x - previous_cursor_position.x);
                game_window.user_input.mouse_y_delta   = (current_cursor_position.y - previous_cursor_position.y);

                //update the previous cursor position for the next mouse event
                previous_cursor_position = current_cursor_position;

            } break;

            case WM_MOUSEWHEEL: {
                game_window.user_input.mouse_wheel = GET_WHEEL_DELTA_WPARAM(window_message.wParam);

            } break;

            //anything not related to keyboard or mouse input, we can let windows handle
            //at which point, our window callback function will be called
            default: {
                TranslateMessage(&window_message);
                DispatchMessage(&window_message);
            } break;
        }

        ImGui_ImplWin32_WndProcHandler(
            window_message.hwnd,
            window_message.message,
            window_message.wParam,
            window_message.lParam
        );
    }
}

LRESULT CALLBACK
itfliesby_platform_win32_window_callback(HWND window_handle,
                      u32 message,
                      WPARAM w_param,
                      LPARAM l_param) {

    local RECT window_rect = {0};

    switch (message) {

        case WM_ACTIVATEAPP: {
            
            XInputEnable(true);

        } break;

        //window resize
        case WM_SIZE: {
            GetClientRect(game_window.window_handle, &window_rect);
            if (game_window.game) {
                game_window.window_dimensions.width  = (window_rect.right - window_rect.left);
                game_window.window_dimensions.height = (window_rect.bottom - window_rect.top);
            }

            game_window.maximized = w_param == SIZE_MAXIMIZED;

            itfliesby_platform_win32_monitor_dimensions();

        } break;

        //window close
        case WM_CLOSE:
        case WM_DESTROY: {
            game_window.running = false;
        } break;

        default: {
            return DefWindowProc(window_handle, message, w_param, l_param);
        }
    }

    return 0;
}

internal int
itfliesby_platform_win32_main(
    HINSTANCE instance,
    HINSTANCE prev_isntance,
    PWSTR     cmd_line,
    int       cmd_show) {


    // Register the main window class.
    WNDCLASS window_class      = {0};
    window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc   = (WNDPROC)itfliesby_platform_win32_window_callback;
    window_class.hInstance     = instance;
    window_class.lpszClassName = "ItfliesbyWindowClass";

    ifb_assert(RegisterClass(&window_class));

    game_window = {0};
    game_window.window_handle = CreateWindowEx(
        0,
        window_class.lpszClassName,
        "It Flies By",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        (GetSystemMetrics(SM_CXSCREEN) - 1024) / 2, // X position (centered)
        (GetSystemMetrics(SM_CYSCREEN) - 768) / 2, // Y position (centered)
        1024,
        768,
        0,
        0,
        instance,
        0);

    game_window.window_dimensions.width  = 1024;
    game_window.window_dimensions.height = 768;

    itfliesby_platform_win32_toggle_full_screen();

    ifb_assert(game_window.window_handle);

    game_window.device_context = GetDC(game_window.window_handle);

    itfliesby_platform_win32_monitor_dimensions();

    //put the platform api together
    ItfliesbyPlatformApi win32_platform_api = {0};

    win32_platform_api.window            = (handle)&game_window;
    win32_platform_api.file_size         = itfliesby_platform_win32_api_file_get_file_size_bytes;
    win32_platform_api.file_read         = itfliesby_platform_win32_api_read_file;
    win32_platform_api.file_write        = itfliesby_platform_win32_api_write_file;
    win32_platform_api.file_open         = itfliesby_platform_win32_api_open_file;
    win32_platform_api.file_close        = itfliesby_platform_win32_api_close_file;
    win32_platform_api.memory_allocate   = itfliesby_platform_win32_api_allocate_memory;
    win32_platform_api.memory_free       = itfliesby_platform_win32_api_free_memory;
    win32_platform_api.graphics_api_init = itfliesby_platform_win32_api_opengl_initialize;
    win32_platform_api.imgui_init        = itfliesby_platform_win32_api_imgui_initialize;
    win32_platform_api.imgui_frame_start = itfliesby_platform_win32_api_imgui_frame_start;
    win32_platform_api.imgui_frame_end   = itfliesby_platform_win32_api_imgui_frame_end;
    win32_platform_api.ticks             = itfliesby_platform_win32_api_ticks; 
    win32_platform_api.delta_time_ms     = itfliesby_platform_win32_api_delta_time_ms; 
    win32_platform_api.sleep             = itfliesby_platform_win32_api_sleep; 

    //allocate the memory
    memory game_memory = itfliesby_platform_win32_api_allocate_memory(ITFLIESBY_GAME_MEMORY_SIZE); 
    ifb_assert(game_memory);

    //initialize the game
    game_window.game = itfliesby_game_create(
        win32_platform_api,
        game_memory,
        ITFLIESBY_GAME_MEMORY_SIZE
    );

    ItfliesbyGamepadInput gamepad = {0};

    game_window.running = true;

    while (game_window.running) {

        //process incoming messages
        itfliesby_platform_win32_process_pending_messages(game_window.window_handle);
        if (!game_window.running) {
            break;
        }

        //get xinput state for game pads
        game_window.user_input.game_pad =
            itfliesby_platform_win32_get_xinput_state(&gamepad)
                ? &gamepad
                : NULL;

        //update the game
        itfliesby_game_update_and_render(
            game_window.game,
            &game_window.user_input,
            game_window.window_dimensions.width,
            game_window.window_dimensions.height,
            game_window.monitor_dimensions.width,
            game_window.monitor_dimensions.height);

        SwapBuffers(game_window.device_context);
    }
    //destroy the game
    itfliesby_game_destroy(game_window.game);

    //deallocate the memory
    itfliesby_platform_win32_api_free_memory(game_memory,ITFLIESBY_GAME_MEMORY_SIZE);

    return(0);
}