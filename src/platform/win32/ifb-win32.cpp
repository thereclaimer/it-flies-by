#pragma once

#include "ifb-win32.hpp"

global IFBWin32 ifb_win32 = {0};

internal void
ifb_win32_create_window() {
    
    ifb_win32.window = 
        ifb_win32_window_create_and_initialize(
            ifb_win32.args.h_instance,
            ifb_win32.args.cmd_show);

    ifb_assert(ifb_win32.window); 
}

internal void
ifb_win32_create_opengl() {

    ifb_win32.opengl = 
        ifb_win32_opengl_create_and_initialize();

    ifb_assert(ifb_win32.opengl);
}

internal void
ifb_win32_create_imgui() {

}

internal void
ifb_win32_create_game() {

    //platform api
    IFBPlatformApi win32_platform_api = ifb_win32_api_create_and_initialize();

    //engine
    ifb_win32.engine = ifb_engine_create_and_initialize(win32_platform_api);
    ifb_assert(ifb_win32.engine);

    //game
    ifb_win32.game = ifb_game_create_and_initialize(ifb_win32.engine);
    ifb_assert(ifb_win32.game);

}

internal int
ifb_win32_main(
    HINSTANCE h_instance,
    HINSTANCE h_instance_prev,
    PWSTR     cmd_line,
    int       cmd_show) {

    //store args
    ifb_win32.args.h_instance      = h_instance;
    ifb_win32.args.h_instance_prev = h_instance_prev;
    ifb_win32.args.cmd_line        = cmd_line;
    ifb_win32.args.cmd_show        = cmd_show;

    //initialize the window and graphics
    ifb_win32_create_window();
    ifb_win32_create_opengl();
    ifb_win32_create_imgui();

    //initialize the game
    ifb_win32_create_game();

    bool running = true;

    while(running) {

        //frame start
        ifb_win32_window_process_events();
        ifb_win32_opengl_resize_and_clear_viewport();

        //update game
        running &= ifb_game_update_and_render(ifb_win32.user_input);

        //render frame
        ifb_win32_window_render();
        running &= !ifb_win32_window_quit_event_received();
    }

    return(0);
}