#ifndef IFB_ENGINE_RENDERER_HPP
#define IFB_ENGINE_RENDERER_HPP

#include <ifb.hpp>
#include "ifb-engine-assets.hpp"


namespace ifb_engine_renderer {

    internal void context_create();
    internal void context_destroy();
};

typedef handle IFBEngineRendererProgram;

namespace ifb_engine_renderer {

    internal const IFBEngineRendererProgram 
    program_compile(
        const IFBEngineAssetId asset_id_shader_stage_vertex,
        const IFBEngineAssetId asset_id_shader_stage_fragment);

};

#endif //IFB_ENGINE_RENDERER_HPP