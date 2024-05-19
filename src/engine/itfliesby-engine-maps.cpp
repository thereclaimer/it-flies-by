#include "itfliesby-engine.hpp"
#include "itfliesby-engine-memory.cpp"

internal ItfliesbyEngineMapManager
itfliesby_engine_maps_manager_create_and_init() {

    ItfliesbyEngineMapManager map_manager = {0};

    memory map_base_memory = itfliesby_engine_memory_maps();

    //we are manually managing all the memory for the maps, this is a special thing
    const u32 dynamic_table_count = 3;

    ItfliesbyEngineMapMemory map_memory = {0};
    map_memory.table_memory_size_bytes  = ITFLIESBY_ENGINE_PARTITION_SIZE_MAPS / dynamic_table_count; 
    map_memory.memory_table_maps       = map_base_memory;
    map_memory.memory_table_rooms      = map_base_memory +  map_memory.table_memory_size_bytes;
    map_memory.memory_table_dimensions = map_base_memory + (map_memory.table_memory_size_bytes * 2);

    map_manager.map_memory = map_memory;

    return(map_manager);
}