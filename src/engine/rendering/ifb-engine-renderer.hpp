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

struct  IFBEngineRendererBuffer;
typedef IFBEngineRendererBuffer* IFBEngineRendererBufferPtr;
typedef IFBEngineRendererBuffer& IFBEngineRendererBufferRef;

//--------------------------------
// BUFFER
//--------------------------------


//--------------------------------
// SHADER
//--------------------------------

struct IFBEngineRendererShaderOpenglIds {
    GLuint program;
    GLuint stage_vertex;
    GLuint stage_fragment;
    GLuint vertex_array_object;
};

struct IFBEngineRendererShaderUniformTable {
    IFBTag* uniform_name_tags;
    GLint*  uniform_locations;
    u32     count;
};

struct IFBEngineRendererShaderMemoryUniform {
    IFBEngineMemoryArenaPtr arena_1kb;
    u64                     uniform_table_size;
    memory                  uniform_table_memory;
};

struct IFBEngineRendererShaderMemoryVertexArrayObject {
    IFBEngineMemoryArenaPtr arena_1kb;
    u64                     vertex_attribute_table_size;
    memory                  vertex_attribute_table_memory;
};

struct IFBEngineRendererShaderMemoryDrawBuffer {
    IFBEngineMemoryArenaPtr arena_16kb;
    u64                     draw_buffer_size;
    memory                  draw_buffer_memory;
};

struct IFBEngineRendererShaderMemory {
    IFBEngineRendererShaderMemoryVertexArrayObject vertex_array_object;    
    IFBEngineRendererShaderMemoryUniform           uniform;
    IFBEngineRendererShaderMemoryDrawBuffer        draw_buffer;    
};

struct IFBEngineRendererShaderVertexArrayAttributeTable {
    GLenum*      gl_types;
    GLboolean*   gl_normalizeds;
    GLsizei*     gl_strides;
    void**       gl_offsets;
    IFBTag*      tags;
    u32          count;
};

struct IFBEngineRendererShaderVertexArrayObject {
    GLuint                                           vertex_array_object;
    IFBEngineRendererShaderVertexArrayAttributeTable attribute_table;
};

struct IFBEngineRendererShader {
    IFBEngineRendererShaderMemory       memory;
    IFBEngineAssetShader                assets;
    IFBEngineRendererShaderOpenglIds    gl_ids;
    IFBEngineRendererShaderUniformTable uniform_table;
    IFBTag                              tag;
};

typedef IFBEngineRendererShader* IFBEngineRendererShaderPtr;
typedef IFBEngineRendererShader& IFBEngineRendererShaderRef;

typedef u32 IFBEngineRendererShaderHandle;

struct IFBEngineRendererShaderManager {
    IFBEngineMemoryArenaPtr  arena_16kb;
    IFBEngineRendererShader* shaders;
    u32                      shaders_count;    
};

typedef IFBEngineRendererShaderManager* IFBEngineRendererShaderManagerPtr;

IFBEngineRendererShaderManagerPtr
ifb_engine_renderer_shader_manager_create_and_initialize();

IFBEngineRendererShaderHandle
ifb_engine_renderer_shader_create(
    const IFBEngineAssetsShaderId asset_stage_vertex,
    const IFBEngineAssetsShaderId asset_stage_fragment,
    const char*                   shader_tag);

void
ifb_engine_renderer_shader_uniform_push(
    const IFBEngineRendererShaderHandle shader_handle,
    const u32                           uniform_count,              
    const char**                        uniform_name);

void
ifb_engine_renderer_shader_compile(
    const IFBEngineRendererShaderHandle shader_handle);

//--------------------------------
// SOLID QUAD SHADER
//--------------------------------

IFBEngineRendererShaderHandle
ifb_engine_renderer_shader_solid_quad_create();

//--------------------------------
// TEXTURED QUAD SHADER
//--------------------------------

IFBEngineRendererShaderHandle
ifb_engine_renderer_shader_textured_quad_create();

//--------------------------------
// Renderer
//--------------------------------

enum IFBEngineRendererShaderType_ {
     IFBEngineRendererShaderType_SolidQuad    = 0,
     IFBEngineRendererShaderType_TexturedQuad = 1,
     IFBEngineRendererShaderType_Count        = 2
};

typedef u32 IFBEngineRendererShaderType;

struct IFBEngineRendererShaderHandles {
    union {
        struct {
            IFBEngineRendererShaderHandle solid_quad;
            IFBEngineRendererShaderHandle textured_quad;
        };

        IFBEngineRendererShaderHandle array[IFBEngineRendererShaderType_Count];
    };
};

struct IFBEngineRenderer {
    IFBEngineRendererMemoryPtr        memory;
    IFBEngineRendererShaderHandles    shader_handles;
    IFBEngineRendererShaderManagerPtr shader_manager;
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

struct IFBEngineRendererMemoryAllocators {
    IFBEngineRendererMemoryAllocatorArena   arena;
};

#define IFB_ENGINE_RENDERER_MEMORY_REGION_SIZE IFB_MATH_MEGABYTES(64)

struct IFBEngineRendererMemory {
    IFBEngineMemoryRegionPtr          region;
    IFBEngineRendererMemoryAllocators allocators;
};

IFBEngineRendererMemoryPtr
ifb_engine_renderer_memory_create_and_initialize();


#endif //IFB_ENGINE_RENDERER_HPP