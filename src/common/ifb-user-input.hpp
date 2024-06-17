#ifndef IFB_USER_INPUT_HPP
#define IFB_USER_INPUT_HPP

#include "ifb-types.hpp"

#define IFB_KEYCODE_GROUPS                14
#define IFB_KEYCODE_GROUP_SIZE             8

//keycode group 0
#define IFB_KEYCODE_BACK                   0 
#define IFB_KEYCODE_TAB                    1 
#define IFB_KEYCODE_CLEAR                  2 
#define IFB_KEYCODE_RETURN                 3 
#define IFB_KEYCODE_SHIFT                  4 
#define IFB_KEYCODE_CONTROL                5 
#define IFB_KEYCODE_MENU                   6 
#define IFB_KEYCODE_PAUSE                  7 

//keycode group 1
#define IFB_KEYCODE_CAPITAL                8 
#define IFB_KEYCODE_ESCAPE                 9 
#define IFB_KEYCODE_SPACE                  10 
#define IFB_KEYCODE_PRIOR                  11 
#define IFB_KEYCODE_NEXT                   12 
#define IFB_KEYCODE_END                    13 
#define IFB_KEYCODE_HOME                   14 
#define IFB_KEYCODE_LEFT                   15 

//keycode group 2
#define IFB_KEYCODE_UP                     16 
#define IFB_KEYCODE_RIGHT                  17 
#define IFB_KEYCODE_DOWN                   18 
#define IFB_KEYCODE_SELECT                 19 
#define IFB_KEYCODE_PRINT                  20 
#define IFB_KEYCODE_SNAPSHOT               21 
#define IFB_KEYCODE_INSERT                 22 
#define IFB_KEYCODE_DELETE                 23 

//keycode group 3
#define IFB_KEYCODE_HELP                   24 
#define IFB_KEYCODE_0                      25 
#define IFB_KEYCODE_1                      26 
#define IFB_KEYCODE_2                      27 
#define IFB_KEYCODE_3                      28 
#define IFB_KEYCODE_4                      29 
#define IFB_KEYCODE_5                      30 
#define IFB_KEYCODE_6                      31 

//keycode group 4
#define IFB_KEYCODE_7                      32 
#define IFB_KEYCODE_8                      33 
#define IFB_KEYCODE_9                      34 
#define IFB_KEYCODE_A                      35 
#define IFB_KEYCODE_B                      36 
#define IFB_KEYCODE_C                      37 
#define IFB_KEYCODE_D                      38 
#define IFB_KEYCODE_E                      39 

//keycode group 5
#define IFB_KEYCODE_F                      40 
#define IFB_KEYCODE_G                      41 
#define IFB_KEYCODE_H                      42 
#define IFB_KEYCODE_I                      43 
#define IFB_KEYCODE_J                      44 
#define IFB_KEYCODE_K                      45 
#define IFB_KEYCODE_L                      46 
#define IFB_KEYCODE_M                      47 

//keycode group 6
#define IFB_KEYCODE_N                      48 
#define IFB_KEYCODE_O                      49 
#define IFB_KEYCODE_P                      50 
#define IFB_KEYCODE_Q                      51 
#define IFB_KEYCODE_R                      52 
#define IFB_KEYCODE_S                      53 
#define IFB_KEYCODE_T                      54 
#define IFB_KEYCODE_U                      55 

//keycode group 7
#define IFB_KEYCODE_V                      56 
#define IFB_KEYCODE_W                      57 
#define IFB_KEYCODE_X                      58 
#define IFB_KEYCODE_Y                      59 
#define IFB_KEYCODE_Z                      60 
#define IFB_KEYCODE_LEFT_WIN               61 
#define IFB_KEYCODE_RIGHT_WIN              62 
#define IFB_KEYCODE_APPS                   63 

//keycode group 8
#define IFB_KEYCODE_NUMPAD_0               64 
#define IFB_KEYCODE_NUMPAD_1               65 
#define IFB_KEYCODE_NUMPAD_2               66 
#define IFB_KEYCODE_NUMPAD_3               67 
#define IFB_KEYCODE_NUMPAD_4               68 
#define IFB_KEYCODE_NUMPAD_5               69 
#define IFB_KEYCODE_NUMPAD_6               70 
#define IFB_KEYCODE_NUMPAD_7               71 

