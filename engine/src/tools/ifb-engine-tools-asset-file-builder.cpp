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
    const ifb_cstr combo_preview = asset_file_builder_ref.selected_file
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
                asset_file_builder_ref.selected_file    = true;
                asset_file_builder_ref.selected_file_id = file_id;
            }

            //if the current table name is the selected one, set it as the default focused item in the list
            if (
                asset_file_builder_ref.selected_file && 
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
    if (!asset_file_builder_ref.selected_file) {
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
        asset_file_builder_ref.file_path_csv);
    
    //asset file (.ifb)
    result &= ifb_engine_tools::asset_file_builder_file_browse_control(
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_BUTTON,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_LABEL,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_FILE_TYPE_NAME,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_FILE_TYPE_SPEC,
        IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_PATH_LENGTH_MAX,
        asset_file_builder_ref.file_path_asset);
      
    return(true);
}

ifb_internal const ifb_b8 
ifb_engine_tools::asset_file_builder_file_browse_control(
    const ifb_cstr  in_file_browse_button_name,
    const ifb_cstr  in_file_browse_label_name,
    const ifb_cstr  in_file_type_name,
    const ifb_cstr  in_file_type_spec,
    const ifb_size  in_file_path_size_max,
    const ifb_cstr out_file_path) {

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

    //we're done
    return(selection_result);
}