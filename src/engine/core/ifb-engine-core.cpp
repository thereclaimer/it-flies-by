#pragma once

#include "ifb-engine-core.hpp"

internal IFBEngine_Ptr
ifb_engine_create_and_initialize(
    ItfliesbyPlatformApi platform) {

    //validate the platform api
    ifb_assert(platform.file_size);
    ifb_assert(platform.file_read);
    ifb_assert(platform.file_write);
    ifb_assert(platform.file_open);
    ifb_assert(platform.file_close);
    ifb_assert(platform.memory_allocate);
    ifb_assert(platform.memory_free);
    ifb_assert(platform.ticks);
    ifb_assert(platform.delta_time_ms);
    ifb_assert(platform.sleep);

    ifb_platform_api = platform;

    //initialize the engine
    ifb_engine = {0};

    //memory
    ifb_engine.memory = ifb_engine_memory_create_and_initialize();
    ifb_assert(ifb_engine.memory);
}
