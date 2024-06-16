#pragma once

#include "itfliesby-engine-devtools.hpp"

internal ItfliesbyEngineDevtools
itfliesby_engine_devtools_create_and_init() {

    ItfliesbyEngineDevtools dev_tools = {0};

    glewInit();

    dev_tools.imgui_context = (ImGuiContext*)platform_api.imgui_init(platform_api.window);
    dev_tools.active        = false;
    dev_tools.demo          = false;

    ImGui::SetCurrentContext(dev_tools.imgui_context);

    return(dev_tools);
}

internal void
itfliesby_engine_devtools_tools_stats(
    ItfliesbyEngine* engine,
    bool*            show) {


    if (!ImGui::Begin("Game Status",show)) {
        ImGui::End();
    }

    ImGui::LabelText("Target FPS","%f",engine->frame.target_fps);
    ImGui::LabelText("Delta Time (ms)","%f",engine->frame.delta_time_ms);

    ImGui::End();
}

internal void
itfliesby_engine_devtools_show(
    ItfliesbyEngine* engine) {

    local bool show_tools_stats = false;

    //show the main menu bar
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Tools")) {

            if (ImGui::MenuItem("Game Status")) {
                show_tools_stats = true;
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    if (show_tools_stats) {
        itfliesby_engine_devtools_tools_stats(engine,&show_tools_stats);
    }

}

internal void
itfliesby_engine_devtools_update(
    ItfliesbyEngine* engine) {

    handle platform_window_handle = platform_api.window;
    

    ItfliesbyEngineDevtools* dev_tools  = &engine->dev_tools;
    ItfliesbyUserInput       user_input = *engine->user_input;

    local b8 f1_state_current;
    local b8 f1_state_previous;
    local b8 f2_state_current;
    local b8 f2_state_previous;

    f1_state_previous = f1_state_current;
    f1_state_current = ITFLIESBY_KEYCODE_STATE_F1(user_input); 

    f2_state_previous = f2_state_current;
    f2_state_current = ITFLIESBY_KEYCODE_STATE_F2(user_input); 

    if (f1_state_previous && !f1_state_current) {
        dev_tools->active = !dev_tools->active;
    }

    if (f2_state_previous && !f2_state_current) {
        dev_tools->demo = !dev_tools->demo;
    }

    //start frame
    platform_api.imgui_frame_start(platform_window_handle);

    if (dev_tools->active) {
        itfliesby_engine_devtools_show(engine);
    }

    if (dev_tools->demo) {
        ImGui::ShowDemoWindow();
    }

    //end frame
    platform_api.imgui_frame_end(platform_window_handle); 
}
