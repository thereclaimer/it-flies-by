#pragma once

#define IFB_ENGINE_INTERNAL

//--------------------------------
// MEMORY
//--------------------------------
#include "ifb-engine-memory-arena-allocator.cpp"
#include "ifb-engine-memory-arena.cpp"
#include "ifb-engine-memory-region.cpp"
#include "ifb-engine-memory.cpp"

//--------------------------------
// assets
//--------------------------------
#include "ifb-engine-assets.cpp"
#include "ifb-engine-assets-memory.cpp"
#include "ifb-engine-assets-file.cpp"
#include "ifb-engine-assets-data-table.cpp"

//--------------------------------
// RENDERER
//--------------------------------
#include "ifb-engine-renderer.cpp"
#include "ifb-engine-renderer-memory.cpp"
#include "ifb-engine-renderer-shader.cpp"

//--------------------------------
// CORE
//--------------------------------
#include "ifb-engine.cpp"
#include "ifb-engine-core-memory.cpp"
