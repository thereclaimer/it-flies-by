#ifndef ITFLIESBY_ENGINE_HPP
#define ITFLIESBY_ENGINE_HPP

#define GLEW_STATIC

#include <glew/glew.h>
#include <glew/wglew.h>
#include <glew/glew.c>
#include <itfliesby.hpp>

#include "itfliesby-engine-globals.hpp"
#include "itfliesby-engine-assets.hpp"
#include "itfliesby-engine-memory.hpp"
#include "itfliesby-engine-physics.hpp"
#include "itfliesby-engine-sprites.hpp"
#include "itfliesby-engine-scene.hpp"
#include "itfliesby-engine-devtools.hpp"

//----------------------------------------------------------------
// FORWARD DECLARATIONS
//----------------------------------------------------------------

typedef struct ItfliesbyEngine;
typedef struct ItfliesbyEngineDevtools;

struct ItfliesbyEngineShaderStore {
    union {
        struct {
            ItfliesbyRendererShaderIndex textured_quad_shader_index;
            ItfliesbyRendererShaderIndex solid_quad_shader_index;
        } instances;
        
        ItfliesbyRendererShaderIndex array[sizeof(instances) / sizeof(ItfliesbyRendererShaderIndex)];
    };
};


struct ItfliesbyEngineRendererMemory {
    itfliesby_memory_partition        partition;
    itfliesby_memory_partition        rendering_context_partition;
    itfliesby_memory_allocator_linear shader_asset_data_allocator;
};

#define ITFLIESBY_ENGINE_RENDERER_MEMORY_PARTITION_SIZE             ITFLIESBY_MATH_MEGABYTES(64)
#define ITFLIESBY_ENGINE_RENDERER_MEMORY_PARTITION_CONTEXT_SIZE     ITFLIESBY_MATH_MEGABYTES(64)
#define ITFLIESBY_ENGINE_RENDERER_MEMORY_ALLOCATOR_SIZE_SHADER_DATA ITFLIESBY_MATH_MEGABYTES(4)


//----------------------------------------------------------------
// DEV TOOLS
//----------------------------------------------------------------

ItfliesbyEngineDevtools
itfliesby_engine_devtools_create_and_init();

void
itfliesby_engine_devtools_update(
    ItfliesbyEngine* engine);

//----------------------------------------------------------------
// MAPS
//----------------------------------------------------------------

enum ItfliesbyEngineMapKeyType : s8 {
    ITFLIESBY_ENGINE_MAP_KEY_TYPE_INVALID     = -1,
    ITFLIESBY_ENGINE_MAP_KEY_TYPE_MAP         =  0,
    ITFLIESBY_ENGINE_MAP_KEY_TYPE_ROOM        =  1,
    ITFLIESBY_ENGINE_MAP_KEY_TYPE_SUBDIVISION =  2,
    ITFLIESBY_ENGINE_MAP_KEY_TYPE_COUNT       =  3,
};

#define ITFLIESBY_ENGINE_MAP_KEYS_MAX 1024

struct ItfliesbyEngineMapKey {
    union {
        struct {

            //this is the index of the key in the key store
            s16 key_index;
            
            //this is the key type
            s8 type;
            

            //this is the index of the map in the map table
            //EVERY key will be mapped to a valid map index 
            s8 map_index;
        };

        s32 value;
    };
};

typedef s8 ItfliesbyEngineMapPrimaryIndex;
typedef s8 ItfliesbyEngineMapForeignIndex;

struct ItfliesbyEngineMapTableKeys { 
    ItfliesbyEngineMapKey keys[ITFLIESBY_ENGINE_MAP_KEYS_MAX];
    b8                    active[ITFLIESBY_ENGINE_MAP_KEYS_MAX];
};

struct ItfliesbyEngineMapDimensions {
    s8  index_table;
    f32 width;
    f32 height;
    f32 bottom_left_x;
    f32 bottom_left_y;
};

struct ItfliesbyEngineMap {
    ItfliesbyEngineMapPrimaryIndex index;
    ItfliesbyEngineMapForeignIndex index_root_room;
    s8                             count_rooms;
    s8                             count_adjacent_maps;
};

