#pragma once

#include "ifb-win32.hpp"

global IFBWin32Opengl ifb_win32_opengl;

internal IFBWin32OpenglHandle
ifb_win32_opengl_create_and_initialize() {

    HDC device_context = ifb_win32_window_device_context();
    ifb_assert(device_context);

    //set our preferred format descriptor
    PIXELFORMATDESCRIPTOR preferred_format_descriptor = {0};
    preferred_format_descriptor.nSize      = sizeof(preferred_format_descriptor);
    preferred_format_descriptor.nVersion   = 1;
    preferred_format_descriptor.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    preferred_format_descriptor.iPixelType = PFD_TYPE_RGBA;
    preferred_format_descriptor.cColorBits = 32;

    //query for the closest format descriptor
    const s32 chosen_format_descriptor = 
        ChoosePixelFormat(
            device_context,
            &preferred_format_descriptor);

    ifb_assert(chosen_format_descriptor);

    //set the chosen pixel format
    b8 pixel_format_is_set = 
        SetPixelFormat(
            device_context,
            chosen_format_descriptor,
            &preferred_format_descriptor);

    ifb_assert(pixel_format_is_set);

    //create opengl context
    HGLRC opengl_context = wglCreateContext(device_context);
    ifb_assert(opengl_context);

    //make the context current
    b8 context_is_current = 
        wglMakeCurrent(
            device_context,
            opengl_context);
 
    ifb_assert(context_is_current);

    assert(glewInit() == GLEW_OK);

    glClearColor(
        0.1569f, 
        0.1569f,
        0.1569f,
        1.0000f);


    return(&ifb_win32_opengl);
}

internal void
ifb_win32_opengl_resize_and_clear_viewport() {

    IFBWin32WindowDimensionsPixels app_dimensions = 
        ifb_win32_window_app_dimensions();

    glViewport(
        0,
        0,
        app_dimensions.width,
        app_dimensions.height);

    glClear(GL_COLOR_BUFFER_BIT);
}

