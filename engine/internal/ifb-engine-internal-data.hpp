#ifndef IFB_ENGINE_INTERNAL_DATA_HPP
#define IFB_ENGINE_INTERNAL_DATA_HPP

#include "ifb-engine.hpp"
#include "ifb-engine-internal-memory.hpp"

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

//properties
typedef ifb_enum IFBEngineDataPropertiesTag;
typedef ifb_enum IFBEngineDataPropertiesArena;
typedef ifb_enum IFBEngineDataPropertiesGraphics;
typedef ifb_enum IFBEngineDataPropertiesShader;
typedef ifb_enum IFBEngineDataPropertiesTransform;
typedef ifb_enum IFBEngineDataPropertiesSprite;

//sizes
struct IFBEngineDataSizes;
struct IFBEngineDataSizesTag;
struct IFBEngineDataSizesArena;
struct IFBEngineDataSizesGraphics;
struct IFBEngineDataSizesShader;
struct IFBEngineDataSizesTransform;
struct IFBEngineDataSizesSprite;

//offsets
struct IFBEngineDataOffsets;
struct IFBEngineDataOffsetsTag;
struct IFBEngineDataOffsetsArena;
struct IFBEngineDataOffsetsGraphics;
struct IFBEngineDataOffsetsShader;
struct IFBEngineDataOffsetsTransform;
struct IFBEngineDataOffsetsSprite;

//config
struct IFBEngineConfig;
struct IFBEngineConfigTag;
struct IFBEngineConfigArena;
struct IFBEngineConfigGraphics;
struct IFBEngineConfigShader;
struct IFBEngineConfigTransform;
struct IFBEngineConfigSprite;

//data store
struct IFBEngineDataCommit;
struct IFBEngineDataStore;
struct IFBEngineDataQuery;
struct IFBEngineDataConfig;
struct IFBEngineData;
struct IFBEngineDataProperties;

//macros
#define IFB_ENGINE_DATA_PROPERTY_COUNT_TAG       (sizeof(IFBEngineDataOffsetsTag)       / sizeof(ifb_u16))
#define IFB_ENGINE_DATA_PROPERTY_COUNT_ARENA     (sizeof(IFBEngineDataOffsetsArena)     / sizeof(ifb_u16))
#define IFB_ENGINE_DATA_PROPERTY_COUNT_GRAPHICS  (sizeof(IFBEngineDataOffsetsGraphics)  / sizeof(ifb_u16))
#define IFB_ENGINE_DATA_PROPERTY_COUNT_SHADER    (sizeof(IFBEngineDataOffsetsShader)    / sizeof(ifb_u16))
#define IFB_ENGINE_DATA_PROPERTY_COUNT_TRANSFORM (sizeof(IFBEngineDataOffsetsTransform) / sizeof(ifb_u16))
#define IFB_ENGINE_DATA_PROPERTY_COUNT_SPRITE    (sizeof(IFBEngineDataOffsetsSprite)    / sizeof(ifb_u16))

#define IFB_ENGINE_DATA_PROPERTY_COUNT_TOTAL   \
    IFB_ENGINE_DATA_PROPERTY_COUNT_TAG       + \
    IFB_ENGINE_DATA_PROPERTY_COUNT_ARENA     + \
    IFB_ENGINE_DATA_PROPERTY_COUNT_GRAPHICS  + \
    IFB_ENGINE_DATA_PROPERTY_COUNT_SHADER    + \
    IFB_ENGINE_DATA_PROPERTY_COUNT_TRANSFORM + \
    IFB_ENGINE_DATA_PROPERTY_COUNT_SPRITE

/**********************************************************************************/
/* DATA CONFIG                                                                    */
/**********************************************************************************/

struct IFBEngineDataConfigTag {
    ifb_u16 c_str_length;
    ifb_u16 count_max;
};

struct IFBEngineDataConfigArena {
    ifb_u16 arena_minimum_size;
    ifb_u16 arena_minimum_pages;
    ifb_u16 arena_count_max;
    ifb_u16 arena_count_committed;
};

struct IFBEngineDataConfigGraphics {
    ifb_u16 monitor_count;
    ifb_u16 monitor_primary_index;
};

struct IFBEngineDataConfigShader {
    ifb_u16 shader_count_total;
    ifb_u16 shader_count_committed;
};

struct IFBEngineDataConfigTransform {
    ifb_u16 transform_count_max;
    ifb_u16 transform_flag_group_count;
};