struct ItfliesbyEngineMapRoom {
    ItfliesbyEngineMapPrimaryIndex index;
    ItfliesbyEngineMapForeignIndex index_map;
    ItfliesbyEngineMapForeignIndex index_dimensions;
};


struct ItfliesbyEngineMapTableMaps {
    s8* col_count_rooms;
    s8* col_count_adjacent_maps;
    s8  count_rows;
};


struct ItfliesbyEngineMapTableRooms {
    ItfliesbyEngineMapForeignIndex* col_index_map; 
    ItfliesbyEngineMapForeignIndex* col_index_dimensions;
    u8                              count_rows;
};

struct ItfliesbyEngineMapTableDimensions {
    f32* col_bottom_left_x;
    f32* col_bottom_left_y;
    f32* col_width;
    f32* col_height;
    s8   count_rows;
};
struct ItfliesbyEngineMapMemory {
    memory base_memory;
};



struct ItfliesbyEngineMapManager {
    struct {
        ItfliesbyEngineMapTableKeys       keys;
        ItfliesbyEngineMapTableMaps       maps;
        ItfliesbyEngineMapTableRooms      rooms;
        ItfliesbyEngineMapTableDimensions dimensions;
    } tables;
    ItfliesbyEngineMapMemory          map_memory;
};

#define ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_MAPS       (sizeof(s8)  * 2)
#define ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_ROOMS      (sizeof(s8)  * 2)
#define ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_DIMENSIONS (sizeof(f32) * 4)

inline size_t
itfliesby_engine_maps_table_maps_size_bytes(
    ItfliesbyEngineMapTableMaps* maps_table) {
    
    size_t maps_size_bytes = 
        maps_table->count_rows * ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_MAPS;

    return(maps_size_bytes);
}

inline size_t
itfliesby_engine_maps_table_rooms_size_bytes(
    ItfliesbyEngineMapTableRooms* rooms_table) {
    
    size_t rooms_size_bytes = 
        rooms_table->count_rows * ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_ROOMS;

    return(rooms_size_bytes);
}

inline size_t
itfliesby_engine_maps_table_dimensions_size_bytes(
    ItfliesbyEngineMapTableDimensions* dimensions_table) {
    
    size_t dimensions_size_bytes = 
        dimensions_table->count_rows * ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_DIMENSIONS;

    return(dimensions_size_bytes);
}

//----------------------------------------------------------------
// ENGINE CORE
//----------------------------------------------------------------

struct ItfliesbyEngineFrame {
    f64 target_fps;
    f64 target_ms_per_frame;
    f64 delta_time_ms;

    //this is how many frames we wait to calculate averages
    u32 frame_profile_max;
};

struct ItfliesbyEngine {
    ItfliesbyEngineAssets       assets;
    ItfliesbyEngineShaderStore  shaders;
    ItfliesbyRendererHandle     renderer;
    ItfliesbyEnginePhysics      physics;
    ItfliesbyEngineSprites      sprites;
    ItfliesbyEngineSceneManager scenes;
    ItfliesbyUserInput*         user_input;
    ItfliesbyEngineDevtools     dev_tools;
    ItfliesbyEngineFrame        frame;
    ItfliesbyEngineMapManager   map_manager;
};

api ItfliesbyEngine*
itfliesby_engine_create(
    ItfliesbyPlatformApi platform,
    memory               mem,
    u64                  mem_size
);

api void
itfliesby_engine_destroy(
    ItfliesbyEngine* engine
);

api void
itfliesby_engine_render_scene(
          ItfliesbyEngine*    engine,
          ItfliesbyUserInput* user_input,
    const f32                 window_width,
    const f32                 window_height,
    const f32                 screen_width,
    const f32                 screen_height);


api ItfliesbyEngineSceneId
itfliesby_engine_scene_create(
    ItfliesbyEngine* engine);

api void
itfliesby_engine_scene_destroy(
    ItfliesbyEngine*       engine,
    ItfliesbyEngineSceneId scene_id);

api void
itfliesby_engine_scene_set_active(
    ItfliesbyEngine*       engine,
    ItfliesbyEngineSceneId scene_id);

api ItfliesbyEngineSpriteId
itfliesby_engine_scene_connor_test(
    ItfliesbyEngine*       engine,
    ItfliesbyEngineSceneId scene_id);

#endif //ITFLIESBY_ENGINE_HPP