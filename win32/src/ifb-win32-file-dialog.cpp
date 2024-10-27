#pragma once

#include "ifb-win32.hpp"

ifb_internal ifb_void 
ifb_win32::file_dialog_api_initialize(
    IFBEnginePlatformFileDialog& platform_api_file_dialog_ref) {

    platform_api_file_dialog_ref.open          = ifb_win32::file_dialog_open;
    platform_api_file_dialog_ref.get_selection = ifb_win32::file_dialog_get_selection;
}

ifb_internal const ifb_b8
ifb_win32::file_dialog_open(
    const ifb_cstr  file_dialog_starting_directory,
    const ifb_size  file_type_count,
    const ifb_cstr* file_type_name_cstr_ptr,
    const ifb_cstr* file_type_spec_cstr_ptr) {

    //sanity check
    if (
        file_dialog_starting_directory == NULL ||
        file_type_count                == 0    ||
        file_type_name_cstr_ptr        == NULL ||
        file_type_spec_cstr_ptr        == NULL) {

        return(false);
    }

    //get a temporary arena
    const RMemoryArenaHandle tmp_arena_handle = r_mem::arena_commit(_ifb_win32.memory.win32_region); 
    if (!tmp_arena_handle) {
        return(false);
    }

    //allocate the wide string arrays 
    ifb_wstr* file_type_name_wstr_ptr = r_mem_arena_push_array(tmp_arena_handle,file_type_count,ifb_wstr);
    ifb_wstr* file_type_spec_wstr_ptr = r_mem_arena_push_array(tmp_arena_handle,file_type_count,ifb_wstr);

    //cache the size of a wchar
    const ifb_size w_char_size = sizeof(ifb_wchar);

    //convert cstr to wstr
    for (
        ifb_index file_type_index = 0;
        file_type_index < file_type_count;
        ++file_type_index) {

        //cache the current c-strings
        const ifb_cstr file_type_name_cstr = file_type_name_cstr_ptr[file_type_index];
        const ifb_cstr file_type_spec_cstr = file_type_spec_cstr_ptr[file_type_index];

        //get the c-string lengths, including the null terminator
        const ifb_size file_type_name_cstr_length = strnlen_s(file_type_name_cstr, IFB_WIN32_DIALOG_CSTR_LENGTH_MAX) + 1;
        const ifb_size file_type_spec_cstr_length = strnlen_s(file_type_spec_cstr, IFB_WIN32_DIALOG_CSTR_LENGTH_MAX) + 1;

        //determine the byte length of the equivalient w-string
        //wstr_byte_len = w_char_size * cstr_len
        const ifb_size file_type_name_wstr_length_bytes = w_char_size * file_type_name_cstr_length; 
        const ifb_size file_type_spec_wstr_length_bytes = w_char_size * file_type_spec_cstr_length; 
    
        //get the memory for the w-strings
        const ifb_wstr file_type_name_wstr = r_mem_arena_push_array(tmp_arena_handle, file_type_name_wstr_length_bytes, ifb_wchar);
        const ifb_wstr file_type_spec_wstr = r_mem_arena_push_array(tmp_arena_handle, file_type_spec_wstr_length_bytes, ifb_wchar);
    
        //clear the memory
        memset(file_type_name_wstr,0,file_type_name_wstr_length_bytes);        
        memset(file_type_spec_wstr,0,file_type_spec_wstr_length_bytes);

        //convert the c-strings to w-strings
        //we pass in the cstr_length twice, because that is the number of characters
        MultiByteToWideChar(CP_UTF8, 0, file_type_name_cstr, file_type_name_cstr_length, file_type_name_wstr, file_type_name_cstr_length);
        MultiByteToWideChar(CP_UTF8, 0, file_type_spec_cstr, file_type_spec_cstr_length, file_type_spec_wstr, file_type_spec_cstr_length);

        //update the wstr array
        file_type_name_wstr_ptr[file_type_index] = file_type_name_wstr; 
        file_type_spec_wstr_ptr[file_type_index] = file_type_spec_wstr; 
    }

    //now, we can open the dialog
    const ifb_b8 result = r_win32::file_dialog_open(
        _ifb_win32.window.file_dialog_handle,
        file_dialog_starting_directory,
        file_type_count,
        file_type_name_wstr_ptr,
        file_type_spec_wstr_ptr);

    //return the arena
    r_mem::arena_decommit(tmp_arena_handle);

    //we're done
    return(result);
}

ifb_internal const ifb_b8
ifb_win32::file_dialog_get_selection(
    const ifb_size  in_file_path_size,
    const ifb_cstr out_file_path_selection) {

    

    return(true);
}