//keycode group 9
#define IFB_KEYCODE_NUMPAD_8               72 
#define IFB_KEYCODE_NUMPAD_9               73 
#define IFB_KEYCODE_MULTIPLY               74 
#define IFB_KEYCODE_ADD                    75 
#define IFB_KEYCODE_SEPARATOR              76 
#define IFB_KEYCODE_SUBTRACT               77 
#define IFB_KEYCODE_DECIMAL                78 
#define IFB_KEYCODE_DIVIDE                 79 

//keycode group 10
#define IFB_KEYCODE_F1                     80 
#define IFB_KEYCODE_F2                     81 
#define IFB_KEYCODE_F3                     82 
#define IFB_KEYCODE_F4                     83 
#define IFB_KEYCODE_F5                     84 
#define IFB_KEYCODE_F6                     85 
#define IFB_KEYCODE_F7                     86 
#define IFB_KEYCODE_F8                     87 

//keycode group 11
#define IFB_KEYCODE_F9                     88 
#define IFB_KEYCODE_F10                    89 
#define IFB_KEYCODE_F11                    90 
#define IFB_KEYCODE_F12                    91 
#define IFB_KEYCODE_NUMLOCK                92 
#define IFB_KEYCODE_LSHIFT                 93 
#define IFB_KEYCODE_RSHIFT                 94 
#define IFB_KEYCODE_LEFT_CONTROL           95 

//keycode group 12
#define IFB_KEYCODE_RIGHT_CONTROL          96 
#define IFB_KEYCODE_LEFT_MENU              97 
#define IFB_KEYCODE_RIGHT_MENU             98 
#define IFB_KEYCODE_VOLUME_MUTE            99 
#define IFB_KEYCODE_VOLUME_DOWN            100 
#define IFB_KEYCODE_VOLUME_UP              101 
#define IFB_KEYCODE_PLUS                   102 
#define IFB_KEYCODE_COMMA                  103 

//keycode group 13
#define IFB_KEYCODE_MINUS                  104 
#define IFB_KEYCODE_PERIOD                 105 
#define IFB_KEYCODE_FORWARD_SLASH_QUESTION 106 
#define IFB_KEYCODE_TILDE                  107 
#define IFB_KEYCODE_OPEN_BRACKETS          108 
#define IFB_KEYCODE_BACK_SLASH_PIPE        109 
#define IFB_KEYCODE_CLOSED_BRACKET         110 
#define IFB_KEYCODE_SINGLE_DOUBLE_QUOTE    111 

#define IFB_GAMEPAD_BUTTONCODE_DPAD_UP      0
#define IFB_GAMEPAD_BUTTONCODE_DPAD_DOWN    1 
#define IFB_GAMEPAD_BUTTONCODE_DPAD_LEFT    2
#define IFB_GAMEPAD_BUTTONCODE_DPAD_RIGHT   3
#define IFB_GAMEPAD_BUTTONCODE_START        4
#define IFB_GAMEPAD_BUTTONCODE_SELECT       5
#define IFB_GAMEPAD_BUTTONCODE_STICK_LEFT   6
#define IFB_GAMEPAD_BUTTONCODE_STICK_RIGHT  7
#define IFB_GAMEPAD_BUTTONCODE_BUMPER_LEFT  8
#define IFB_GAMEPAD_BUTTONCODE_BUMPER_RIGHT 9
#define IFB_GAMEPAD_BUTTONCODE_BPAD_A       12
#define IFB_GAMEPAD_BUTTONCODE_BPAD_B       13 
#define IFB_GAMEPAD_BUTTONCODE_BPAD_X       14
#define IFB_GAMEPAD_BUTTONCODE_BPAD_Y       15

