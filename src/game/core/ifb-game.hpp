#ifndef IFB_GAME_HPP
#define IFB_GAME_HPP

#include <ifb.hpp>
#include <ifb-engine.hpp>

//--------------------------------
// TYPES
//--------------------------------
struct  IFBGame;
typedef IFBGame* IFBGame_Ptr;
typedef IFBGame& IFBGame_Ref;
typedef void*    IFBGameHandle;

//--------------------------------
// GAME
//--------------------------------

struct IFBGame {
    IFBEngineHandle engine;
    IFBUserInput    user_input;
};

external IFBGameHandle
ifb_game_create_and_initialize(
    IFBEngineHandle engine);

external bool
ifb_game_update_and_render(
    IFBUserInput& user_input);

#endif //IFB_GAME_HPP