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

    return(0);
}

internal LRESULT
ifb_win32_window_on_destroy(
    WPARAM w_param,
    LPARAM l_param) {

    LRESULT result = 0;

    PostQuitMessage(0);

    return(result);
}

internal LRESULT CALLBACK 
ifb_win32_window_callback(
    HWND   window_handle,
    UINT   message,
    WPARAM w_param,
    LPARAM l_param) {

    funcptr_ifb_win32_window_on_wm_message funcptr_wm_message_handler = NULL;

    switch (message) {
        
        case WM_SIZE:    funcptr_wm_message_handler = ifb_win32_window_on_sized;   break;
        case WM_QUIT:    funcptr_wm_message_handler = ifb_win32_window_on_quit;    break;
        case WM_DESTROY: funcptr_wm_message_handler = ifb_win32_window_on_destroy; break;

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
            WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            IFB_WIN32_WINDOW_DEFAULT_WIDTH, 
            IFB_WIN32_WINDOW_DEFAULT_HEIGHT,
            NULL,
            NULL,
            h_instance,
            NULL);

    ifb_assert(ifb_win32_window.handle);

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
ifb_win32_window_process_events() {

    while(PeekMessage(&ifb_win32_window.message,0,0,0,PM_REMOVE)) {
        
        switch (ifb_win32_window.message.message)
        {
            //TODO: this is where we need to pass key/mouse events to the game
            //BUT we gotta let Windows/ImGUi process those as well

            default: {
                TranslateMessage(&ifb_win32_window.message);
                DispatchMessage(&ifb_win32_window.message);
            } break;
        }
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