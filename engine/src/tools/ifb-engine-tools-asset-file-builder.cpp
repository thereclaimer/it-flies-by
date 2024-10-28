#pragma once

#include "ifb-engine-internal-tools.hpp"
#include "ifb-engine-internal-core.hpp"
#include "ifb-engine-internal-platform.hpp"

ifb_internal const ifb_b8 
ifb_engine_tools::asset_file_builder_render(
    IFBEngineToolsAssetFileBuilder& asset_file_builder_ref) {

    //if the window isn't open, we're done
    if (!asset_file_builder_ref.open) {
        return(true);
    }

    //window start
    if (!ImGui::Begin("Asset File Builder",(bool*)&asset_file_builder_ref.open)) {
        return(false);
    }

    ifb_b8 result = true;

    //render the combo menu
    result &= ifb_engine_tools::asset_file_builder_combo(asset_file_builder_ref);
    
    //render file selection controls
    result &= ifb_engine_tools::asset_file_builder_file_selection(asset_file_builder_ref);

    //load the csv table
    result &= ifb_engine_tools::asset_file_builder_csv_table_load(asset_file_builder_ref);

    //window end
    ImGui::End();

    //we're done
    return(result);
}

ifb_internal const ifb_b8
ifb_engine_tools::asset_file_builder_combo(
    IFBEngineToolsAssetFileBuilder& asset_file_builder_ref) {

    ImGui::SeparatorText("Asset File Type");

    //get the preview for the combo menu
    const ifb_cstr combo_preview = asset_file_builder_ref.selected_file_type
        ? ifb_engine_tools::asset_file_builder_table_name_lookup(asset_file_builder_ref.selected_file_id)
        : "Select Asset File Type";
    if (combo_preview == NULL) {
        return(false);
    }

    //render the combo menu
    if (ImGui::BeginCombo("Asset File Type",combo_preview)) {

        for (
            IFBEngineAssetFileId file_id = 0;
            file_id < IFBEngineAssetFileId_Count;
            ++file_id) {

            //get the name of the current table name
            const ifb_cstr current_file_name = asset_file_builder_table_name_lookup(file_id);
            if (current_file_name == NULL) {

                //if its null, something is wrong and we're done
                return(false);
            }

            //update our builder if the table name was just selected
            if (ImGui::Selectable(current_file_name)) {
                asset_file_builder_ref.selected_file_type = true;
                asset_file_builder_ref.selected_file_id   = file_id;
            }

            //if the current table name is the selected one, set it as the default focused item in the list
            if (
                asset_file_builder_ref.selected_file_type && 
                asset_file_builder_ref.selected_file_id == file_id) {
                
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    //we're done
    return(true);
}

ifb_internal const ifb_b8
ifb_engine_tools::asset_file_builder_file_selection(
    IFBEngineToolsAssetFileBuilder& asset_file_builder_ref) {

    ImGui::SeparatorText("Asset Files");

    //if there's no selected file, we're done
    if (!asset_file_builder_ref.selected_file_type) {
        ImGui::Text("(No asset file type selected)");
        return(true);
    }

    ifb_b8 result = true;

    //builder file (.csv)
    result &= ifb_engine_tools::asset_file_builder_file_browse_control(
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_CSV_BUTTON,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_CSV_LABEL,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_CSV_FILE_TYPE_NAME,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_CSV_FILE_TYPE_SPEC,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_PATH_LENGTH_MAX,
        asset_file_builder_ref.csv_file_is_selected,
        asset_file_builder_ref.csv_table.file_path_csv);
    
    //asset file (.ifb)
    result &= ifb_engine_tools::asset_file_builder_file_browse_control(
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_BUTTON,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_LABEL,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_FILE_TYPE_NAME,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_FILE_TYPE_SPEC,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_PATH_LENGTH_MAX,
        asset_file_builder_ref.ifb_file_is_selected,
        asset_file_builder_ref.file_path_asset);
      
    return(true);
}

ifb_internal const ifb_b8 
ifb_engine_tools::asset_file_builder_csv_table_load(
    IFBEngineToolsAssetFileBuilder& asset_file_builder_ref) {

    //check to see if the csv input and ifb output files have been selected
    if (
        !asset_file_builder_ref.csv_file_is_selected ||
        !asset_file_builder_ref.ifb_file_is_selected) {

        //if not, we're done
        return(true);
    }

    //cache the csv table
    IFBEngineToolsAssetFileBuilderCSVTable& csv_table_ref = asset_file_builder_ref.csv_table; 

    //with both of these files selected, check to see if the actual file data has been loaded
    if (!csv_table_ref.arena_handle) {

        //without an arena, we know the file hasn't been loaded
        csv_table_ref.arena_handle = r_mem::arena_commit(asset_file_builder_ref.region_handle);
        if (!csv_table_ref.arena_handle) {
            return(false);
        }

        //load the file
        const ifb_b8 file_loaded = ifb_engine::platform_file_open_read_write(
            csv_table_ref.file_path_csv,
            csv_table_ref.platform_file_index);

        //if that didn't work, something went wrong and we're done
        if (!file_loaded) {
            return(false);
        }
    
        //get the file size
        const ifb_size csv_file_size = ifb_engine::platform_file_size(csv_table_ref.platform_file_index);

        //get the arena memory
        ifb_memory csv_memory = r_mem::arena_push(csv_table_ref.arena_handle,csv_file_size);
        
        //load the existing csv data
        const ifb_b8 read_result = ifb_engine::platform_file_read(
            csv_table_ref.platform_file_index,
            0,
            csv_file_size,
            csv_memory);

        if (!read_result) {
            return(false);
        }
    }



    return(true);
}

ifb_internal const ifb_b8 
ifb_engine_tools::asset_file_builder_file_browse_control(
    const ifb_cstr  in_file_browse_button_name,
    const ifb_cstr  in_file_browse_label_name,
    const ifb_cstr  in_file_type_name,
    const ifb_cstr  in_file_type_spec,
    const ifb_size  in_file_path_size_max,
          ifb_b8&  out_file_selected_ref,
    const ifb_cstr out_file_path) {

    //sanity check
    if (
        in_file_browse_button_name == NULL ||
        in_file_browse_label_name  == NULL ||
        in_file_type_name          == NULL ||
        in_file_type_spec          == NULL ||
        in_file_path_size_max      == 0    ||
        out_file_path              == NULL) {

        return(false);
    }

    //render the controls
    const ifb_b8 button_pressed = ImGui::Button(in_file_browse_button_name);
    ImGui::SameLine();
    ImGui::InputText(in_file_browse_label_name,out_file_path,in_file_path_size_max);

    //if the user didn't push the button, we're done
    if (!button_pressed) {
        return(true);
    }

    //dialog config
    const ifb_cstr dialog_starting_directory = ".";
    const ifb_size dialog_file_type_count    = 1;
    
    //open the dialog    
    const ifb_b8 dialog_result = 
        ifb_engine::platform_file_dialog_open(
            dialog_starting_directory,
            dialog_file_type_count,
            &in_file_type_name,
            &in_file_type_spec);

    //if we don't have a selection, we're done
    if (!dialog_result) {
        return(true);
    }

    //get the user's selection
    const ifb_b8 selection_result = 
            ifb_engine::platform_file_dialog_get_selection(
                in_file_path_size_max,
                out_file_path);

    out_file_selected_ref = selection_result;

    //we're done
    return(selection_result);
}