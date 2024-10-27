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

    //csv file
    if (ImGui::Button("Browse##AssetBuilderFile")) {
        
        const ifb_cstr file_type_name[1] = {"Comma Separated Values (*.csv)"};
        const ifb_cstr file_type_spec[1] = {"(*.csv)"}; 

        ifb_engine::platform_file_dialog_open(
            ".",
            1,
            file_type_name,
            file_type_spec);
    }
    ImGui::SameLine();
    ImGui::InputText("Asset Builder (.csv)",asset_file_builder_ref.file_path_csv,IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_PATH_LENGTH_MAX);

    //asset file
    if (ImGui::Button("Browse##AssetFile")) {

        const ifb_cstr file_type_name[1] = {"IFB Asset File (*.ifb)"};
        const ifb_cstr file_type_spec[1] = {"*.ifb"};

        ifb_engine::platform_file_dialog_open(
            ".",
            1,
            file_type_name,
            file_type_spec);        
    }

    ImGui::SameLine();
    ImGui::InputText("Asset File (.ifb)",asset_file_builder_ref.file_path_asset,IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_PATH_LENGTH_MAX);
    
    return(true);
}