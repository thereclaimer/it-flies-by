#pragma once

#include "ifb-engine-shader.hpp"

global IFBEngineShaderManager _shader_manager;

IFBEngineShaderManager*
ifb_engine_shader_manager_create() {

    _shader_manager = {0};

    ifb_engine_shader_memory_initialize(_shader_manager.memory);

    //vertex stage asset ids
    _shader_manager.vertex_stage_asset_ids[IFBEngineShaderId_SolidQuad]    = IFBEngineAssetsShader_SolidQuadVertexShader;
    _shader_manager.vertex_stage_asset_ids[IFBEngineShaderId_TexturedQuad] = IFBEngineAssetsShader_TexturedQuadVertexShader;

    //fragment stage asset ids
    _shader_manager.fragment_stage_asset_ids[IFBEngineShaderId_SolidQuad]    = IFBEngineAssetsShader_SolidQuadFragmentShader;
    _shader_manager.fragment_stage_asset_ids[IFBEngineShaderId_TexturedQuad] = IFBEngineAssetsShader_TexturedQuadFragmentShader;

    //load the assets
    IFBEngineAssetShader shader_assets[IFBEngineShaderId_Count];
    char*                vertex_stage_buffers[IFBEngineShaderId_Count];
    char*                fragment_stage_buffers[IFBEngineShaderId_Count];

    for (
        u32 shader_index = 0;
        shader_index < IFBEngineShaderId_Count;
        ++shader_index) {

        ifb_engine_assets_data_shader_load(
            _shader_manager.vertex_stage_asset_ids[shader_index],
            _shader_manager.fragment_stage_asset_ids[shader_index],
            shader_assets[shader_index]);


        vertex_stage_buffers[shader_index]   = shader_assets[shader_index].shader_buffer_vertex; 
        fragment_stage_buffers[shader_index] = shader_assets[shader_index].shader_buffer_fragment;
    }

    //compile the shaders
    ifb_engine_opengl_program_compile(
        IFBEngineShaderId_Count,
        vertex_stage_buffers,
        fragment_stage_buffers,
        _shader_manager.gl_programs);

    //unload the asset data
    for (
        u32 shader_index = 0;
        shader_index < IFBEngineShaderId_Count;
        ++shader_index) {
    
        ifb_engine_assets_data_shader_unload(
            shader_assets[shader_index])
    }

    return(&_shader_manager);
}