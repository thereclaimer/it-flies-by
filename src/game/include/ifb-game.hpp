#ifndef IFB_GAME_HPP
#define IFB_GAME_HPP

#include <ifb.hpp>

typedef void* IFBGameHandle;

api IFBGameHandle
ifb_game_create_and_initialize(
    IFBEngineHandle engine);

api bool
ifb_game_update_and_render(
    IFBUserInput& user_input);

#endif //IFB_GAME_HPP