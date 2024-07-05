#ifndef IFB_ENGINE_RENDERER_HPP
#define IFB_ENGINE_RENDERER_HPP

#include "ifb.hpp"
#include "ifb-engine-assets.hpp"
#include "ifb-engine-renderer-opengl.hpp"

//--------------------------------
// TYPES
//--------------------------------

struct  IFBEngineRenderer;
typedef IFBEngineRenderer* IFBEngineRendererPtr;
typedef IFBEngineRenderer& IFBEngineRendererRef;

struct  IFBEngineRendererMemory;
typedef IFBEngineRendererMemory* IFBEngineRendererMemoryPtr;
typedef IFBEngineRendererMemory& IFBEngineRendererMemoryRef;

struct  IFBEngineRendererShader;
typedef IFBEngineRendererShader* IFBEngineRendererShaderPtr;
typedef IFBEngineRendererShader& IFBEngineRendererShaderRef;

struct  IFBEngineRendererShaderUniform;
typedef IFBEngineRendererShaderUniform* IFBEngineRendererShaderUniformPtr;
typedef IFBEngineRendererShaderUniform& IFBEngineRendererShaderUniformRef;

struct  IFBEngineRendererShaderStore;
typedef IFBEngineRendererShaderStore* IFBEngineRendererShaderStorePtr;
typedef IFBEngineRendererShaderStore& IFBEngineRendererShaderStoreRef;


struct  IFBEngineRendererBuffer;
typedef IFBEngineRendererBuffer* IFBEngineRendererBufferPtr;
typedef IFBEngineRendererBuffer& IFBEngineRendererBufferRef;

//--------------------------------
// BUFFER
//--------------------------------


//--------------------------------
// SHADERS
//--------------------------------

//shader store
struct IFBEngineRendererShaderStore {
    IFBEngineRendererShaderPtr shaders;
    u32                        shaders_count;
};

IFBEngineRendererShaderStorePtr
ifb_engine_renderer_shader_store_create_and_initialize();

struct IFBEngineRendererShaderStages {
    GLuint gl_program_id;
    GLuint gl_shader_stage_vertex;
    GLuint gl_shader_stage_fragment;
};

//shader uniform
struct IFBEngineRendererShaderUniform {
    IFBTag name;
    GLint  id;
};


typedef void
(*funcptr_ifb_engine_renderer_shader_update)(
    void* shader_impl
);

struct IFBEngineRendererShader {
    IFBEngineMemoryArenaPtr                   arena_8k;
    IFBEngineRendererShaderStages             stages;
    u32                                       uniform_count;
    IFBEngineRendererShaderUniformPtr         uniforms;
    IFBEngineAssetShader                      asset;
    funcptr_ifb_engine_renderer_shader_update update_callback;
    void*                                     shader_impl;
};

typedef IFBEngineRendererShader* IFBEngineRendererShaderPtr;
typedef IFBEngineRendererShader& IFBEngineRendererShaderRef;

struct IFBEngineRendererShaderSimpleQuad {
    IFBEngineRendererShaderPtr shader_base;
};

struct IFBEngineRendererShaderSolidQuad {
    IFBEngineRendererShaderPtr shader_base;
};


struct IFBEngineRendererShaderTexturedQuad {
    IFBEngineRendererShaderPtr shader_base;
};

struct IFBEngineRendererShaders {
    IFBEngineRendererShaderStore         shader_store;
    IFBEngineRendererShaderSimpleQuad*   shader_simple_quad;
    IFBEngineRendererShaderSolidQuad*    shader_solid_quad;
    IFBEngineRendererShaderTexturedQuad* shader_textured_quad;
};

struct  IFBEngineRendererShaders;
typedef IFBEngineRendererShaders* IFBEngineRendererShadersPtr;
typedef IFBEngineRendererShaders& IFBEngineRendererShadersRef;

IFBEngineRendererShaderPtr
ifb_engine_renderer_shader_create(
    const IFBEngineAssetsShaderId                   stage_id_vertex,
    const IFBEngineAssetsShaderId                   stage_id_fragment,
    const u32                                       uniform_count,
    const char**                                    uniform_names,
    const funcptr_ifb_engine_renderer_shader_update update_callback);