#define ifb_gamepad_button_state_dpad_up(user_input)      user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_DPAD_UP) 
#define ifb_gamepad_button_state_dpad_down(user_input)    user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_DPAD_DOWN) 
#define ifb_gamepad_button_state_dpad_left(user_input)    user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_DPAD_LEFT) 
#define ifb_gamepad_button_state_dpad_right(user_input)   user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_DPAD_RIGHT) 
#define ifb_gamepad_button_state_start(user_input)        user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_START) 
#define ifb_gamepad_button_state_select(user_input)       user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_SELECT) 
#define ifb_gamepad_button_state_stick_left(user_input)   user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_STICK_LEFT) 
#define ifb_gamepad_button_state_stick_right(user_input)  user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_STICK_RIGHT) 
#define ifb_gamepad_button_state_bumper_left(user_input)  user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_BUMPER_LEFT) 
#define ifb_gamepad_button_state_bumper_right(user_input) user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_BUMPER_RIGHT) 
#define ifb_gamepad_button_state_bpad_a(user_input)       user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_BPAD_A) 
#define ifb_gamepad_button_state_bpad_b(user_input)       user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_BPAD_B) 
#define ifb_gamepad_button_state_bpad_x(user_input)       user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_BPAD_X) 
#define ifb_gamepad_button_state_bpad_y(user_input)       user_input.game_pad != NULL && ifb_gamepad_button_state(user_input.game_pad->buttons, IFB_GAMEPAD_BUTTONCODE_BPAD_Y) 

struct IFBGamepadInput {
    f32 trigger_right;
    f32 trigger_left;
    u16 buttons;
    f32 stick_right_x;
    f32 stick_right_y;
    f32 stick_left_x;
    f32 stick_left_y;
};

struct IFBUserInput {
    b8  mouse_button_left;
    b8  mouse_button_right;
    s32 mouse_x_current;
    s32 mouse_y_current;
    s32 mouse_x_delta;
    s32 mouse_y_delta;
    s32 mouse_wheel;
    //there are 112 keycodes, so if every keycode is represented by
    //a bit, we can store the data in a contiguous space created by
    //14 bytes
    u8 keycodes[IFB_KEYCODE_GROUPS];
    IFBGamepadInput* game_pad;
};
typedef IFBGamepadInput user_input;

#define KEY_UP    1
#define KEY_DOWN -1

#define ifb_gamepad_button_state(buttons, button_code) ((buttons >> button_code) & 1) 

#define USER_INPUT_KEY_STATE(val_user_input, val_keycode) (val_user_input.keycodes[(val_keycode/IFB_KEYCODE_GROUP_SIZE)] >> ((val_keycode % IFB_KEYCODE_GROUP_SIZE)) & 1)
#define USER_INPUT_SET_KEY(val_user_input, val_keycode)   (val_user_input.keycodes[(val_keycode/IFB_KEYCODE_GROUP_SIZE)] |= (1 << (val_keycode % IFB_KEYCODE_GROUP_SIZE)))
#define USER_INPUT_CLEAR_KEY(val_user_input, val_keycode) (val_user_input.keycodes[(val_keycode/IFB_KEYCODE_GROUP_SIZE)] &= ~(1 << (val_keycode % IFB_KEYCODE_GROUP_SIZE)))

