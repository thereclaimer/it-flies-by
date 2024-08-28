#pragma once

#define IFB_ENGINE_INTERNAL

//--------------------------------
// MEMORY
//--------------------------------
#include "ifb-engine-memory-arena.cpp"
#include "ifb-engine-memory-manager.cpp"
#include "ifb-engine-memory-reservation.cpp"

//--------------------------------
// assets
//--------------------------------
#include "ifb-engine-assets-data.cpp"
#include "ifb-engine-assets-file.cpp"
#include "ifb-engine-assets-memory.cpp"

//--------------------------------
// RENDERER
//--------------------------------
#include "ifb-engine-renderer.cpp"
#include "ifb-engine-renderer-memory.cpp"
#include "ifb-engine-renderer-shader.cpp"
#include "ifb-engine-renderer-shader-solid-quad.cpp"
#include "ifb-engine-renderer-shader-textured-quad.cpp"

//--------------------------------
// CORE
//--------------------------------
#include "ifb-engine.cpp"
#include "ifb-engine-core-memory.cpp"
