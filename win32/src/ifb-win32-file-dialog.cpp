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

        const ifb_wstr file_type_name_wstr = r_win32::string_cstr_to_wstr(tmp_arena_handle,IFB_WIN32_DIALOG_CSTR_LENGTH_MAX,file_type_name_cstr);
        const ifb_wstr file_type_spec_wstr = r_win32::string_cstr_to_wstr(tmp_arena_handle,IFB_WIN32_DIALOG_CSTR_LENGTH_MAX,file_type_spec_cstr);

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

    ifb_b8   result             = true;

    //sanity check
    if (
        in_file_path_size       == 0 ||
        out_file_path_selection == NULL) {

        return(false);
    }

    //get a temporary arena
    const RMemoryArenaHandle tmp_arena_handle = r_mem::arena_commit(_ifb_win32.memory.win32_region); 
    if (!tmp_arena_handle) {
        return(NULL);
    }

    //see if we have a selection
    const r_wstr file_path_selection_wstr = r_win32::file_dialog_get_selection_as_path_wstr(_ifb_win32.window.file_dialog_handle);

    //if we don't, we're done
    if (!file_path_selection_wstr) {
        return(NULL);
    }

    //convert the w-string path to c-string
    const ifb_cstr file_path_selection_cstr = r_win32::string_wstr_to_cstr(
        tmp_arena_handle,
        in_file_path_size,
        file_path_selection_wstr);

    result &= file_path_selection_cstr != NULL;

    //copy the selection to the out parameter
    memmove(
        out_file_path_selection,
        file_path_selection_cstr,
        in_file_path_size);

    //return the arena
    result &= r_mem::arena_decommit(tmp_arena_handle);

    //we're done
    return(result);
}