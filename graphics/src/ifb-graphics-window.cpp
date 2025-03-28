#pragma once

#include "ifb-graphics.hpp"

/**********************************************************************************/
/* WINDOW                                                                         */
/**********************************************************************************/

const IFBU32
ifb_graphics::window_memory_size(
    IFBVoid) {

    const IFBU32 size_window   = ifb_macro_align_size_struct(IFBGraphicsWindow); 
    const IFBU32 size_title    = IFB_WINDOW_TITLE_LENGTH_MAX;
    const IFBU32 size_platform = ifb_platform::window_size();
    
    const IFBU32 size_total = 
        size_window + 
        size_title  +
        size_platform;

    return(size_total);
}

IFBGraphicsWindow* 
ifb_graphics::window_memory_initialize(
    const IFBGraphicsWindowArgs& args) {

    ifb_macro_assert(args.memory.start);
    ifb_macro_assert(args.memory.size);

    const IFBAddr start_window = args.memory.start;
    const IFBAddr start_title  = start_window + ifb_macro_align_size_struct(IFBGraphicsWindow);

    //cast pointers
    IFBGraphicsWindow* window       = (IFBGraphicsWindow*)start_window;
    IFBChar*           window_title =           (IFBChar*)start_title;

    //copy the window title
    strcpy_s(
        window_title,
        IFB_WINDOW_TITLE_LENGTH_MAX,
        args.title);

    //set the properties
    window->title = window_title;
    window->dims  = args.dims;
    window->pos   = args.pos;

    //tell the platform to create the window
    const IFBB8 window_created = ifb_platform::window_create(window);
    ifb_macro_assert(window_created);

    //we're done
    return(window);
}

const IFBB8 
ifb_graphics::window_show(
    IFBGraphicsWindow* ptr_window) {
    
    ifb_macro_assert(ptr_window);

    //tell the platform to show the window
    //the platform should set the flag
    IFBB8 is_visible = true;
    is_visible &= ifb_platform::window_show(ptr_window);
    is_visible &= ifb_macro_bit_test(IFBPlatformWindowFlags_Visible, ptr_window->flags);
    
    //we're done
    return(is_visible);
}

const IFBB8 
ifb_graphics::window_frame_start(
    IFBGraphicsWindow* ptr_window) {

    //start the frame
    const IFBB8 result = ifb_platform::window_process_events(ptr_window);

    return(result);
}

const IFBB8 
ifb_graphics::window_frame_render(
    IFBGraphicsWindow* ptr_window) {

    //start the frame
    const IFBB8 result = ifb_platform::window_swap_buffers(ptr_window);

    return(result);
}
