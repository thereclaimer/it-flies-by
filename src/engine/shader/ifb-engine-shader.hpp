#ifndef IFB_ENGINE_SHADER_HPP
#define IFB_ENGINE_SHADER_HPP

#include "ifb-engine-memory.hpp"
#include "ifb-engine-assets.hpp"
#include "ifb-engine-opengl.hpp"

struct IFBEngineShader;
struct IFBEngineShaderManager;
struct IFBEngineShaderMemory;

enum IFBEngineShaderId_ {
     IFBEngineShaderId_SolidQuad    = 0,
     IFBEngineShaderId_TexturedQuad = 1,
     IFBEngineShaderId_Count        = 2,
};

typedef u32 IFBEngineShaderId;

/*****************/
/* SHADER MEMORY */
/*****************/

#define IFB_ENGINE_SHADER_MEMORY_SIZE IFB_MATH_MEGABYTES(64);

struct IFBEngineShaderMemory {
    IFBEngineMemoryRegionPtr             region;
    struct {
    } arenas;
    struct {
        IFBEngineMemoryArenaAllocatorPtr arena_4kb;
    } allocators;
};

void
ifb_engine_shader_memory_initialize(
    IFBEngineShaderMemory& shader_memory);

IFBEngineOpenglShaderProgram*
ifb_engine_shader_program_allocate(
    u32 count);

IFBEngineOpenglShaderUniform*
ifb_engine_shader_uniform_allocate(
    u32 count);

/*******************/
/* SHADER UNIFORMS */
/*******************/

struct IFBEngineShaderUniform {
    IFBEngineOpenglShaderUniform gl;    
    u64                          data_size;
    memory                       data
};

/******************/
/* SHADER MANAGER */
/******************/

struct IFBEngineShaderManager {
    IFBEngineShaderMemory        memory;
    IFBEngineAssetsShaderId      vertex_stage_asset_ids[IFBEngineShaderId_Count];
    IFBEngineAssetsShaderId      fragment_stage_asset_ids[IFBEngineShaderId_Count];
    IFBEngineOpenglShaderProgram gl_programs[IFBEngineShaderId_Count];
     gl_uniforms[IFBEngineShaderId_Count];

};

IFBEngineShaderManager*
ifb_engine_shader_manager_create();

#endif //IFB_ENGINE_SHADER_HPP