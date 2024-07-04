#ifndef IFB_HPP
#define IFB_HPP



#include "ifb-dependencies.hpp"
#include "ifb-types.hpp"
#include "ifb-user-input.hpp"
#include "ifb-platform-api.hpp"
#include "ifb-tag.hpp"

#ifndef IFB_ENGINE_INTERNAL
#include "ifb-engine.hpp"
#endif //IFB_ENGINE_INTERNAL

#ifndef  IFB_GAME_INTERNAL 
#ifndef  IFB_ENGINE_INTERNAL
#include "ifb-game.hpp"
#endif //IFB_ENGINE_INTERNAL
#endif //IFB_GAME_INTERNAL

#endif //IFB_HPP