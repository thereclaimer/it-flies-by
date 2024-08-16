#ifndef IFB_ENGINE_RENDERER_HPP
#define IFB_ENGINE_RENDERER_HPP

#include "ifb.hpp"
#include "ifb-engine-memory.hpp"
#include "ifb-engine-assets.hpp"
#include "ifb-engine-renderer-opengl.hpp"


//--------------------------------
// TYPES
//--------------------------------

struct  IFBEngineRenderer;
struct  IFBEngineRendererMemory;
struct  IFBEngineRendererBuffer;

#define IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR   0.003921569
#define IFB_ENGINE_RENDERER_COLOR_DENORMALIZATION_FACTOR 255 

struct IFBEngineRendererColorHex {

    union {
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };

        f32 rgba;
    };

};

struct IFBEngineRendererColorNormalized {
    union {

        struct {
            f32 r;
            f32 g;
            f32 b;
            f32 a;
        };

        f32 rgba[4];
    };
};

inline IFBEngineRendererColorHex
ifb_engine_renderer_color_normalized_to_hex(
    const IFBEngineRendererColorNormalized& color_normalized) {

    IFBEngineRendererColorHex color_hex = {0};
    color_hex.r = color_normalized.r * IFB_ENGINE_RENDERER_COLOR_DENORMALIZATION_FACTOR;
    color_hex.g = color_normalized.g * IFB_ENGINE_RENDERER_COLOR_DENORMALIZATION_FACTOR;
    color_hex.b = color_normalized.b * IFB_ENGINE_RENDERER_COLOR_DENORMALIZATION_FACTOR;
    color_hex.a = color_normalized.a * IFB_ENGINE_RENDERER_COLOR_DENORMALIZATION_FACTOR;

    return(color_hex);
} 

inline IFBEngineRendererColorNormalized
ifb_engine_renderer_color_normalize(
    const IFBEngineRendererColorHex& color_hex) {

    IFBEngineRendererColorNormalized color_normalized = {0};
    color_normalized.r = color_hex.r * IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR;
    color_normalized.g = color_hex.g * IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR;
    color_normalized.b = color_hex.b * IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR;
    color_normalized.a = color_hex.a * IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR;

    return(color_normalized);    
}

inline IFBEngineRendererColorNormalized
ifb_engine_renderer_color_normalize_value_rgba(
    const u32 color_hex_value) {

    IFBEngineRendererColorHex color_hex = {0};

    color_hex.a = (color_hex_value >> 24) & 0xFF; 
    color_hex.b = (color_hex_value >> 16) & 0xFF; 
    color_hex.g = (color_hex_value >> 8)  & 0xFF; 
    color_hex.r =  color_hex_value        & 0xFF; 

    IFBEngineRendererColorNormalized color_normalized = {0};
    color_normalized.r = color_hex.r * IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR;
    color_normalized.g = color_hex.g * IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR;
    color_normalized.b = color_hex.b * IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR;
    color_normalized.a = color_hex.a * IFB_ENGINE_RENDERER_COLOR_NORMALIZATION_FACTOR;

    return(color_normalized);    
}

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
    IFBEngineMemoryArena    arena;
    u64                     uniform_table_size;
    memory                  uniform_table_memory;
};

struct IFBEngineRendererShaderMemoryVertexArrayObject {
    IFBEngineMemoryArena    arena;
    u64                     vertex_attribute_table_size;
    memory                  vertex_attribute_table_memory;
};

struct IFBEngineRendererShaderMemoryDrawBuffer {
    IFBEngineMemoryArena    arena;
    u64                     draw_buffer_size;
    memory                  draw_buffer_memory;
};

struct IFBEngineRendererShaderMemory {
    IFBEngineRendererShaderMemoryVertexArrayObject vertex_array_object;    
    IFBEngineRendererShaderMemoryUniform           uniform;
    IFBEngineRendererShaderMemoryDrawBuffer        draw_buffer;    
};

struct IFBEngineRendererShaderVertexArrayAttribute {
    GLenum     gl_type;
    GLboolean  gl_normalized;
    u32        element_size;
    u32        element_count;
    IFBTag     tag;
};

struct IFBEngineRendererShaderVertexArrayAttributeTable {
    GLenum*    gl_types;
    GLboolean* gl_normalizeds;
    u32*       element_sizes;
    u32*       element_counts;
    IFBTag*    tags;
    u32        count;
};

struct IFBEngineRendererShaderVertexArrayObject {
    GLuint                                           vertex_array_object;
    IFBEngineRendererShaderVertexArrayAttributeTable attribute_table;
};

struct IFBEngineRendererShader {
    IFBEngineRendererShaderMemory            memory;
    IFBEngineAssetShader                     assets;
    IFBEngineRendererShaderVertexArrayObject vertex_array_object;
    IFBEngineRendererShaderOpenglIds         gl_ids;
    IFBEngineRendererShaderUniformTable      uniform_table;
    IFBTag                                   tag;
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
ifb_engine_renderer_shader_vertex_array_attributes_push(
    const IFBEngineRendererShaderHandle                shader_handle,
    const u32                                          attribute_count,
    const IFBEngineRendererShaderVertexArrayAttribute* attributes);

void
ifb_engine_renderer_shader_compile(
    const IFBEngineRendererShaderHandle shader_handle);

memory
ifb_engine_renderer_shader_push_vertex(
    const IFBEngineRendererShaderHandle shader_handle,
    const u32                           vertex_count);

//--------------------------------
// SOLID QUAD SHADER
//--------------------------------

struct IFBEngineRendererShaderSolidQuad {
    IFBEngineRendererShaderHandle shader_handle;
};

struct IFBEngineRendererSolidQuad {
    IFBMathMat3                      transform;
    IFBEngineRendererColorNormalized color;
};

struct IFBEngineRendererSolidQuadVertex {
    union {
        struct {
            f32 transform[9];
            f32 color[4];
        };
        f32 buffer[13];
    };
};

IFBEngineRendererShaderSolidQuad*
ifb_engine_renderer_shader_solid_quad_create();

void
ifb_engine_renderer_shader_solid_quad_push(
    IFBEngineRendererSolidQuad* solid_quads,
    u32                         solid_quads_count);

//--------------------------------
// TEXTURED QUAD SHADER
//--------------------------------

struct IFBEngineRendererShaderTexturedQuad {
    IFBEngineRendererShaderHandle shader_handle;
};

IFBEngineRendererShaderTexturedQuad*
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

struct IFBEngineRendererShaders {
    IFBEngineRendererShaderSolidQuad*    solid_quad;
    IFBEngineRendererShaderTexturedQuad* textured_quad;
};

struct IFBEngineRenderer {
    IFBEngineRendererMemoryPtr        memory;
    IFBEngineRendererShaders          shaders;
    IFBEngineRendererShaderManagerPtr shader_manager;
};

IFBEngineRendererPtr
ifb_engine_renderer_create_and_initialize();

void
ifb_engine_renderer_update();

void
ifb_engine_renderer_push_solid_quad(
    const IFBEngineRendererSolidQuad* solid_quads,
    const u32                         count);


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