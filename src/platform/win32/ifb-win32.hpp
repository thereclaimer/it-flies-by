#ifndef IFB_WIN32_HPP
#define IFB_WIN32_HPP

#include <itfliesby-types.hpp>
#include <Windows.h>

struct  IFBWin32;
typedef IFBWin32*       IFBWin32_Ptr;
typedef IFBWin32&       IFBWin32_Ref;

struct  IFBWin32Window;
typedef IFBWin32Window* IFBWin32Window_Ptr;
typedef IFBWin32Window& IFBWin32Window_Ref;
typedef void*           IFBWin32WindowHandle;

struct  IFBWin32Opengl;
typedef IFBWin32Opengl* IFBWin32Opengl_Ptr;
typedef IFBWin32Opengl& IFBWin32Opengl_Ref;

struct  IFBWin32ImGui;
typedef IFBWin32ImGui*  IFBWin32ImGui_Ptr;
typedef IFBWin32ImGui&  IFBWin32ImGui_Ref;


//--------------------------------
// WIN32
//--------------------------------

struct IFBWin32 {
    IFBWin32WindowHandle window;
};

#define ifb_win32_main wWinMain

//--------------------------------
// WINDOW
//--------------------------------

#define IFB_WIN32_WINDOW_DEFAULT_WIDTH  1280
#define IFB_WIN32_WINDOW_DEFAULT_HEIGHT 720

struct IFBWin32WindowDimensionsPixels {
    u32 width;
    u32 height;
};

struct IFBWin32Window {
    HWND                           handle;
    HDC                            device_context;
    b8                             quit_event_received;
    MSG                            message;
    IFBWin32WindowDimensionsPixels app_dimensions_pixels;
    IFBWin32WindowDimensionsPixels monitor_dimensions_pixels;
};

IFBWin32WindowHandle
ifb_win32_window_create_and_initialize();

void
ifb_win32_window_process_events();

typedef LRESULT
(*funcptr_ifb_win32_window_on_wm_message) (
    WPARAM w_param,
    LPARAM l_param);

#endif  //IFB_WIN32_HPP