#ifndef IFB_ENGINE_INTERNAL_TOOLS_HPP
#define IFB_ENGINE_INTERNAL_TOOLS_HPP

#include "ifb-engine.hpp"
#include "ifb-engine-internal-core.hpp"

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

struct IFBEngineTools;
struct IFBEngineToolsAssets;
struct IFBEngineToolsAssetFileBuilder;

/**********************************************************************************/
/* ASSET TOOLS                                                                    */
/**********************************************************************************/

#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_SHADERS "Shaders"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_IMAGES  "Images"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_SOUNDS  "Sounds"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_DIALOG  "Dialog"

const ifb_cstr IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_LOOKUP[] = {
    IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_SHADERS, // IFBEngineAssetFileId_Shaders
    IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_IMAGES,  // IFBEngineAssetFileId_Images
    IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_SOUNDS,  // IFBEngineAssetFileId_Sounds
    IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_DIALOG   // IFBEngineAssetFileId_Dialog
};

#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_PATH_LENGTH_MAX 256

namespace ifb_engine_tools {

    inline const ifb_cstr 
    asset_file_builder_table_name_lookup(
        const IFBEngineAssetFileId file_id) {

        const ifb_cstr asset_table_name = 
            (file_id >= IFBEngineAssetFileId_Count)
            ? NULL 
            : IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_TABLE_NAME_LOOKUP[file_id];

        return(asset_table_name);
    }
};

struct IFBEngineToolsAssetFileBuilderCSVTable {
    RMemoryArenaHandle         arena_handle;
    ifb_char                   file_path_csv[IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_PATH_LENGTH_MAX];
    IFBEnginePlatformFileIndex platform_file_index;
    ifb_size                   row_count;
    ifb_cstr                   tag_buffer;
    ifb_cstr                   file_path_buffer;
    struct {
        IFBEngineAssetId* asset_id;
    } columns;
};

struct IFBEngineToolsAssetFileBuilder {
    RMemoryRegionHandle                    region_handle;
    r_b8                                   open;
    r_b8                                   selected_file_type;
    r_b8                                   csv_file_is_selected;
    r_b8                                   ifb_file_is_selected;
    IFBEngineAssetFileId                   selected_file_id;
    ifb_char                               file_path_asset [IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_PATH_LENGTH_MAX];
    IFBEngineToolsAssetFileBuilderCSVTable csv_table;    
};

#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_CSV_BUTTON         "Browse##AssetBuilderFile"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_CSV_LABEL          "Asset Builder (.csv)"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_CSV_FILE_TYPE_NAME "Comma Separated Values (.csv)"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_CSV_FILE_TYPE_SPEC "*.csv"

#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_BUTTON         "Browse##AssetFile"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_LABEL          "Asset File (.ifb)"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_FILE_TYPE_NAME "IFB Asset File (.ifb)"
#define IFB_ENGINE_TOOLS_ASSET_FILE_BUILDER_BROWSE_IFB_FILE_TYPE_SPEC "*.ifb"


namespace ifb_engine_tools {

    ifb_internal const ifb_b8 asset_file_builder_render         (IFBEngineToolsAssetFileBuilder& asset_file_builder_ref);
    ifb_internal const ifb_b8 asset_file_builder_combo          (IFBEngineToolsAssetFileBuilder& asset_file_builder_ref);
    ifb_internal const ifb_b8 asset_file_builder_file_selection (IFBEngineToolsAssetFileBuilder& asset_file_builder_ref);
    ifb_internal const ifb_b8 asset_file_builder_csv_table_load (IFBEngineToolsAssetFileBuilder& asset_file_builder_ref);
    
    ifb_internal const ifb_b8 
    asset_file_builder_file_browse_control(
        const ifb_cstr  in_file_browse_button_name,
        const ifb_cstr  in_file_browse_label_name,
        const ifb_cstr  in_file_type_name,
        const ifb_cstr  in_file_type_spec,
        const ifb_size  in_file_path_size_max,
              ifb_b8&  out_file_selected_ref,
        const ifb_cstr out_file_path); 
};

struct IFBEngineToolsAssets {
    IFBEngineToolsAssetFileBuilder file_builder;
};


/**********************************************************************************/
/* TOOLS WINDOW                                                                   */
/**********************************************************************************/

#define IFB_ENGINE_TOOLS_MEMORY_REGION_NAME "ENGINE TOOLS"
#define IFB_ENGINE_TOOLS_MEMORY_ARENA_SIZE  r_memory_size_kilobytes(64)
#define IFB_ENGINE_TOOLS_MEMORY_ARENA_COUNT 1024  


struct IFBEngineTools {
    RMemoryRegionHandle  region_handle;
    ifb_b8               imgui_demo;
    IFBEngineToolsAssets assets;
};

namespace ifb_engine_tools {

    ifb_internal const r_b8 
    tools_start_up(
        IFBEngineCoreMemory& in_core_memory,
        IFBEngineTools&     out_tools_ref);
    
    ifb_internal const r_b8 tools_render_all (IFBEngineTools& tools_ref);
    ifb_internal const r_b8 tools_menu_bar   (IFBEngineTools& tools_ref);
};

#endif //IFB_ENGINE_INTERNAL_TOOLS_HPP