#define IFB_KEYCODE_STATE_BACK(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_BACK)                   
#define IFB_KEYCODE_STATE_TAB(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_TAB)                    
#define IFB_KEYCODE_STATE_CLEAR(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_CLEAR)                  
#define IFB_KEYCODE_STATE_RETURN(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_RETURN)                 
#define IFB_KEYCODE_STATE_SHIFT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_SHIFT)                  
#define IFB_KEYCODE_STATE_CONTROL(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_CONTROL)                
#define IFB_KEYCODE_STATE_MENU(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_MENU)                   
#define IFB_KEYCODE_STATE_PAUSE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_PAUSE)                  
#define IFB_KEYCODE_STATE_CAPITAL(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_CAPITAL)                
#define IFB_KEYCODE_STATE_ESCAPE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_ESCAPE)                 
#define IFB_KEYCODE_STATE_SPACE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_SPACE)                  
#define IFB_KEYCODE_STATE_PRIOR(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_PRIOR)                  
#define IFB_KEYCODE_STATE_NEXT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NEXT)                   
#define IFB_KEYCODE_STATE_END(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_END)                    
#define IFB_KEYCODE_STATE_HOME(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_HOME)                   
#define IFB_KEYCODE_STATE_LEFT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_LEFT)                   
#define IFB_KEYCODE_STATE_UP(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_UP)                     
#define IFB_KEYCODE_STATE_RIGHT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_RIGHT)                  
#define IFB_KEYCODE_STATE_DOWN(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_DOWN)                   
#define IFB_KEYCODE_STATE_SELECT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_SELECT)                 
#define IFB_KEYCODE_STATE_PRINT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_PRINT)                  
#define IFB_KEYCODE_STATE_SNAPSHOT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_SNAPSHOT)               
#define IFB_KEYCODE_STATE_INSERT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_INSERT)                 
#define IFB_KEYCODE_STATE_DELETE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_DELETE)                 
#define IFB_KEYCODE_STATE_HELP(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_HELP)                   
#define IFB_KEYCODE_STATE_0(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_0)                      
#define IFB_KEYCODE_STATE_1(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_1)                      
#define IFB_KEYCODE_STATE_2(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_2)                      
#define IFB_KEYCODE_STATE_3(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_3)                      
#define IFB_KEYCODE_STATE_4(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_4)                      
#define IFB_KEYCODE_STATE_5(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_5)                      
#define IFB_KEYCODE_STATE_6(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_6)                      
#define IFB_KEYCODE_STATE_7(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_7)                      
#define IFB_KEYCODE_STATE_8(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_8)                      
#define IFB_KEYCODE_STATE_9(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_9)                      
#define IFB_KEYCODE_STATE_A(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_A)                      
#define IFB_KEYCODE_STATE_B(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_B)                      
#define IFB_KEYCODE_STATE_C(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_C)                      
#define IFB_KEYCODE_STATE_D(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_D)                      
#define IFB_KEYCODE_STATE_E(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_E)                      
#define IFB_KEYCODE_STATE_F(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F)                      
#define IFB_KEYCODE_STATE_G(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_G)                      
#define IFB_KEYCODE_STATE_H(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_H)                      
#define IFB_KEYCODE_STATE_I(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_I)                      
#define IFB_KEYCODE_STATE_J(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_J)                      
#define IFB_KEYCODE_STATE_K(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_K)                      
#define IFB_KEYCODE_STATE_L(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_L)                      
#define IFB_KEYCODE_STATE_M(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_M)                      
#define IFB_KEYCODE_STATE_N(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_N)                      
#define IFB_KEYCODE_STATE_O(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_O)                      
#define IFB_KEYCODE_STATE_P(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_P)                      
#define IFB_KEYCODE_STATE_Q(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_Q)                      
#define IFB_KEYCODE_STATE_R(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_R)                      
#define IFB_KEYCODE_STATE_S(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_S)                      
#define IFB_KEYCODE_STATE_T(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_T)                      
#define IFB_KEYCODE_STATE_U(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_U)                      
#define IFB_KEYCODE_STATE_V(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_V)                      
#define IFB_KEYCODE_STATE_W(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_W)                      
#define IFB_KEYCODE_STATE_X(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_X)                      
#define IFB_KEYCODE_STATE_Y(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_Y)                      
#define IFB_KEYCODE_STATE_Z(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_Z)                      
#define IFB_KEYCODE_STATE_LEFT_WIN(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_LEFT_WIN)               
#define IFB_KEYCODE_STATE_RIGHT_WIN(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_RIGHT_WIN)              
#define IFB_KEYCODE_STATE_APPS(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_APPS)                   
#define IFB_KEYCODE_STATE_NUMPAD_0(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_0)               
#define IFB_KEYCODE_STATE_NUMPAD_1(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_1)               
#define IFB_KEYCODE_STATE_NUMPAD_2(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_2)               
#define IFB_KEYCODE_STATE_NUMPAD_3(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_3)               
#define IFB_KEYCODE_STATE_NUMPAD_4(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_4)               
#define IFB_KEYCODE_STATE_NUMPAD_5(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_5)               
#define IFB_KEYCODE_STATE_NUMPAD_6(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_6)               
#define IFB_KEYCODE_STATE_NUMPAD_7(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_7)               
#define IFB_KEYCODE_STATE_NUMPAD_8(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_8)               
#define IFB_KEYCODE_STATE_NUMPAD_9(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMPAD_9)               
#define IFB_KEYCODE_STATE_MULTIPLY(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_MULTIPLY)               
#define IFB_KEYCODE_STATE_ADD(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_ADD)                    
#define IFB_KEYCODE_STATE_SEPARATOR(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_SEPARATOR)              
#define IFB_KEYCODE_STATE_SUBTRACT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_SUBTRACT)               
#define IFB_KEYCODE_STATE_DECIMAL(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_DECIMAL)                
#define IFB_KEYCODE_STATE_DIVIDE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_DIVIDE)                 
#define IFB_KEYCODE_STATE_F1(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F1)                     
#define IFB_KEYCODE_STATE_F2(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F2)                     
#define IFB_KEYCODE_STATE_F3(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F3)                     
#define IFB_KEYCODE_STATE_F4(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F4)                     
#define IFB_KEYCODE_STATE_F5(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F5)                     
#define IFB_KEYCODE_STATE_F6(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F6)                     
#define IFB_KEYCODE_STATE_F7(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F7)                     
#define IFB_KEYCODE_STATE_F8(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F8)                     
#define IFB_KEYCODE_STATE_F9(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F9)                     
#define IFB_KEYCODE_STATE_F10(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F10)                    
#define IFB_KEYCODE_STATE_F11(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F11)                    
#define IFB_KEYCODE_STATE_F12(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_F12)                    
#define IFB_KEYCODE_STATE_NUMLOCK(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_NUMLOCK)                
#define IFB_KEYCODE_STATE_LSHIFT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_LSHIFT)                 
#define IFB_KEYCODE_STATE_RSHIFT(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_RSHIFT)                 
#define IFB_KEYCODE_STATE_LEFT_CONTROL(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_LEFT_CONTROL)           
#define IFB_KEYCODE_STATE_RIGHT_CONTROL(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_RIGHT_CONTROL)          
#define IFB_KEYCODE_STATE_LEFT_MENU(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_LEFT_MENU)              
#define IFB_KEYCODE_STATE_RIGHT_MENU(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_RIGHT_MENU)             
#define IFB_KEYCODE_STATE_VOLUME_MUTE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_VOLUME_MUTE)            
#define IFB_KEYCODE_STATE_VOLUME_DOWN(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_VOLUME_DOWN)            
#define IFB_KEYCODE_STATE_VOLUME_UP(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_VOLUME_UP)              
#define IFB_KEYCODE_STATE_PLUS(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_PLUS)                   
#define IFB_KEYCODE_STATE_COMMA(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_COMMA)                  
#define IFB_KEYCODE_STATE_MINUS(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_MINUS)                  
#define IFB_KEYCODE_STATE_PERIOD(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_PERIOD)                 
#define IFB_KEYCODE_STATE_FORWARD_SLASH_QUESTION(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_FORWARD_SLASH_QUESTION) 
#define IFB_KEYCODE_STATE_TILDE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_TILDE)                  
#define IFB_KEYCODE_STATE_OPEN_BRACKETS(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_OPEN_BRACKETS)          
#define IFB_KEYCODE_STATE_BACK_SLASH_PIPE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_BACK_SLASH_PIPE)        
#define IFB_KEYCODE_STATE_CLOSED_BRACKET(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_CLOSED_BRACKET)         
#define IFB_KEYCODE_STATE_SINGLE_DOUBLE_QUOTE(user_input) USER_INPUT_KEY_STATE(user_input,IFB_KEYCODE_SINGLE_DOUBLE_QUOTE)    

#endif //IFB_USER_INPUT_HPP