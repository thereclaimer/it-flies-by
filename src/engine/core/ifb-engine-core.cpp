#pragma once

#include "ifb-engine-core.hpp"

internal IFBEngine_Ptr
ifb_engine_create_and_initialize(
    ItfliesbyPlatformApi platform) {

    //validate the platform api
    ITFLIESBY_ASSERT(platform.file_size);
    ITFLIESBY_ASSERT(platform.file_read);
    ITFLIESBY_ASSERT(platform.file_write);
    ITFLIESBY_ASSERT(platform.file_open);
    ITFLIESBY_ASSERT(platform.file_close);
    ITFLIESBY_ASSERT(platform.memory_allocate);
    ITFLIESBY_ASSERT(platform.memory_free);
    ITFLIESBY_ASSERT(platform.graphics_api_init);
    ITFLIESBY_ASSERT(platform.imgui_init);
    ITFLIESBY_ASSERT(platform.imgui_frame_start);
    ITFLIESBY_ASSERT(platform.imgui_frame_end);
    ITFLIESBY_ASSERT(platform.ticks);
    ITFLIESBY_ASSERT(platform.delta_time_ms);
    ITFLIESBY_ASSERT(platform.sleep);

    ifb_platform_api = platform;

    //initialize the engine
    ifb_engine = {0};

    //memory
    ifb_engine.memory = ifb_engine_memory_create_and_initialize();
    ITFLIESBY_ASSERT(ifb_engine.memory);
}
