#pragma once

#include "ifb-win32.hpp"

global IFBWin32ImGui ifb_win32_imgui = {0};

internal IFBWin32ImGuiHandle
ifb_win32_imgui_create_and_initialize() {

    IMGUI_CHECKVERSION();

    HWND window_handle = ifb_win32_window_handle();
    ifb_assert(window_handle);

    ImGuiContext* imgui_context = ImGui::CreateContext();
    ifb_assert(imgui_context);

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    b8 impl_init_win32  = ImGui_ImplWin32_InitForOpenGL(window_handle);
    b8 impl_init_opengl = ImGui_ImplOpenGL3_Init();

    ifb_assert(
        impl_init_win32 &&
        impl_init_opengl);

    ifb_win32_imgui.imgui_context = imgui_context;
    ifb_win32_imgui.show_demo     = false;

    return(&ifb_win32_imgui);    
}

internal void
ifb_win32_imgui_frame_start() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

internal void
ifb_win32_imgui_frame_render() {

    if (ifb_win32_imgui.show_demo) {
        ImGui::ShowDemoWindow(&ifb_win32_imgui.show_demo);
    }

    ImGui::Render();

    ImDrawData* imgui_draw_data = ImGui::GetDrawData();
    ifb_assert(imgui_draw_data);

    ImGui_ImplOpenGL3_RenderDrawData(imgui_draw_data);
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

internal void
ifb_win32_imgui_show_demo(
    b8 show_demo) {

    ifb_win32_imgui.show_demo = show_demo;
}