struct IFBEngineDataConfigSprite {
    ifb_u16 sprite_count_max;
    ifb_u16 flag_group_count;
};

struct IFBEngineDataConfig {
    IFBEngineDataConfigTag       tag;
    IFBEngineDataConfigArena     arena;
    IFBEngineDataConfigGraphics  graphics;
    IFBEngineDataConfigShader    shader;
    IFBEngineDataConfigTransform transform;
    IFBEngineDataConfigSprite    sprite;
};

/**********************************************************************************/
/* DATA PROPERTIES                                                                */
/**********************************************************************************/

enum IFBEngineDataPropertiesTag_ {
    IFBEngineDataPropertiesTag_None           = 0,
    IFBEngineDataPropertiesTag_CharBuffer     = IFB_BIT_FLAG_0,
    IFBEngineDataPropertiesTag_HashArray      = IFB_BIT_FLAG_1,
    IFBEngineDataPropertiesTag_All            = 
        IFBEngineDataPropertiesTag_CharBuffer |
        IFBEngineDataPropertiesTag_HashArray
};

enum IFBEngineDataPropertiesArena_ {
    IFBEngineDataPropertiesArena_None           = 0,
    IFBEngineDataPropertiesArena_CommitId       = IFB_BIT_FLAG_0, 
    IFBEngineDataPropertiesArena_TagId          = IFB_BIT_FLAG_1,
    IFBEngineDataPropertiesArena_ArenaStart     = IFB_BIT_FLAG_2,
    IFBEngineDataPropertiesArena_ArenaSize      = IFB_BIT_FLAG_3,
    IFBEngineDataPropertiesArena_ArenaData      = 
        IFBEngineDataPropertiesArena_CommitId   |
        IFBEngineDataPropertiesArena_TagId      |
        IFBEngineDataPropertiesArena_ArenaStart |
        IFBEngineDataPropertiesArena_ArenaSize
};

enum IFBEngineDataPropertiesGraphics_ {
    IFBEngineDataPropertiesGraphics_None         = 0, 
    IFBEngineDataPropertiesGraphics_Window       = IFB_BIT_FLAG_0,
    IFBEngineDataPropertiesGraphics_Viewport     = IFB_BIT_FLAG_1,
    IFBEngineDataPropertiesGraphics_Monitors     = IFB_BIT_FLAG_2,
    IFBEngineDataPropertiesGraphics_GraphicsData = 
        IFBEngineDataPropertiesGraphics_Window   |
        IFBEngineDataPropertiesGraphics_Viewport |
        IFBEngineDataPropertiesGraphics_Monitors
};

enum IFBEngineDataPropertiesShader_ {
    IFBEngineDataPropertiesShader_None                     = 0,
    IFBEngineDataPropertiesShader_ArenaId                  = IFB_BIT_FLAG_0,
    IFBEngineDataPropertiesShader_GLIDShaderProgram        = IFB_BIT_FLAG_1,
    IFBEngineDataPropertiesShader_GLIDVertex               = IFB_BIT_FLAG_2,
    IFBEngineDataPropertiesShader_VertexSize               = IFB_BIT_FLAG_3,
    IFBEngineDataPropertiesShader_VertexBufferStart        = IFB_BIT_FLAG_4,
    IFBEngineDataPropertiesShader_VertexBufferSize         = IFB_BIT_FLAG_5,
    IFBEngineDataPropertiesShader_VertexBufferPosition     = IFB_BIT_FLAG_6,
    IFBEngineDataPropertiesShader_ShaderData               = 
        IFBEngineDataPropertiesShader_ArenaId              |
        IFBEngineDataPropertiesShader_GLIDShaderProgram    |
        IFBEngineDataPropertiesShader_GLIDVertex           |
        IFBEngineDataPropertiesShader_VertexSize           |
        IFBEngineDataPropertiesShader_VertexBufferStart    |
        IFBEngineDataPropertiesShader_VertexBufferSize     |
        IFBEngineDataPropertiesShader_VertexBufferPosition
};

