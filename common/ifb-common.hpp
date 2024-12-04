#ifndef IFB_COMMON_HPP
#define IFB_COMMON_HPP

#include "ifb-common-aspect-ratio.hpp"
#include "ifb-common-color.hpp"
#include "ifb-common-dependencies.hpp"
#include "ifb-common-hash.hpp"
#include "ifb-common-macros.hpp"
#include "ifb-common-memory.hpp"
#include "ifb-common-resolution.hpp"
#include "ifb-common-scopes.hpp"
#include "ifb-common-stack.hpp"
#include "ifb-common-types.hpp"
#include "ifb-common-user-input.hpp"
#include "ifb-common-platform.hpp"

struct IFBDimensions {
    ifb_u32 width;
    ifb_u32 height;
    ifb_u32 position_x;
    ifb_u32 position_y;
};

struct IFBArenaId {
    ifb_index arena_index;
    ifb_index tag_index;
};

struct IFBArena {
    IFBArenaId arena_id;
    ifb_u32    page_start;
    ifb_u32    page_count;
    ifb_memory start;
    ifb_cstr   tag;
};

struct IFBTagId {
    ifb_index index;
};

struct IFBTag {
    IFBTagId id;
    ifb_cstr value;
    IFBHash  hash;
};

#endif //IFB_COMMON_HPP