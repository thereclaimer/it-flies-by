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

internal void
itfliesby_engine_maps_shift_memory_up(
    const size_t  memory_size_bytes,
    const size_t  memory_shift_size_bytes,
    const size_t  affected_pointers_count,
          memory  memory_start,
          memory* affected_pointers[]) {

    //calculate the ending addresses of the current and new memory layout
    memory memory_end_current = memory_start       + memory_size_bytes;
    memory memory_end_new     = memory_end_current + memory_shift_size_bytes;

    //shift memory up
    for (
        size_t byte_index = memory_size_bytes - 1;
        byte_index > -1;
        --byte_index) {

        memory_end_new[byte_index] = memory_end_current[byte_index]; 
    }

    //now, update the affected pointers
    for (
        size_t pointer_index = 0;
        pointer_index < affected_pointers_count;
        ++pointer_index) {

        *affected_pointers[pointer_index] += memory_shift_size_bytes;
    }
}

internal void
itfliesby_engine_maps_shift_table_columns_up(
    const size_t  rows_count_current,
    const size_t  rows_count_new,
    const size_t  columns_count,
    const size_t  column_sizes[],
          memory* column_pointers[]) {

    size_t  i_column_size               = 0;
    size_t  i_column_shift_size         = 0;
    memory* i_column_memory_pointer     = NULL;
    memory  i_column_memory_end_current = NULL;
    memory  i_column_memory_end_new     = NULL;

    for (
        size_t column_index = columns_count - 1;
        column_index > -1;
        --column_index) {

        i_column_size               = column_sizes[column_index];
        i_column_memory_pointer     = column_pointers[column_index];
        i_column_memory_end_current = i_column_memory_pointer + i_column_size; 
        i_column_memory_end_new     = 

        for (
            size_t byte_index = i_column_size - 1;
            byte_index > -1;
            --byte_index) {

            memory_end_new[byte_index] = memory_end_current[byte_index]; 
        }
    }

}

external ItfliesbyEngineMapKey
itfliesby_engine_maps_manager_map_create(
    ItfliesbyEngine* engine,
    f32              root_room_bottom_height,
    f32              root_room_bottom_width) {

    //TODO: there's a lot of stuff that can go wrong here, we should have
    //the ability to check memory, if keys are available, and to undo any
    //changes if something does happen
    ItfliesbyEngineMapManager*         map_manager                  = &engine->map_manager;
    ItfliesbyEngineMapTableKeys*       map_manager_table_keys       = &map_manager->tables.keys;
    ItfliesbyEngineMapTableMaps*       map_manager_table_maps       = &map_manager->tables.maps;
    ItfliesbyEngineMapTableRooms*      map_manager_table_rows       = &map_manager->tables.rooms;
    ItfliesbyEngineMapTableDimensions* map_manager_table_dimensions = &map_manager->tables.dimensions;
    
    //---------------------------------
    // SHIFT OTHER TABLES
    //---------------------------------

    //put together our list of affected pointers
    const size_t affected_pointers_count = 6;
    memory* affected_pointers[affected_pointers_count] = {
        &(memory)map_manager_table_rows->col_index_map,
        &(memory)map_manager_table_rows->col_index_dimensions,
        &(memory)map_manager_table_dimensions->col_bottom_left_x,
        &(memory)map_manager_table_dimensions->col_bottom_left_y,
        &(memory)map_manager_table_dimensions->col_width,
        &(memory)map_manager_table_dimensions->col_height
    };

    //calculate how much memory is being shifted
    const size_t affected_memory_size_bytes = 
        itfliesby_engine_maps_table_rooms_size_bytes(map_manager_table_rows) + 
        itfliesby_engine_maps_table_dimensions_size_bytes(map_manager_table_dimensions);

    //calculate how much the affected memory is shifting by
    const size_t shift_size_bytes = ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_MAPS;

    //calculate the start of the affected memory
    memory affected_memory_start = 
        itfliesby_engine_maps_table_maps_memory_next_table(map_manager_table_maps);

    //shift the memory up
    itfliesby_engine_maps_shift_memory_up(
        affected_memory_size_bytes,
        shift_size_bytes,
        affected_pointers_count,
        affected_memory_start,
        affected_pointers);

    //---------------------------------
    // SHIFT MAP COLUMNS
    //---------------------------------

    // //put together our list of affected pointers
    // const size_t map_column_pointers_count = 1;
    // memory* affected_pointers[affected_pointers_count] = {
    //     &(memory)map_manager_table_rows->col_index_map,
    // };

    // //calculate how much memory is being shifted
    // const size_t affected_memory_size_bytes = 
    //     ITFLIESBY_ENGINE_MAP_TABLE_ROW_SIZE_MAPS * sizeof(8);

    // //calculate how much the affected memory is shifting by
    // const size_t shift_size_bytes = sizeof(8);

    // //calculate the start of the affected memory
    // memory map_column_memory_start = 
    //     (memory)map_manager_table_rows->col_index_map;


    //---------------------------------
    // ACTIVATE NEXT MAP INDEX
    //---------------------------------

    //first, grab the next map index
    s8 map_index = map_manager_table_maps->count_rows;
    ++map_manager_table_maps->count_rows;

    //activate the next key
    ItfliesbyEngineMapKey map_key = 
        itfliesby_engine_maps_manager_find_and_activate_key(
            map_manager_table_keys,
            ITFLIESBY_ENGINE_MAP_KEY_TYPE_MAP,
            map_index);

    return(map_key);
}