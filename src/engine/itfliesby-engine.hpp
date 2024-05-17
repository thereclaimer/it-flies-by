#ifndef ITFLIESBY_ENGINE_HPP
#define ITFLIESBY_ENGINE_HPP

#define GLEW_STATIC

#include <glew/glew.h>
#include <glew/wglew.h>
#include <glew/glew.c>
#include <imgui/imgui_core.h>
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

struct ItfliesbyEngineDevtools {
    ImGuiContext* imgui_context;
};

ItfliesbyEngineDevtools
itfliesby_engine_devtools_create_and_init();

void
itfliesby_engine_devtools_update(
    ItfliesbyEngine* engine);

inline void
itfliesby_engine_devtools_frame_start(ImGuiContext* context) {
    
    ImGui::SetCurrentContext(context);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

inline void
itfliesby_engine_devtools_frame_end() {

    ImGui::Render();

    ImDrawData* draw_data = ImGui::GetDrawData();

    ImGui_ImplOpenGL3_RenderDrawData(draw_data);    
}

//----------------------------------------------------------------
// ENGINE CORE
//----------------------------------------------------------------

struct ItfliesbyEngineFrame {
    f64 target_fps;
    f64 delta_time_ms;
    u32 frame_index;

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