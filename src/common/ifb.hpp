#ifndef IFB_HPP
#define IFB_HPP

#include "ifb-dependencies.hpp"
#include "ifb-types.hpp"
#include "ifb-user-input.hpp"
#include "ifb-platform-api.hpp"
#include "ifb-tag.hpp"

/**************************************************
 * FLAGS
 * 
 * [IFB_NO_API]
 * this will exclude the api from the build of whatever includes this header
 * use when you want to use the common types without the overhead of the game
 * 
 * [IFB_ENGINE_INTERNAL]
 * used by the engine only, prevents inclusion of the engine api
 * 
 * [IFB_GAME_INTERNAL]
 * used by the game only, prevents inclusion of the game api
 * 
 **************************************************/

#ifndef  IFB_NO_API
#ifndef  IFB_ENGINE_INTERNAL

#include "ifb-engine.hpp"

#endif   //IFB_ENGINE_INTERNAL
#endif   //IFB_NO_API

#ifndef  IFB_NO_API
#ifndef  IFB_GAME_INTERNAL 
#ifndef  IFB_ENGINE_INTERNAL

#include "ifb-game.hpp"

#endif //IFB_ENGINE_INTERNAL
#endif //IFB_GAME_INTERNAL
#endif //IFB_NO_API

#endif //IFB_HPP