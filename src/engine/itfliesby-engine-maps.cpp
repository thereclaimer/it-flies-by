#include "itfliesby-engine.hpp"
#include "itfliesby-engine-memory.cpp"

internal ItfliesbyEngineMapManager
itfliesby_engine_maps_manager_create_and_init() {

    ItfliesbyEngineMapManager map_manager = {0};

    memory map_base_memory = itfliesby_engine_memory_maps();

    //now, we can start assigning base addresses to the tables
    ItfliesbyEngineMapTableMaps table_maps = {0};
    table_maps.count_rows              = 0;
    table_maps.col_count_rooms         = (s8*)(map_base_memory);    
    table_maps.col_count_adjacent_maps = (s8*)(map_base_memory + 2);    

    ItfliesbyEngineMapTableRooms table_rooms = {0};    
    table_rooms.count_rows           = 0;
    table_rooms.col_index_map        = (ItfliesbyEngineMapForeignIndex*)(map_base_memory + 3);
    table_rooms.col_index_dimensions = (ItfliesbyEngineMapForeignIndex*)(map_base_memory + 4);
    
    ItfliesbyEngineMapTableDimensions table_dimensions = {0};
    table_dimensions.count_rows        = 0; 
    table_dimensions.col_bottom_left_x = (f32*)(map_base_memory + 5);
    table_dimensions.col_bottom_left_y = (f32*)(map_base_memory + 6);
    table_dimensions.col_width         = (f32*)(map_base_memory + 7); 
    table_dimensions.col_height        = (f32*)(map_base_memory + 8); 

    map_manager.tables.maps             = table_maps;
    map_manager.tables.rooms            = table_rooms;
    map_manager.tables.dimensions       = table_dimensions;
    map_manager.map_memory.base_memory  = map_base_memory;

    return(map_manager);
}

internal ItfliesbyEngineMapKey
itfliesby_engine_maps_manager_find_and_activate_key(
    ItfliesbyEngineMapTableKeys* key_table,
    ItfliesbyEngineMapKeyType    key_type,
    s8                           map_index) {

    ItfliesbyEngineMapKey* key_table_keys   = key_table->keys;
    b8*                    key_table_active = key_table->active;

    //initilalize the new key
    ItfliesbyEngineMapKey new_key = {0};
    new_key.key_index = ITFLIESBY_ENGINE_MAP_KEY_TYPE_INVALID;
    new_key.type      = key_type;
    new_key.map_index = map_index; 

    //find a new inactive key index
    for (
        u16 key_index = 0;
        key_index < ITFLIESBY_ENGINE_MAP_KEYS_MAX;
        ++key_index) {

        //if we found an available key, activate it
        if (!key_table_active[key_index]) {
            
            new_key.key_index = key_index;

            key_table_keys[key_index]   = new_key;
            key_table_active[key_index] = true; 

            break;
        }
    }

    return(new_key);
}

external ItfliesbyEngineMapKey
itfliesby_engine_maps_manager_map_create(
    ItfliesbyEngine*           engine,
    f32                        root_room_bottom_height,
    f32                        root_room_bottom_width) {

    //TODO: there's a lot of stuff that can go wrong here, we should have
    //the ability to check memory, if keys are available, and to undo any
    //changes if something does happen
    ItfliesbyEngineMapManager* map_manager = &engine->map_manager;


    ItfliesbyEngineMapTableKeys*       map_manager_table_keys       = &map_manager->tables.keys;
    ItfliesbyEngineMapTableMaps*       map_manager_table_maps       = &map_manager->tables.maps;
    ItfliesbyEngineMapTableRooms*      map_manager_table_rows       = &map_manager->tables.rooms;
    ItfliesbyEngineMapTableDimensions* map_manager_table_dimensions = &map_manager->tables.dimensions;
    
    //first, grab the next map index
    s8 map_index = map_manager_table_maps->count_rows;
    ++map_manager_table_maps->count_rows;

    //activate the next key
    ItfliesbyEngineMapKey map_key = 
        itfliesby_engine_maps_manager_find_and_activate_key(
            map_manager_table_keys,
            ITFLIESBY_ENGINE_MAP_KEY_TYPE_MAP,
            map_index);

    //now, we need to shift the tables
    //first, we'll shift the room and dimension tables down
    size_t memory_shift_count_bytes = ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_MAPS * 1;
    memory ending_address_current   = (memory)&map_manager_table_dimensions->col_height[map_manager_table_dimensions->count_rows - 1];
    memory ending_address_new       = ending_address_current + memory_shift_count_bytes;
    size_t addresses_to_shift_count =  
        itfliesby_engine_maps_table_rooms_size_bytes(map_manager_table_rows) + 
        itfliesby_engine_maps_table_dimensions_size_bytes(map_manager_table_dimensions);

    //shift memory
    for (
        size_t address_index = addresses_to_shift_count;
        address_index > -1;
        --address_index) {

        ending_address_new[address_index] = ending_address_current[address_index];
    }
    
    //update pointers
    memory new_map_manager_table_rows_col_index_map           = (memory)map_manager_table_rows->col_index_map           + memory_shift_count_bytes;
    memory new_map_manager_table_rows_col_index_dimensions    = (memory)map_manager_table_rows->col_index_dimensions    + memory_shift_count_bytes;
    memory new_map_manager_table_dimensions_col_bottom_left_x = (memory)map_manager_table_dimensions->col_bottom_left_x + memory_shift_count_bytes;
    memory new_map_manager_table_dimensions_col_bottom_left_y = (memory)map_manager_table_dimensions->col_bottom_left_y + memory_shift_count_bytes;
    memory new_map_manager_table_dimensions_col_width         = (memory)map_manager_table_dimensions->col_width         + memory_shift_count_bytes;
    memory new_map_manager_table_dimensions_col_height        = (memory)map_manager_table_dimensions->col_height        + memory_shift_count_bytes;

    map_manager_table_rows->col_index_map           = (ItfliesbyEngineMapForeignIndex*)new_map_manager_table_rows_col_index_map; 
    map_manager_table_rows->col_index_dimensions    = (ItfliesbyEngineMapForeignIndex*)new_map_manager_table_rows_col_index_dimensions; 
    map_manager_table_dimensions->col_bottom_left_x = (f32*)new_map_manager_table_dimensions_col_bottom_left_x; 
    map_manager_table_dimensions->col_bottom_left_y = (f32*)new_map_manager_table_dimensions_col_bottom_left_y; 
    map_manager_table_dimensions->col_width         = (f32*)new_map_manager_table_dimensions_col_width; 
    map_manager_table_dimensions->col_height        = (f32*)new_map_manager_table_dimensions_col_height; 
    
    return(map_key);
}