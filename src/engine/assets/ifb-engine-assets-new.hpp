#ifndef IFB_ENGINE_ASSETS_HPP
#define IFB_ENGINE_ASSETS_HPP

#include <ifb.hpp>

#include "ifb-engine-memory.hpp"

/********************************************************************************************/
/* ASSETS                                                                                   */
/********************************************************************************************/

enum IFBEngineAssetId_ {

    //shaders
    IFBEngineAssetId_Shader_TexturedQuadVertexShader   =  0x0000, 
    IFBEngineAssetId_Shader_TexturedQuadFragmentShader =  0x0001,
    IFBEngineAssetId_Shader_SolidQuadVertexShader      =  0x0002,
    IFBEngineAssetId_Shader_SolidQuadFragmentShader    =  0x0003,
    IFBEngineAssetId_Shader_SimpleQuadVertexShader     =  0x0004,
    IFBEngineAssetId_Shader_SimpleQuadFragmentShader   =  0x0005,

    //images
    IFBEngineAssetId_Image_ConnorCalibaration          =  0x0100,
    IFBEngineAssetId_Image_JigCalibratrion             =  0x0101,
};

typedef u32 IFBEngineAssetId;

/********************************************************************************************/
/* FILES                                                                                    */
/********************************************************************************************/

typedef u16 IFBEngineAssetsId;

enum IFBEngineAssetsType_ {
     IFBEngineAssetsType_Shader  =  0,
     IFBEngineAssetsType_Image   =  1,
     IFBEngineAssetsType_Count   =  2
};

typedef u16 IFBEngineAssetsType;

const char* IFB_ENGINE_ASSETS_FILE_PATHS[] = {
    "ItFliesBy.Assets.Shaders.ifb",
    "ItFliesBy.Assets.Images.ifb"
};

namespace ifb_engine_assets {

    external char*  file_path         (const IFBEngineAssetsType type);
    external size_t file_assets_count (const IFBEngineAssetsType type);
}


/********************************************************************************************/
/* CONTEXT                                                                                  */
/********************************************************************************************/

namespace ifb_engine_assets {

    external void context_create();
    external void context_destroy();
};

/********************************************************************************************/
/* DATA                                                                                     */
/********************************************************************************************/

struct IFBEngineAssetsData {
    IFBEngineMemoryArena   arena
    IFBEngineAssetsType    type;
    size_t                 total_size;
    size_t*                offsets;
    size_t*                sizes;
    IFBEngineAssetsId*     ids
    memory                 start;
};

namespace ifb_engine_assets {

    external void data_load(
        const IFBEngineAssetId     asset_id,
              IFBEngineAssetsData& asset_data);

    external void data_unload(
        IFBEngineAssetsData& asset_data);
};

/********************************************************************************************/
/* SHADERS                                                                                  */
/********************************************************************************************/

enum IFBEngineAssetsShader_ {
     IFBEngineAssetsShader_TexturedQuadVertexShader   =  0, 
     IFBEngineAssetsShader_TexturedQuadFragmentShader =  1,
     IFBEngineAssetsShader_SolidQuadVertexShader      =  2,
     IFBEngineAssetsShader_SolidQuadFragmentShader    =  3,
     IFBEngineAssetsShader_SimpleQuadVertexShader     =  4,
     IFBEngineAssetsShader_SimpleQuadFragmentShader   =  5,
     IFBEngineAssetsShader_Count                      =  6
};

typedef s16 IFBEngineAssetsShader;

namespace ifb_engine_assets {

    external void 
    shader_data_load(
        const size_t*                      count, 
        const IFBEngineAssetsShader*       shaders,
              IFBEngineAssetsData&         data);

    external void
    shader_data_unload(
        IFBEngineAssetsData& data);
};

/********************************************************************************************/
/* IMAGES                                                                                   */
/********************************************************************************************/

enum IFBEngineAssetsImage_ {
     IFBEngineAssetsImage_ConnorCalibaration =  0,
     IFBEngineAssetsImage_JigCalibratrion    =  1,
     IFBEngineAssetsImage_Count              =  2
};

namespace ifb_engine_assets {

    external void 
    image_data_load(
        const size_t*                      count, 
        const IFBEngineAssetsShader*       shaders,
              IFBEngineAssetsData&         data);

    external void
    image_data_unload(
        IFBEngineAssetsData& data);
};

#endif //IFB_ENGINE_ASSETS_HPP