void
ifb_engine_renderer_shader_store_update();



//--------------------------------
// Renderer
//--------------------------------

struct IFBEngineRenderer {
    IFBEngineRendererMemoryPtr memory;
};

IFBEngineRendererPtr
ifb_engine_renderer_create_and_initialize();

void
ifb_engine_renderer_update();

//--------------------------------
// MEMORY
//--------------------------------

enum IFBEngineRendererMemoryArenaSize_ {
     IFBEngineRendererMemoryArenaSize_256B  = 0,
     IFBEngineRendererMemoryArenaSize_512B  = 1,
     IFBEngineRendererMemoryArenaSize_1KB   = 2,
     IFBEngineRendererMemoryArenaSize_8KB   = 3,
     IFBEngineRendererMemoryArenaSize_16KB  = 4,
     IFBEngineRendererMemoryArenaSize_Count = 5,
};

typedef u32 IFBEngineRendererMemoryArenaSize; 

#define IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_256B IFB_MATH_MEGABYTES(8)
#define IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_512B IFB_MATH_MEGABYTES(8)
#define IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_1KB  IFB_MATH_MEGABYTES(8)
#define IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_8KB  IFB_MATH_MEGABYTES(8)
#define IFB_ENGINE_RENDERER_MEMORY_ARENA_ALLOCATOR_TOTAL_SIZE_16KB IFB_MATH_MEGABYTES(8)

#define IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_256B 256
#define IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_512B 512
#define IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_1KB  IFB_MATH_KILOBYTES(1)
#define IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_8KB  IFB_MATH_KILOBYTES(8)
#define IFB_ENGINE_RENDERER_MEMORY_ARENA_SIZE_16KB IFB_MATH_KILOBYTES(16)


struct IFBEngineRendererMemoryAllocatorArena {
    union {
        struct {
            IFBEngineMemoryArenaAllocatorPtr arena_256B;
            IFBEngineMemoryArenaAllocatorPtr arena_512B;
            IFBEngineMemoryArenaAllocatorPtr arena_1KB;
            IFBEngineMemoryArenaAllocatorPtr arena_8KB;
            IFBEngineMemoryArenaAllocatorPtr arena_16KB;
        };
    
        IFBEngineMemoryArenaAllocatorPtr array[IFBEngineRendererMemoryArenaSize_Count];
    };
};

IFBEngineMemoryArenaPtr
ifb_engine_renderer_memory_arena_reserve_auto(
    u64 size_needed);

IFBEngineMemoryArenaPtr
ifb_engine_renderer_memory_arena_reserve(
    IFBEngineRendererMemoryArenaSize arena_size);

void
ifb_engine_renderer_memory_arena_release(
    IFBEngineMemoryArenaPtr arena);

struct IFBEngineRendererMemoryAllocatorUniform {
    IFBEngineMemoryArenaPtr arena_8k;
    memory                  stack_ptr;
    u32                     uniforms_count_total;
};

IFBEngineRendererShaderUniformPtr
ifb_engine_renderer_memory_uniforms_push(
    u32 uniforms_count);

struct IFBEngineRendererMemoryAllocatorShader {
    IFBEngineMemoryArenaPtr arena_8k;
    u32                     shaders_count;
    memory                  stack_ptr;
};

IFBEngineRendererShaderPtr
ifb_engine_renderer_memory_allocator_shader_push();

struct IFBEngineRendererMemoryAllocators {
    IFBEngineRendererMemoryAllocatorArena   arena;
    IFBEngineRendererMemoryAllocatorUniform uniform;      
    IFBEngineRendererMemoryAllocatorShader  shader;      
};

#define IFB_ENGINE_RENDERER_MEMORY_REGION_SIZE IFB_MATH_MEGABYTES(64)

struct IFBEngineRendererMemory {
    IFBEngineMemoryRegionPtr          region;
    IFBEngineRendererMemoryAllocators allocators;
};

IFBEngineRendererMemoryPtr
ifb_engine_renderer_memory_create_and_initialize();


#endif //IFB_ENGINE_RENDERER_HPP