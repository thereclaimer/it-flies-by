#pragma once

#include "ifb-win32.hpp"

global IFBWin32Window ifb_win32_window;

internal LRESULT
ifb_win32_window_on_sized(
    WPARAM w_param,
    LPARAM l_param) {

    ifb_win32_window.app_dimensions_pixels.width  = LOWORD(l_param);
    ifb_win32_window.app_dimensions_pixels.height = HIWORD(l_param);

    return(0);
}

internal LRESULT
ifb_win32_window_on_quit(
    WPARAM w_param,
    LPARAM l_param) {

    ifb_win32_window.quit_event_received = true;
    PostQuitMessage(0);

    return(0);
}

internal LRESULT CALLBACK 
ifb_win32_window_callback(
    HWND   window_handle,
    UINT   message,
    WPARAM w_param,
    LPARAM l_param) {

    LRESULT imgui_result = 
        ifb_win32_imgui_message_handler(
            window_handle,
            message,
            w_param,
            l_param);
        
    if (imgui_result) {
        return(imgui_result);
    }

    funcptr_ifb_win32_window_on_wm_message funcptr_wm_message_handler = NULL;

    switch (message) {
        
        case WM_SIZE:    funcptr_wm_message_handler = ifb_win32_window_on_sized; break;
        case WM_QUIT:    funcptr_wm_message_handler = ifb_win32_window_on_quit;  break;
        case WM_DESTROY: funcptr_wm_message_handler = ifb_win32_window_on_quit;  break;
        case WM_CLOSE:   funcptr_wm_message_handler = ifb_win32_window_on_quit;  break;

        default: {

            LRESULT default_result = 
                DefWindowProc(
                    window_handle,
                    message,
                    w_param,
                    l_param);

            return(default_result);

        } break;
    }

    ifb_assert(funcptr_wm_message_handler);
    LRESULT ifb_result = funcptr_wm_message_handler(w_param,l_param);

    return(ifb_result);
}

internal IFBWin32WindowHandle
ifb_win32_window_create_and_initialize(
    HINSTANCE h_instance,
    int       cmd_show) {

    ifb_win32_window = {0};

    //define window class
    WNDCLASS window_class = {0};
    window_class.lpfnWndProc   = (WNDPROC)ifb_win32_window_callback;
    window_class.hInstance     = h_instance;
    window_class.lpszClassName = L"IFBWin32Window";
    window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

    b8 registered = RegisterClass(&window_class);
    ifb_assert(registered);

    //create the window
    ifb_win32_window.handle = 
        CreateWindowEx(
            0,
            window_class.lpszClassName,
            L"It Flies By", 
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            IFB_WIN32_WINDOW_DEFAULT_WIDTH, 
            IFB_WIN32_WINDOW_DEFAULT_HEIGHT,
            NULL,
            NULL,
            h_instance,
            NULL);

    ifb_assert(ifb_win32_window.handle);

    //center the window
    RECT screen_rect;
    SystemParametersInfo(SPI_GETWORKAREA,0,&screen_rect,0);
    u32 screen_width  = screen_rect.right  - screen_rect.left; 
    u32 screen_height = screen_rect.bottom - screen_rect.top; 

    RECT window_rect;
    GetWindowRect(ifb_win32_window.handle,&window_rect);
    int center_x = (screen_width  - IFB_WIN32_WINDOW_DEFAULT_WIDTH)  / 2;
    int center_y = (screen_height - IFB_WIN32_WINDOW_DEFAULT_HEIGHT) / 2;

    SetWindowPos(
        ifb_win32_window.handle,
        HWND_TOP,
        center_x,
        center_y ,
        0,
        0,
        SWP_NOSIZE | SWP_NOZORDER);

    //get the device context
    ifb_win32_window.device_context = GetDC(ifb_win32_window.handle);
    ifb_assert(ifb_win32_window.device_context);

    //show the window
    ShowWindow(
        ifb_win32_window.handle,
        cmd_show);

    return(&ifb_win32_window);
}

internal void
ifb_win32_window_monitor_info() {

    HMONITOR monitor_handle = MonitorFromWindow(ifb_win32_window.handle,MONITOR_DEFAULTTONEAREST);

    MONITORINFO monitor_info = {0};
    monitor_info.cbSize = sizeof(monitor_info);
    GetMonitorInfo(monitor_handle,&monitor_info);

    RECT window_rect = monitor_info.rcMonitor;

    ifb_win32_window.monitor_dimensions_pixels.width  = window_rect.right  - window_rect.left; 
    ifb_win32_window.monitor_dimensions_pixels.height = window_rect.bottom - window_rect.top; 
}

internal void
ifb_win32_window_process_events() {

    while(PeekMessage(&ifb_win32_window.message,0,0,0,PM_REMOVE)) {
        TranslateMessage(&ifb_win32_window.message);
        DispatchMessage(&ifb_win32_window.message);
    }
}

internal bool
ifb_win32_window_quit_event_received() {
    return(ifb_win32_window.quit_event_received);
}

internal void 
ifb_win32_window_render() {
    SwapBuffers(ifb_win32_window.device_context);
}

internal HDC
ifb_win32_window_device_context() {
    return(ifb_win32_window.device_context);
}


internal IFBWin32WindowDimensionsPixels
ifb_win32_window_app_dimensions() {
    return(ifb_win32_window.app_dimensions_pixels);
}

internal HWND
ifb_win32_window_handle() {
    return(ifb_win32_window.handle);
}