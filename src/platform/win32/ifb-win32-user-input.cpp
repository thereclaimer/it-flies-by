#pragma once

#include "ifb-win32.hpp"

#define WIN32_GAMEPAD_STICK_DEADZONE_SQUARED 5000
#define WIN32_GAMEPAD_STICK_MAX_NEGATIVE     0.000030517578125f
#define WIN32_GAMEPAD_STICK_MAX_POSITIVE     0.000030518509475f
#define WIN32_GAMEPAD_TRIGGER_MAX            255.0f

internal b8
ifb_win32_get_xinput_state(
    IFBGamepadInput* gamepad) {

    local XINPUT_STATE xinput_state = {0};

    if (XInputGetState(0,&xinput_state) == ERROR_SUCCESS) {
        
        //reset the input
        *gamepad = {0};

        //check if the sticks are in the deadzone
        s32 stick_left_x_corrected  = (xinput_state.Gamepad.sThumbLX ^ 2) > WIN32_GAMEPAD_STICK_DEADZONE_SQUARED || (xinput_state.Gamepad.sThumbLX ^ 2) < -WIN32_GAMEPAD_STICK_DEADZONE_SQUARED ? xinput_state.Gamepad.sThumbLX : 0;
        s32 stick_left_y_corrected  = (xinput_state.Gamepad.sThumbLY ^ 2) > WIN32_GAMEPAD_STICK_DEADZONE_SQUARED || (xinput_state.Gamepad.sThumbLY ^ 2) < -WIN32_GAMEPAD_STICK_DEADZONE_SQUARED ? xinput_state.Gamepad.sThumbLY : 0;
        s32 stick_right_x_corrected = (xinput_state.Gamepad.sThumbRX ^ 2) > WIN32_GAMEPAD_STICK_DEADZONE_SQUARED || (xinput_state.Gamepad.sThumbRX ^ 2) < -WIN32_GAMEPAD_STICK_DEADZONE_SQUARED ? xinput_state.Gamepad.sThumbRX : 0;
        s32 stick_right_y_corrected = (xinput_state.Gamepad.sThumbRY ^ 2) > WIN32_GAMEPAD_STICK_DEADZONE_SQUARED || (xinput_state.Gamepad.sThumbRY ^ 2) < -WIN32_GAMEPAD_STICK_DEADZONE_SQUARED ? xinput_state.Gamepad.sThumbRY : 0;

        //set the stick inputs
        gamepad->stick_left_x  = stick_left_x_corrected < 0 ? (stick_left_x_corrected * WIN32_GAMEPAD_STICK_MAX_NEGATIVE) : (stick_left_x_corrected * WIN32_GAMEPAD_STICK_MAX_POSITIVE);
        gamepad->stick_left_y  = stick_left_y_corrected < 0 ? (stick_left_y_corrected * WIN32_GAMEPAD_STICK_MAX_NEGATIVE) : (stick_left_y_corrected * WIN32_GAMEPAD_STICK_MAX_POSITIVE);
        gamepad->stick_right_x = stick_right_x_corrected < 0 ? (stick_right_x_corrected * WIN32_GAMEPAD_STICK_MAX_NEGATIVE) : (stick_right_x_corrected * WIN32_GAMEPAD_STICK_MAX_POSITIVE);
        gamepad->stick_right_y = stick_right_y_corrected < 0 ? (stick_right_y_corrected * WIN32_GAMEPAD_STICK_MAX_NEGATIVE) : (stick_right_y_corrected * WIN32_GAMEPAD_STICK_MAX_POSITIVE);
        
        gamepad->trigger_left  = xinput_state.Gamepad.bLeftTrigger /  WIN32_GAMEPAD_TRIGGER_MAX;
        gamepad->trigger_right = xinput_state.Gamepad.bRightTrigger / WIN32_GAMEPAD_TRIGGER_MAX;

        gamepad->buttons = xinput_state.Gamepad.wButtons;
        

        return(true);
    }
    return(false);
}

