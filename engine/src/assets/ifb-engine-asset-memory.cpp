#pragma once

#include "ifb-engine-internal-asset.hpp"

inline const ifb_b8 
ifb_engine::asset_memory_reserve(
    IFBEngineCoreMemory&   in_core_memory_ref,
    IFBEngineAssetMemory& out_asset_memory_ref) {

    //file table sizes
    const ifb_size file_count            = IFBEngineAssetFileId_Count;
    const ifb_size file_path_length      = IFB_ENGINE_ASSET_FILE_PATH_LENGTH;
    const ifb_size file_path_buffer_size = file_path_length * IFBEngineAssetFileId_Count;

    //header table sizes 
    const ifb_size asset_count              = IFBEngineAssetId_Count;
    const ifb_size asset_name_length        = IFB_ENGINE_ASSET_HEADER_NAME_LENGTH;
    const ifb_size asset_name_buffer_length = asset_name_length * asset_count;

    //data table sizes
    const ifb_size data_block_size  = IFB_ENGINE_ASSET_DATA_BLOCK_SIZE;
    const ifb_size data_block_count = IFB_ENGINE_ASSET_DATA_BLOCK_COUNT;

    //cache our stuff
    IFBEngineMemoryArenaHandle&         system_arena_handle_ref     = in_core_memory_ref.arenas.system;
    IFBEngineAssetMemoryTableFile&      memory_table_file_ref       = out_asset_memory_ref.system_arena_handles.table_file;
    IFBEngineAssetMemoryTableHeader&    memory_table_header_ref     = out_asset_memory_ref.system_arena_handles.table_header;
    IFBEngineAssetMemoryTableDataBlock& memory_table_data_block_ref = out_asset_memory_ref.system_arena_handles.table_data_block;

    //allocate the memory
    ifb_b8 result = true;

    //file
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, file_path_buffer_size,    ifb_char,                       memory_table_file_ref.path);
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, file_count,               IFBEnginePlatformFileIndex,     memory_table_file_ref.platform_file_index);
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, file_count,               ifb_size,                       memory_table_file_ref.size);

    //header    
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, asset_name_buffer_length, ifb_char,                       memory_table_header_ref.name_buffer);
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, asset_count,              IFBEngineAssetTableIndexFile,   memory_table_header_ref.file_table_index);
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, asset_count,              ifb_size,                       memory_table_header_ref.file_data_size);
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, asset_count,              ifb_size,                       memory_table_header_ref.file_data_start);

    //data    
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, data_block_count,         IFBEngineAssetTableIndexHeader, memory_table_data_block_ref.asset_table_index_header);
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, data_block_count,         ifb_timems,                     memory_table_data_block_ref.time_ms_loaded);
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, data_block_count,         ifb_timems,                     memory_table_data_block_ref.time_ms_last_accessed);
    result &= ifb_engine_memory_arena_push_array(system_arena_handle_ref, data_block_count,         IFBEngineMemoryArenaHandle,     memory_table_data_block_ref.asset_data_arena_handle);
    
    //we're done
    return(result);
}

inline const ifb_b8
ifb_engine::asset_memory_pointers_file_table(
    IFBEngineAssetMemory&             in_asset_memory_ref,
    IFBEngineAssetTableFilePointers& out_asset_file_table_pointers_ref) {

    //get the pointers
    const ifb_memory file_path_ptr           = ifb_engine::memory_pointer(in_asset_memory_ref.system_arena_handles.table_file.path);
    const ifb_memory file_platform_index_ptr = ifb_engine::memory_pointer(in_asset_memory_ref.system_arena_handles.table_file.platform_file_index);
    const ifb_memory file_size_ptr           = ifb_engine::memory_pointer(in_asset_memory_ref.system_arena_handles.table_file.size);

    //sanity check
    const ifb_b8 result = (
        file_path_ptr           != NULL &&
        file_platform_index_ptr != NULL &&
        file_size_ptr           != NULL);

    //initialize the struct
    out_asset_file_table_pointers_ref.path_buffer    = (ifb_char*)file_path_ptr;
    out_asset_file_table_pointers_ref.platform_index = (IFBEnginePlatformFileIndex*)file_platform_index_ptr;
    out_asset_file_table_pointers_ref.size           = (ifb_size*)file_size_ptr;

    //we're done
    return(result);
}