enum IFBEngineDataPropertiesTransform_ {
    IFBEngineDataPropertiesTransform_None           = 0,
    IFBEngineDataPropertiesTransform_Translation    = IFB_BIT_FLAG_0,
    IFBEngineDataPropertiesTransform_Scale          = IFB_BIT_FLAG_1,
    IFBEngineDataPropertiesTransform_Rotation       = IFB_BIT_FLAG_2,
    IFBEngineDataPropertiesTransform_Flags          = IFB_BIT_FLAG_3,
    IFBEngineDataPropertiesTransform_TransformData  =
       IFBEngineDataPropertiesTransform_Translation |
       IFBEngineDataPropertiesTransform_Scale       |
       IFBEngineDataPropertiesTransform_Rotation,
    IFBEngineDataPropertiesTransform_All            = 
       IFBEngineDataPropertiesTransform_Translation |
       IFBEngineDataPropertiesTransform_Scale       |
       IFBEngineDataPropertiesTransform_Rotation    |
       IFBEngineDataPropertiesTransform_Flags
};

enum IFBEngineDataPropertiesSprite_ {
    IFBEngineDataPropertiesSprite_None               = 0,
    IFBEngineDataPropertiesSprite_TagId              = IFB_BIT_FLAG_0,
    IFBEngineDataPropertiesSprite_TransformId        = IFB_BIT_FLAG_1,
    IFBEngineDataPropertiesSprite_ColorTableIndex    = IFB_BIT_FLAG_2,
    IFBEngineDataPropertiesSprite_Flags              = IFB_BIT_FLAG_3,
    IFBEngineDataPropertiesSprite_SpriteData         = 
       IFBEngineDataPropertiesSprite_TagId           |
       IFBEngineDataPropertiesSprite_TransformId     |
       IFBEngineDataPropertiesSprite_ColorTableIndex,
    IFBEngineDataPropertiesSprite_All                = 
       IFBEngineDataPropertiesSprite_TagId           |
       IFBEngineDataPropertiesSprite_TransformId     |
       IFBEngineDataPropertiesSprite_ColorTableIndex |
       IFBEngineDataPropertiesSprite_Flags
};
 
struct IFBEngineDataProperties {
    IFBEngineDataPropertiesTag       tag;
    IFBEngineDataPropertiesArena     arena;
    IFBEngineDataPropertiesGraphics  graphics;
    IFBEngineDataPropertiesShader    shader;
    IFBEngineDataPropertiesTransform transform;
    IFBEngineDataPropertiesSprite    sprite;
};

/**********************************************************************************/
/* DATA SIZES                                                                     */
/**********************************************************************************/

struct IFBEngineDataSizesTag {
    ifb_u32 char_buffer;
    ifb_u32 hash_array;
};

struct IFBEngineDataSizesArena {
    ifb_u32 commit_id;
    ifb_u32 tag_id;
    ifb_u32 start;
    ifb_u32 size;
};

struct IFBEngineDataSizesGraphics {
    ifb_u32 window;
    ifb_u32 viewport;
    ifb_u32 monitors;
};

struct IFBEngineDataSizesShader {
    ifb_u32 arena_id;
    ifb_u32 gl_shader_program_id;
    ifb_u32 gl_vertex_id;
    ifb_u32 vertex_size;
    ifb_u32 vertex_buffer_start;
    ifb_u32 vertex_buffer_size;
    ifb_u32 vertex_buffer_position;
};

struct IFBEngineDataSizesTransform {
    ifb_u32 tag_id;
    ifb_u32 transform_id;
    ifb_u32 color_table_index;
    ifb_u32 flags;
};

struct IFBEngineDataSizesSprite {
    ifb_u32 tag_id;
    ifb_u32 transform_id;
    ifb_u32 color_table_index;
    ifb_u32 flags;
};

struct IFBEngineDataSizes {
    IFBEngineDataSizesTag       tag;
    IFBEngineDataSizesArena     arena;
    IFBEngineDataSizesGraphics  graphics;
    IFBEngineDataSizesShader    shader;
    IFBEngineDataSizesTransform transform;
    IFBEngineDataSizesSprite    sprite;
};

namespace ifb_engine {

    ifb_void data_offsets_calculate_all       (IFBEngineDataSizes&          data_sizes_ref);
    ifb_void data_offsets_calculate_tag       (IFBEngineDataSizesTag&       data_sizes_tag_ref);
    ifb_void data_offsets_calculate_arena     (IFBEngineDataSizesArena&     data_sizes_arena_ref);
    ifb_void data_offsets_calculate_graphics  (IFBEngineDataSizesGraphics&  data_sizes_graphics_ref);
    ifb_void data_offsets_calculate_shader    (IFBEngineDataSizesShader&    data_sizes_shader_ref);
    ifb_void data_offsets_calculate_transform (IFBEngineDataSizesTransform& data_sizes_transform_ref);
    ifb_void data_offsets_calculate_sprite    (IFBEngineDataSizesSprite&    data_sizes_sprite_ref);
};

