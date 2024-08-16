#ifndef IFB_ENGINE_SHADER_HPP
#define IFB_ENGINE_SHADER_HPP

#include <ifb.hpp>
#include "ifb-engine-memory.hpp"

typedef handle IFBEngineShader;

/********************************************************************************************/
/* SHADER MANAGER                                                                           */
/********************************************************************************************/

namespace ifb_engine_shader {

    internal void manager_create  (void);
    internal void manager_destroy (void);
};


/********************************************************************************************/
/* SHADER STAGE                                                                             */
/********************************************************************************************/

typedef handle IFBEngineShaderStage;

enum IFBEngineShaderStageType_ {
    IFBEngineShaderStageType_Vertex   = 0,
    IFBEngineShaderStageType_Fragment = 1,
};

typedef u32 IFBEngineShaderStageType;

namespace ifb_engine_shader {

    external IFBEngineShaderStage 
    shader_stage_load(
        const IFBEngineShaderStageType type,
        const char*                    shader_stage_buffer);

    external void
    shader_stage_unload(
        const IFBEngineShaderStage shader_stage);
};

/********************************************************************************************/
/* SHADER PROGRAM                                                                           */
/********************************************************************************************/

typedef handle IFBEngineShaderProgram;

namespace ifb_engine_shader {

    external const IFBEngineShaderProgram 
    shader_program_compile(
        const IFBEngineShaderStage shader_stage_vertex, 
        const IFBEngineShaderStage shader_stage_fragment);

    external void
    shader_program_destroy(
        const IFBEngineShaderProgram shader_program);
};


#endif //IFB_ENGINE_SHADER_HPP