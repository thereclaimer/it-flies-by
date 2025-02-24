#ifndef IFB_GRAPHICS_INTERNAL_HPP
#define IFB_GRAPHICS_INTERNAL_HPP

#include "ifb-graphics.hpp"

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

struct IFBGraphicsContext;
struct IFBWindow;

struct IFBGraphicsHandles {
    IFBU32 parent_window;
};

/**********************************************************************************/
/* WINDOW                                                                         */
/**********************************************************************************/

#define IFB_WINDOW_TITLE_LENGTH_MAX 255

struct IFBWindowPlatformContexts {
    IFBGLContext  opengl;
    ImGuiContext* imgui;
};

struct IFBWindowTitle {
    IFBChar c_str[IFB_WINDOW_TITLE_LENGTH_MAX];
};

struct IFBWindow {
    IFBHNDWindow              handle;
    IFBPosition               position;
    IFBDimensions             dimensions;
    IFBWindowPlatformContexts platform_contexts;
    IFBWindowTitle            title;
};

/**********************************************************************************/
/* CONTEXT                                                                        */
/**********************************************************************************/

struct IFBGraphicsContext {
    IFBHNDArena    arena_handle;
    IFBColorFormat color_format;
    IFBWindow*     window;
};

namespace ifb_graphics {
    const IFBHNDArena    context_get_arena_handle (IFBVoid);
    const IFBColorFormat context_get_color_format (IFBVoid);
    IFBWindow*           context_get_window       (IFBVoid);
};

#endif //IFB_GRAPHICS_INTERNAL_HPP