/**********************************************************************************/
/* DATA OFFSETS                                                                   */
/**********************************************************************************/

struct IFBEngineDataOffsetsTag {
    ifb_u16 char_buffer;
    ifb_u16 hash_array;
};

struct IFBEngineDataOffsetsArena {
    ifb_u16 commit_id;
    ifb_u16 tag_id;
    ifb_u16 start;
    ifb_u16 size;
};

struct IFBEngineDataOffsetsGraphics {
    ifb_u16 window;
    ifb_u16 viewport;
    ifb_u16 monitors;
};

struct IFBEngineDataOffsetsShader {
    ifb_u16 arena_id;
    ifb_u16 gl_shader_program_id;
    ifb_u16 gl_vertex_id;
    ifb_u16 vertex_size;
    ifb_u16 vertex_buffer_start;
    ifb_u16 vertex_buffer_size;
    ifb_u16 vertex_buffer_position;
};

struct IFBEngineDataOffsetsTransform {
    ifb_u16 translation;
    ifb_u16 scale;
    ifb_u16 rotation;
    ifb_u16 flags;
};

struct IFBEngineDataOffsetsSprite {
    ifb_u16 tag_id;
    ifb_u16 transform_id;
    ifb_u16 color_table_index;
    ifb_u16 flags;
};

struct IFBEngineDataOffsets {
    IFBEngineDataOffsetsTag       tag;
    IFBEngineDataOffsetsArena     arena;
    IFBEngineDataOffsetsGraphics  graphics;
    IFBEngineDataOffsetsShader    shader;
    IFBEngineDataOffsetsTransform transform;
    IFBEngineDataOffsetsSprite    sprite;
};

namespace ifb_engine {

    ifb_void data_offsets_calculate_all       (const IFBEngineDataSizes&          data_sizes_ref,           IFBEngineDataOffsets&          data_offsets_ref);
    ifb_void data_offsets_calculate_tag       (const IFBEngineDataSizesTag&       data_sizes_tag_ref,       IFBEngineDataOffsetsTag&       data_offsets_tag_ref);
    ifb_void data_offsets_calculate_arena     (const IFBEngineDataSizesArena&     data_sizes_arena_ref,     IFBEngineDataOffsetsArena&     data_offsets_arena_ref);
    ifb_void data_offsets_calculate_graphics  (const IFBEngineDataSizesGraphics&  data_sizes_graphics_ref,  IFBEngineDataOffsetsGraphics&  data_offsets_graphics_ref);
    ifb_void data_offsets_calculate_shader    (const IFBEngineDataSizesShader&    data_sizes_shader_ref,    IFBEngineDataOffsetsShader&    data_offsets_shader_ref);
    ifb_void data_offsets_calculate_transform (const IFBEngineDataSizesTransform& data_sizes_transform_ref, IFBEngineDataOffsetsTransform& data_offsets_transform_ref);
    ifb_void data_offsets_calculate_sprite    (const IFBEngineDataSizesSprite&    data_sizes_sprite_ref,    IFBEngineDataOffsetsSprite&    data_offsets_sprite_ref);
};

/**********************************************************************************/
/* DATA MEMORY                                                                    */
/**********************************************************************************/

struct IFBEngineDataMemory {
    IFBIDCommit commit_id;
    ifb_address commit_start;
};

namespace ifb_engine {

    ifb_ptr               data_memory_get_pointer                               (const IFBEngineDataMemory& data_memory_ref, const ifb_u16                        data_offset);
    
    //tag
    ifb_char*             data_memory_get_pointer_tag_char_buffer               (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsTag&       data_offsets_tag_ref);
    IFBHash*              data_memory_get_pointer_tag_hash_array                (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsTag&       data_offsets_tag_ref);
    
    //arena
    IFBIDCommit*          data_memory_get_pointer_arena_commit_id               (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsArena&     data_offsets_arena_ref);
    IFBIDTag*             data_memory_get_pointer_arena_tag_id                  (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsArena&     data_offsets_arena_ref);
    ifb_address*          data_memory_get_pointer_arena_start                   (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsArena&     data_offsets_arena_ref);
    ifb_u32*              data_memory_get_pointer_arena_size                    (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsArena&     data_offsets_arena_ref);
    
