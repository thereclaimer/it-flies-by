#pragma once

#include "ifb-win32.hpp"

ifb_internal ifb_s32 
ifb_win32_main(
    RWin32MainArgs& args) {

    _ifb_win32 = {0};

    ifb_b8 result = true;

    //create the rlibs win32 context
    result = r_win32::context_create(args);

    //configuration
    ifb_win32::win32_config_set(_ifb_win32);

    //reserve memory
    result &= ifb_win32::win32_memory_reserve(_ifb_win32);

    //create the window0
    result &= ifb_win32::win32_window_create(_ifb_win32);

    //start the game
    result &=  ifb_win32::win32_game_startup(_ifb_win32);

    //run the game till we get a quit event
    ifb_win32::win32_game_run(_ifb_win32);
    
    //destroy the context
    result &= r_win32::context_destroy();

    //we're done
    return(result ? S_OK : S_FALSE);
}

ifb_internal ifb_void 
ifb_win32::win32_config_set(
    IFBWin32& win32_ref) {

    win32_ref.config.reservation_size_max      = r_mem::size_gigabytes(IFB_WIN32_CONFIG_DEFAULT_RESERVATION_SIZE_GIGABYTES); 
    win32_ref.config.memory_manager_stack_size = IFB_WIN32_CONFIG_DEFAULT_MEMORY_STACK_SIZE;
    win32_ref.config.win32_arena_size          = IFB_WIN32_CONFIG_DEFAULT_ARENA_SIZE;
    win32_ref.config.win32_arena_count         = IFB_WIN32_CONFIG_DEFAULT_ARENA_COUNT;
    win32_ref.config.win32_window_size_width   = IFB_WIN32_CONFIG_DEFAULT_WINDOW_SIZE_WIDTH;
    win32_ref.config.win32_window_size_height  = IFB_WIN32_CONFIG_DEFAULT_WINDOW_SIZE_HEIGHT;
}

ifb_internal const ifb_b8
ifb_win32::win32_memory_reserve(
    IFBWin32& win32_ref) {

    ifb_b8 result = true;

    //create the memory manager
    result &= r_mem::memory_manager_create_win32(
        _ifb_win32.config.reservation_size_max,
        _ifb_win32.config.memory_manager_stack_size);

    //create the reservation
    _ifb_win32.memory.memory_reservation = r_mem::reserve("IT FLIES BY",_ifb_win32.config.reservation_size_max);
    result &= _ifb_win32.memory.memory_reservation != NULL;

    //create a region for our win32 systems
    _ifb_win32.memory.win32_region = 
        r_mem::region_create_arena_pool(
            _ifb_win32.memory.memory_reservation,
            "WIN32 PLATFORM",
            _ifb_win32.config.win32_arena_size,
            _ifb_win32.config.win32_arena_count);
    result &= _ifb_win32.memory.win32_region != NULL;

    //set the win32 region
    result &= r_win32::context_set_memory_region(_ifb_win32.memory.win32_region);

    //we're done
    return(result);
}

ifb_internal const ifb_b8
ifb_win32::win32_window_create(
    IFBWin32& win32_ref) {

    //get monitor info
    r_win32::monitor_info(win32_ref.window.monitor_info);

#if 0
    win32_ref.window.window_handle = 
        r_win32::window_create_centered(
            "It Flies By",
            win32_ref.window.monitor_info.pixels_width,
            win32_ref.window.monitor_info.pixels_height,
            RColorFormat_RGBA,
            RWin32WindowUseImGui_No);
    
#else
    win32_ref.window.window_handle = 
        r_win32::window_create_centered(
            "It Flies By",
            1024,
            768,
            RColorFormat_RGBA,
            RWin32WindowUseImGui_No);
#endif   

    ifb_b8 result = true;

    result &= win32_ref.window.window_handle != NULL;

    //create the opengl context
    win32_ref.window.rendering_context_handle = r_win32::rendering_create_opengl_context(_ifb_win32.window.window_handle); 
    result &= win32_ref.window.window_handle != NULL;

    //create the imgui context
    win32_ref.window.imgui_context = r_win32::imgui_create_context(_ifb_win32.window.window_handle);
    result &= win32_ref.window.imgui_context != NULL;

    //set the clear color
    RColor32Bit color_32 = {0};
    color_32.format = RColorFormat_RGBA;
    color_32.hex    = 0x282828FF;
    r_win32::rendering_set_clear_color(_ifb_win32.window.rendering_context_handle,color_32);

    //initialize the dialog
    win32_ref.window.file_dialog_handle = r_win32::file_dialog_create(_ifb_win32.window.window_handle);
    result &= win32_ref.window.file_dialog_handle != NULL;

    //show the window
    result &= r_win32::window_show(_ifb_win32.window.window_handle);

    //we're done
    return(result);
}

ifb_internal const ifb_b8
ifb_win32::win32_game_startup(
    IFBWin32& win32_ref) {

    ifb_b8 result = true;

    //initialize the platform api
    ifb_win32::file_api_initialize(_ifb_win32.game.platform_api.file);
    ifb_win32::file_dialog_api_initialize(_ifb_win32.game.platform_api.file_dialog);

    //create the engine
    _ifb_win32.game.engine_handle = 
        ifb_engine::engine_startup(
            _ifb_win32.memory.memory_reservation,
            _ifb_win32.window.imgui_context,
            _ifb_win32.game.platform_api);

    result &= _ifb_win32.game.engine_handle != NULL;
    
    return(result);
}

ifb_internal ifb_void
ifb_win32::win32_game_run(
    IFBWin32& win32_ref) {

    //main game loop
    r_b8 running = (
        win32_ref.window.window_handle &&
        win32_ref.game.engine_handle);

    while(running) {

        //start a new frame
        running &= r_win32::window_frame_start(win32_ref.window.window_handle);

        //update the engine        
        running &= ifb_engine::engine_update(win32_ref.game.engine_handle);

        //render the frame
        running &= r_win32::window_frame_render(win32_ref.window.window_handle);
    }
}
