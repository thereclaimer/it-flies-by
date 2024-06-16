#ifndef IFB_WIN32_HPP
#define IFB_WIN32_HPP

#include <itfliesby-types.hpp>
#include <Windows.h>
#include <GL/GL.h>

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
typedef void*           IFBWin32OpenglHandle;


struct  IFBWin32ImGui;
typedef IFBWin32ImGui*  IFBWin32ImGui_Ptr;
typedef IFBWin32ImGui&  IFBWin32ImGui_Ref;

//--------------------------------
// WIN32
//--------------------------------

struct IFBWin32Args {
    HINSTANCE h_instance;
    HINSTANCE h_instance_prev;
    PWSTR     cmd_line;
    int       cmd_show;
};

struct IFBWin32 {
    IFBWin32Args         args;
    IFBWin32WindowHandle window;
    IFBWin32OpenglHandle opengl;
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
ifb_win32_window_create_and_initialize(
    HINSTANCE h_instance,
    int       cmd_show);

void
ifb_win32_window_process_events();

typedef LRESULT
(*funcptr_ifb_win32_window_on_wm_message) (
    WPARAM w_param,
    LPARAM l_param);

bool
ifb_win32_window_quit_event_received();

void 
ifb_win32_window_render();

HDC
ifb_win32_window_device_context();

IFBWin32WindowDimensionsPixels
ifb_win32_window_app_dimensions();

//--------------------------------
// OPENGL
//--------------------------------

struct IFBWin32Opengl {
    HGLRC gl_context;
};

IFBWin32OpenglHandle
ifb_win32_opengl_create_and_initialize();

void
ifb_win32_opengl_resize_and_clear_viewport();

#endif  //IFB_WIN32_HPP