    //graphics
    IFBWindow*            data_memory_get_pointer_graphics_window               (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsGraphics&  data_offsets_graphics_ref);
    IFBGLViewport*        data_memory_get_pointer_graphics_viewport             (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsGraphics&  data_offsets_graphics_ref);
    IFBMonitor*           data_memory_get_pointer_graphics_monitors             (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsGraphics&  data_offsets_graphics_ref);
    
    //shader
    IFBIDArena*           data_memory_get_pointer_shader_arena_id               (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsShader&    data_offsets_shader_ref);
    IFBGLIDShaderProgram* data_memory_get_pointer_shader_gl_shader_program_id   (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsShader&    data_offsets_shader_ref);
    IFBGLIDVertex*        data_memory_get_pointer_shader_gl_vertex_id           (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsShader&    data_offsets_shader_ref);
    ifb_u32*              data_memory_get_pointer_shader_vertex_size            (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsShader&    data_offsets_shader_ref);
    ifb_address*          data_memory_get_pointer_shader_vertex_buffer_start    (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsShader&    data_offsets_shader_ref);
    ifb_u32*              data_memory_get_pointer_shader_vertex_buffer_size     (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsShader&    data_offsets_shader_ref);
    ifb_u32*              data_memory_get_pointer_shader_vertex_buffer_position (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsShader&    data_offsets_shader_ref);
    
    //transform
    IFBVec2*              data_memory_get_pointer_transform_translation         (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsTransform& data_offsets_transform_ref);
    IFBVec2*              data_memory_get_pointer_transform_scale               (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsTransform& data_offsets_transform_ref);
    ifb_f32*              data_memory_get_pointer_transform_rotation            (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsTransform& data_offsets_transform_ref);
    ifb_byte*             data_memory_get_pointer_transform_flags               (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsTransform& data_offsets_transform_ref);
    
    //sprite
    IFBIDTag*             data_memory_get_pointer_sprite_tag_id                 (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsSprite&    data_offsets_sprite_ref);
    IFBIDTransform*       data_memory_get_pointer_sprite_transform_id           (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsSprite&    data_offsets_sprite_ref);
    IFBColorTableIndex*   data_memory_get_pointer_sprite_color_table_index      (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsSprite&    data_offsets_sprite_ref);
    ifb_byte*             data_memory_get_pointer_sprite_flags                  (const IFBEngineDataMemory& data_memory_ref, const IFBEngineDataOffsetsSprite&    data_offsets_sprite_ref);
};

/**********************************************************************************/
/* DATA QUERY                                                                     */
/**********************************************************************************/

struct IFBEngineDataQueryRequest {
    IFBEngineDataProperties property_flags;
    ifb_u32                 property_count;
};

struct IFBEngineDataQueryResult {
    ifb_u32 property_count;
    ifb_ptr property_array[IFB_ENGINE_DATA_PROPERTY_COUNT_TOTAL];
};

struct IFBEngineDataQuery {
    IFBEngineDataQueryRequest request;
    IFBEngineDataQueryResult  result;
};

namespace ifb_engine {

    //------------------------
    // request add property
    //------------------------
    
