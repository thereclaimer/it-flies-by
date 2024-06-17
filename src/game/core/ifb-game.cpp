#pragma once

#include "ifb-game.hpp"

global IFBGame ifb_game;

external IFBGameHandle
ifb_game_create_and_initialize(
    IFBEngineHandle engine) {

    ifb_assert(engine);

    ifb_game.engine = engine;

    return(&ifb_game);
}

external bool
ifb_game_update_and_render(
    IFBUserInput& user_input) {

    ifb_game.user_input = user_input;

    return(true);
}