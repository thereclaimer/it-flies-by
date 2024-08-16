#ifndef IFB_ENGINE_SHADER_INTERNAL_HPP
#define IFB_ENGINE_SHADER_INTERNAL_HPP

#include "ifb-engine-shader.hpp"

/********************************************************************************************/
/* FORWARD DECLARATIONS                                                                     */
/********************************************************************************************/

struct IFBEngineShaderManager;
struct IFBEngineShaderMemory;
struct IFBEngineShaderStageTable;
struct IFBEngineShaderStage_Impl;

/********************************************************************************************/
/* SHADER MANAGER                                                                           */
/********************************************************************************************/

struct IFBEngineShaderManager {
    IFBEngineShaderMemory memory;
};

/********************************************************************************************/
/* MEMORY                                                                                   */
/********************************************************************************************/

struct IFBEngineShaderMemory {
    IFBEngineMemoryReservation reservation;
    IFBEngineMemoryRegion      table_region;
};

namespace ifb_engine_shader {

    internal       void                 memory_reserve        (void);
    internal       void                 memory_release        (void);
    internal const IFBEngineMemoryArena memory_arena_commit   (void);
    internal const IFBEngineMemoryArena memory_arena_decommit (void);
};

/********************************************************************************************/
/* SHADER STAGE TABLE                                                                       */
/********************************************************************************************/

struct IFBEngineShaderStage_Impl {
    GLuint                   gl_shader_stage_id;
    GLuint                   gl_shader_stage_type;
    IFBEngineShaderStageType ifb_shader_stage_type;
};

struct IFBEngineShaderStageTable {
    IFBEngineMemoryArena       arena;
    size_t                     count;
    IFBEngineShaderStage_Impl* shader_stages;    
};

namespace ifb_engine_shader {

    shader_stage_table_create  (void);
    shader_stage_table_destroy (void);
}

#endif //IFB_ENGINE_SHADER_INTERNAL_HPP