    ifb_void data_query_request_add_tag_char_buffer                   (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_tag_hash_array                    (IFBEngineDataQueryRequest& data_query_request_ref);

    ifb_void data_query_request_add_arena_commit_id                   (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_arena_tag_id                      (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_arena_start                       (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_arena_size                        (IFBEngineDataQueryRequest& data_query_request_ref);

    ifb_void data_query_request_add_graphics_window                   (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_graphics_viewport                 (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_graphics_monitors                 (IFBEngineDataQueryRequest& data_query_request_ref);

    ifb_void data_query_request_add_shader_arena_id                   (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_shader_gl_shader_program_id       (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_shader_gl_vertex_id               (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_shader_vertex_size                (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_shader_vertex_buffer_start        (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_shader_vertex_buffer_size         (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_shader_vertex_buffer_position     (IFBEngineDataQueryRequest& data_query_request_ref);

    ifb_void data_query_request_add_transform_translation             (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_transform_scale                   (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_transform_rotation                (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_transform_flags                   (IFBEngineDataQueryRequest& data_query_request_ref);

    ifb_void data_query_request_add_sprite_tag_id                     (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_sprite_transform_id               (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_sprite_color_table_index          (IFBEngineDataQueryRequest& data_query_request_ref);
    ifb_void data_query_request_add_sprite_flags                      (IFBEngineDataQueryRequest& data_query_request_ref);
    
    //------------------------
    // request has property
    //------------------------

    const ifb_b8 data_query_request_has_properties_tag                (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_properties_arena              (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_properties_graphics           (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_properties_shader             (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_properties_transform          (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_properties_sprite             (const IFBEngineDataQueryRequest& data_query_request_ref);

    const ifb_b8 data_query_request_has_tag_char_buffer               (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_tag_hash_array                (const IFBEngineDataQueryRequest& data_query_request_ref);
    
    const ifb_b8 data_query_request_has_arena_commit_id               (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_arena_tag_id                  (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_arena_start                   (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_arena_size                    (const IFBEngineDataQueryRequest& data_query_request_ref);
    
    const ifb_b8 data_query_request_has_graphics_window               (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_graphics_viewport             (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_graphics_monitors             (const IFBEngineDataQueryRequest& data_query_request_ref);
    
    const ifb_b8 data_query_request_has_shader_arena_id               (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_shader_gl_shader_program_id   (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_shader_gl_vertex_id           (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_shader_vertex_size            (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_shader_vertex_buffer_start    (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_shader_vertex_buffer_size     (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_shader_vertex_buffer_position (const IFBEngineDataQueryRequest& data_query_request_ref);
    
    const ifb_b8 data_query_request_has_transform_translation         (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_transform_scale               (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_transform_rotation            (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_transform_flags               (const IFBEngineDataQueryRequest& data_query_request_ref);
    
    const ifb_b8 data_query_request_has_sprite_tag_id                 (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_sprite_transform_id           (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_sprite_color_table_index      (const IFBEngineDataQueryRequest& data_query_request_ref);
    const ifb_b8 data_query_request_has_sprite_flags                  (const IFBEngineDataQueryRequest& data_query_request_ref);
    
    //------------------------
    // result add property
    //------------------------

    ifb_void data_query_result_add_tag_char_buffer                (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_tag_hash_array                 (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    
    ifb_void data_query_result_add_arena_commit_id                (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_arena_tag_id                   (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_arena_start                    (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_arena_size                     (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    
    ifb_void data_query_result_add_graphics_window                (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_graphics_viewport              (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_graphics_monitors              (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    
    ifb_void data_query_result_add_shader_arena_id                (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_shader_gl_shader_program_id    (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_shader_gl_vertex_id            (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_shader_vertex_size             (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_shader_vertex_buffer_start     (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_shader_vertex_buffer_size      (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_shader_vertex_buffer_position  (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    
    ifb_void data_query_result_add_transform_translation          (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_transform_scale                (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_transform_rotation             (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_transform_flags                (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    
    ifb_void data_query_result_add_sprite_tag_id                  (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_sprite_transform_id            (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_sprite_color_table_index       (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);
    ifb_void data_query_result_add_sprite_flags                   (IFBEngineDataQueryResult& data_query_result_ref, const IFBEngineDataStore* data_store_ptr);

};

/**********************************************************************************/
/* DATA STORE                                                                     */
/**********************************************************************************/

struct IFBEngineDataInfo {
    ifb_u16 count_tags;
    ifb_u16 count_arenas;
    ifb_u16 arena_size;
    ifb_u16 count_monitors;
    ifb_u16 count_shaders;
    ifb_u16 count_transforms;
    ifb_u16 count_sprites;
};

struct IFBEngineDataStore {
    IFBEngineDataMemory  memory;
    IFBEngineDataOffsets offsets;
    IFBEngineDataInfo    info;
    IFBEngineDataConfig  config;
    IFBEngineDataSizes   sizes;
};

namespace ifb_engine {

    ifb_void
    data_store_initialize(
        IFBEngineDataStore* data_store_ptr,
        IFBEngineMemory*    engine_memory_ptr,
        IFBEngineConfig*    engine_config_ptr);


    ifb_void 
    data_store_commit_memory(
        IFBEngineDataMemory& data_memory_ref,
        IFBEngineMemory*     engine_memory_ptr);


    const ifb_b8
    data_store_execute_query(
        IFBEngineDataStore* data_store_ptr,
        IFBEngineDataQuery* data_query_ptr);

    const IFBEngineDataInfo& data_store_get_info(IFBEngineDataStore* data_store_ptr) { return(data_store_ptr->info); }
};

#endif //IFB_ENGINE_INTERNAL_DATA_HPP