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
/* REQUEST                                                                                  */
/********************************************************************************************/

typedef handle IFBEngineAssetsRequest;
typedef handle IFBEngineAssetsData;

namespace ifb_engine_assets {

    external const IFBEngineAssetsRequest request_create          (const size_t asset_count);
    external const bool                   request_push_id         (const IFBEngineAssetId asset_id);
    external const bool                   request_process         (const IFBEngineAssetsRequest request);
    external const IFBEngineAssetsData    request_asset_data_list (const IFBEngineAssetsRequest request);
    external       void                   request_destroy         (const IFBEngineAssetsRequest request);
};

/********************************************************************************************/
/* DATA                                                                                     */
/********************************************************************************************/

namespace ifb_engine_assets {

    external const IFBEngineAssetsData data_next     (const IFBEngineAssetsData data);
    external const size_t              data_size     (const IFBEngineAssetsData data);
    external const memory              data_start    (const IFBEngineAssetsData data); 
    external const IFBEngineAssetId    data_asset_id (const IFBEngineAssetsData data); 
}

/********************************************************************************************/
/* SHADERS                                                                                  */
/********************************************************************************************/

enum IFBEngineAssetsShaderIndex_ {
    IFBEngineAssetsShaderIndex_TexturedQuadVertexShader   =  0, 
    IFBEngineAssetsShaderIndex_TexturedQuadFragmentShader =  1,
    IFBEngineAssetsShaderIndex_SolidQuadVertexShader      =  2,
    IFBEngineAssetsShaderIndex_SolidQuadFragmentShader    =  3,
    IFBEngineAssetsShaderIndex_SimpleQuadVertexShader     =  4,
    IFBEngineAssetsShaderIndex_SimpleQuadFragmentShader   =  5,
    IFBEngineAssetsShaderIndex_Count                      =  6
};

/********************************************************************************************/
/* IMAGES                                                                                   */
/********************************************************************************************/

enum IFBEngineAssetsImageIndex_ {
    IFBEngineAssetsImageIndex_ConnorCalibaration =  0,
    IFBEngineAssetsImageIndex_JigCalibratrion    =  1,
    IFBEngineAssetsImageIndex_Count              =  2
};

#endif //IFB_ENGINE_ASSETS_HPP