internal u32
ifb_win32_translate_user_keycode(
    u32 keycode) {

    u32 ifb_keycode = 0;

    switch (keycode ) {

        case VK_BACK:        { ifb_keycode = IFB_KEYCODE_BACK; } break;
        case VK_TAB:         { ifb_keycode = IFB_KEYCODE_TAB; } break;
        case VK_CLEAR:       { ifb_keycode = IFB_KEYCODE_CLEAR; } break;
        case VK_RETURN:      { ifb_keycode = IFB_KEYCODE_RETURN; } break;
        case VK_SHIFT:       { ifb_keycode = IFB_KEYCODE_SHIFT; } break;
        case VK_CONTROL:     { ifb_keycode = IFB_KEYCODE_CONTROL; } break;
        case VK_MENU:        { ifb_keycode = IFB_KEYCODE_MENU; } break;
        case VK_PAUSE:       { ifb_keycode = IFB_KEYCODE_PAUSE; } break;
        case VK_CAPITAL:     { ifb_keycode = IFB_KEYCODE_CAPITAL; } break;
        case VK_ESCAPE:      { ifb_keycode = IFB_KEYCODE_ESCAPE; } break;
        case VK_SPACE:       { ifb_keycode = IFB_KEYCODE_SPACE; } break;
        case VK_PRIOR:       { ifb_keycode = IFB_KEYCODE_PRIOR; } break;
        case VK_NEXT:        { ifb_keycode = IFB_KEYCODE_NEXT; } break;
        case VK_END:         { ifb_keycode = IFB_KEYCODE_END; } break;
        case VK_HOME:        { ifb_keycode = IFB_KEYCODE_HOME; } break;
        case VK_LEFT:        { ifb_keycode = IFB_KEYCODE_LEFT; } break;
        case VK_UP:          { ifb_keycode = IFB_KEYCODE_UP; } break;
        case VK_RIGHT:       { ifb_keycode = IFB_KEYCODE_RIGHT; } break;
        case VK_DOWN:        { ifb_keycode = IFB_KEYCODE_DOWN; } break;
        case VK_SELECT:      { ifb_keycode = IFB_KEYCODE_SELECT; } break;
        case VK_PRINT:       { ifb_keycode = IFB_KEYCODE_PRINT; } break;
        case VK_SNAPSHOT:    { ifb_keycode = IFB_KEYCODE_SNAPSHOT; } break;
        case VK_INSERT:      { ifb_keycode = IFB_KEYCODE_INSERT; } break;
        case VK_DELETE:      { ifb_keycode = IFB_KEYCODE_DELETE; } break;
        case VK_HELP:        { ifb_keycode = IFB_KEYCODE_HELP; } break;
        case 0x30:           { ifb_keycode = IFB_KEYCODE_0; } break;
        case 0x31:           { ifb_keycode = IFB_KEYCODE_1; } break;
        case 0x32:           { ifb_keycode = IFB_KEYCODE_2; } break;
        case 0x33:           { ifb_keycode = IFB_KEYCODE_3; } break;
        case 0x34:           { ifb_keycode = IFB_KEYCODE_4; } break;
        case 0x35:           { ifb_keycode = IFB_KEYCODE_5; } break;
        case 0x36:           { ifb_keycode = IFB_KEYCODE_6; } break;
        case 0x37:           { ifb_keycode = IFB_KEYCODE_7; } break;
        case 0x38:           { ifb_keycode = IFB_KEYCODE_8; } break;
        case 0x39:           { ifb_keycode = IFB_KEYCODE_9; } break;
        case 0x41:           { ifb_keycode = IFB_KEYCODE_A; } break;
        case 0x42:           { ifb_keycode = IFB_KEYCODE_B; } break;
        case 0x43:           { ifb_keycode = IFB_KEYCODE_C; } break;
        case 0x44:           { ifb_keycode = IFB_KEYCODE_D; } break;
        case 0x45:           { ifb_keycode = IFB_KEYCODE_E; } break;
        case 0x46:           { ifb_keycode = IFB_KEYCODE_F; } break;
        case 0x47:           { ifb_keycode = IFB_KEYCODE_G; } break;
        case 0x48:           { ifb_keycode = IFB_KEYCODE_H; } break;
        case 0x49:           { ifb_keycode = IFB_KEYCODE_I; } break;
        case 0x4A:           { ifb_keycode = IFB_KEYCODE_J; } break;
        case 0x4B:           { ifb_keycode = IFB_KEYCODE_K; } break;
        case 0x4C:           { ifb_keycode = IFB_KEYCODE_L; } break;
        case 0x4D:           { ifb_keycode = IFB_KEYCODE_M; } break;
        case 0x4E:           { ifb_keycode = IFB_KEYCODE_N; } break;
        case 0x4F:           { ifb_keycode = IFB_KEYCODE_O; } break;
        case 0x50:           { ifb_keycode = IFB_KEYCODE_P; } break;
        case 0x51:           { ifb_keycode = IFB_KEYCODE_Q; } break;
        case 0x52:           { ifb_keycode = IFB_KEYCODE_R; } break;
        case 0x53:           { ifb_keycode = IFB_KEYCODE_S; } break;
        case 0x54:           { ifb_keycode = IFB_KEYCODE_T; } break;
        case 0x55:           { ifb_keycode = IFB_KEYCODE_U; } break;
        case 0x56:           { ifb_keycode = IFB_KEYCODE_V; } break;
        case 0x57:           { ifb_keycode = IFB_KEYCODE_W; } break;
        case 0x58:           { ifb_keycode = IFB_KEYCODE_X; } break;
        case 0x59:           { ifb_keycode = IFB_KEYCODE_Y; } break;
        case 0x5A:           { ifb_keycode = IFB_KEYCODE_Z; } break;
        case VK_LWIN:        { ifb_keycode = IFB_KEYCODE_LEFT_WIN; } break;
        case VK_RWIN:        { ifb_keycode = IFB_KEYCODE_RIGHT_WIN; } break;
        case VK_APPS:        { ifb_keycode = IFB_KEYCODE_APPS; } break;
        case VK_NUMPAD0:     { ifb_keycode = IFB_KEYCODE_NUMPAD_0; } break;
        case VK_NUMPAD1:     { ifb_keycode = IFB_KEYCODE_NUMPAD_1; } break;
        case VK_NUMPAD2:     { ifb_keycode = IFB_KEYCODE_NUMPAD_2; } break;
        case VK_NUMPAD3:     { ifb_keycode = IFB_KEYCODE_NUMPAD_3; } break;
        case VK_NUMPAD4:     { ifb_keycode = IFB_KEYCODE_NUMPAD_4; } break;
        case VK_NUMPAD5:     { ifb_keycode = IFB_KEYCODE_NUMPAD_5; } break;
        case VK_NUMPAD6:     { ifb_keycode = IFB_KEYCODE_NUMPAD_6; } break;
        case VK_NUMPAD7:     { ifb_keycode = IFB_KEYCODE_NUMPAD_7; } break;
        case VK_NUMPAD8:     { ifb_keycode = IFB_KEYCODE_NUMPAD_8; } break;
        case VK_NUMPAD9:     { ifb_keycode = IFB_KEYCODE_NUMPAD_9; } break;
        case VK_MULTIPLY:    { ifb_keycode = IFB_KEYCODE_MULTIPLY; } break;
        case VK_ADD:         { ifb_keycode = IFB_KEYCODE_ADD; } break;
        case VK_SEPARATOR:   { ifb_keycode = IFB_KEYCODE_SEPARATOR; } break;
        case VK_SUBTRACT:    { ifb_keycode = IFB_KEYCODE_SUBTRACT; } break;
        case VK_DECIMAL:     { ifb_keycode = IFB_KEYCODE_DECIMAL; } break;
        case VK_DIVIDE:      { ifb_keycode = IFB_KEYCODE_DIVIDE; } break;
        case VK_F1:          { ifb_keycode = IFB_KEYCODE_F1; } break;
        case VK_F2:          { ifb_keycode = IFB_KEYCODE_F2; } break;
        case VK_F3:          { ifb_keycode = IFB_KEYCODE_F3; } break;
        case VK_F4:          { ifb_keycode = IFB_KEYCODE_F4; } break;
        case VK_F5:          { ifb_keycode = IFB_KEYCODE_F5; } break;
        case VK_F6:          { ifb_keycode = IFB_KEYCODE_F6; } break;
        case VK_F7:          { ifb_keycode = IFB_KEYCODE_F7; } break;
        case VK_F8:          { ifb_keycode = IFB_KEYCODE_F8; } break;
        case VK_F9:          { ifb_keycode = IFB_KEYCODE_F9; } break;
        case VK_F10:         { ifb_keycode = IFB_KEYCODE_F10; } break;
        case VK_F11:         { ifb_keycode = IFB_KEYCODE_F11; } break;
        case VK_F12:         { ifb_keycode = IFB_KEYCODE_F12; } break;
        case VK_NUMLOCK:     { ifb_keycode = IFB_KEYCODE_NUMLOCK; } break;
        case VK_LSHIFT:      { ifb_keycode = IFB_KEYCODE_LSHIFT; } break;
        case VK_RSHIFT:      { ifb_keycode = IFB_KEYCODE_RSHIFT; } break;
        case VK_LCONTROL:    { ifb_keycode = IFB_KEYCODE_LEFT_CONTROL; } break;
        case VK_RCONTROL:    { ifb_keycode = IFB_KEYCODE_RIGHT_CONTROL; } break;
        case VK_LMENU:       { ifb_keycode = IFB_KEYCODE_LEFT_MENU; } break;
        case VK_RMENU:       { ifb_keycode = IFB_KEYCODE_RIGHT_MENU; } break;
        case VK_VOLUME_MUTE: { ifb_keycode = IFB_KEYCODE_VOLUME_MUTE; } break;
        case VK_VOLUME_DOWN: { ifb_keycode = IFB_KEYCODE_VOLUME_DOWN; } break;
        case VK_VOLUME_UP:   { ifb_keycode = IFB_KEYCODE_VOLUME_UP; } break;
        case VK_OEM_PLUS:    { ifb_keycode = IFB_KEYCODE_PLUS; } break;
        case VK_OEM_COMMA:   { ifb_keycode = IFB_KEYCODE_COMMA; } break;
        case VK_OEM_MINUS:   { ifb_keycode = IFB_KEYCODE_MINUS; } break;
        case VK_OEM_PERIOD:  { ifb_keycode = IFB_KEYCODE_PERIOD; } break;
        case VK_OEM_2:       { ifb_keycode = IFB_KEYCODE_FORWARD_SLASH_QUESTION; } break;
        case VK_OEM_3:       { ifb_keycode = IFB_KEYCODE_TILDE; } break;
        case VK_OEM_4:       { ifb_keycode = IFB_KEYCODE_OPEN_BRACKETS; } break;
        case VK_OEM_5:       { ifb_keycode = IFB_KEYCODE_BACK_SLASH_PIPE; } break;
        case VK_OEM_6:       { ifb_keycode = IFB_KEYCODE_CLOSED_BRACKET; } break;
        case VK_OEM_7:       { ifb_keycode = IFB_KEYCODE_SINGLE_DOUBLE_QUOTE; } break;
        default: break;
    }

    return(ifb_keycode);
}
