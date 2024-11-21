#ifndef IFB_ENGINE_INTERNAL_TABLES_HPP
#define IFB_ENGINE_INTERNAL_TABLES_HPP

#include "ifb-engine.hpp"
#include "ifb-engine-internal.hpp"

struct IFBEngineTableTag;
struct IFBEngineTableStackAllocator;
struct IFBEngineTableBlockAllocator;
struct IFBEngineTableArena;
struct IFBEngineTableShaderProgram;


/**********************************************************************************/
/* TAG TABLE                                                                      */
/**********************************************************************************/

#define IFB_ENGINE_TABLE_TAG_ROW_COUNT    1024
#define IFB_ENGINE_TABLE_TAG_VALUE_LENGTH 32

struct IFBEngineTableTag {
    ifb_handle_memory column_handle_tag_buffer;
    ifb_handle_memory column_handle_hash_value;
};

namespace ifb_engine {

    ifb_void      table_tag                          (IFBEngineTableTag& tag_table);
    ifb_char*     table_tag_column_memory_tag_buffer (IFBEngineTableTag& tag_table);
    IFBHashValue* table_tag_column_memory_hash_value (IFBEngineTableTag& tag_table);
};

/**********************************************************************************/
/* STACK ALLOCATOR TABLE                                                          */
/**********************************************************************************/

#define IFB_ENGINE_TABLE_STACK_ALLOCATOR_ROW_COUNT 1024

struct IFBEngineTableStackAllocator {
    ifb_handle_memory column_handle_arena_index;
    ifb_handle_memory column_handle_used;
};

namespace ifb_engine {

    const ifb_b8 table_stack_allocator(IFBEngineTableStackAllocator);

    ifb_table_index_arena* table_stack_allocator_column_memory_table_index_arena (IFBEngineTableStackAllocator& stack_allocator_table);
    ifb_table_index_arena* table_stack_allocator_column_memory_used              (IFBEngineTableStackAllocator& stack_allocator_table);
};

/**********************************************************************************/
/* ARENA TABLE                                                                    */
/**********************************************************************************/

#define IFB_ENGINE_TABLE_ARENA_ROW_COUNT 4096

struct IFBEngineTableArena {
    ifb_handle_memory column_handle_page_start;
    ifb_handle_memory column_handle_page_count;
    ifb_handle_memory column_handle_table_index_tag;
};

namespace ifb_engine {

    ifb_void             table_arena                               (IFBEngineTableArena& arena_table);
    ifb_u32*             table_arena_column_memory_page_start      (IFBEngineTableArena& arena_table);
    ifb_u32*             table_arena_column_memory_page_count      (IFBEngineTableArena& arena_table);
    ifb_table_index_tag* table_arena_column_memory_table_index_tag (IFBEngineTableArena& arena_table);
};


/**********************************************************************************/
/* TABLE MANAGER                                                                  */
/**********************************************************************************/

struct IFBEngineTable {
    ifb_u32             row_count;
    ifb_u32             column_handle_start;
    ifb_u32             column_handle_count;
};

#define IFB_ENGINE_TABLE_MANAGER_COLUMN_COUNT_MAX 1024

union IFBEngineTables {
    struct {
        IFBEngineTable tag;
        IFBEngineTable stack_allocator;
        IFBEngineTable block_allocator;
        IFBEngineTable arena;
        IFBEngineTable shader_program;
    };
    IFBEngineTable array[IFBEngineTableId_Count];
};

struct IFBEngineTableColumns {
    ifb_u32             column_count_max;
    ifb_u32             column_count_used;
    ifb_handle_memory column_handles[IFB_ENGINE_TABLE_MANAGER_COLUMN_COUNT_MAX];
};

struct IFBEngineTableManager {
    IFBEngineTables       tables;
    IFBEngineTableColumns columns;
};

namespace ifb_engine {

    const ifb_handle_memory  table_manager_create (ifb_void);

    IFBEngineTableManager* table_manager_from_handle  (const ifb_handle_memory handle);
    IFBEngineTableManager* table_manager_from_context (ifb_void);
    
    IFBEngineTable& 
    table_manager_get_table_ref(
              IFBEngineTableManager* table_manager_ptr,
        const IFBEngineTableId       table_id);

    
    const ifb_b8
    table_manager_create_table(
              IFBEngineTableManager* table_manager_ptr,
        const IFBEngineTableId       table_id,
        const ifb_u32                table_column_count,
        const ifb_u32*               table_column_sizes);

    const ifb_b8 table_manager_create_table_tag             (IFBEngineTableManager* table_manager_ptr);
    const ifb_b8 table_manager_create_table_stack_allocator (IFBEngineTableManager* table_manager_ptr);
    const ifb_b8 table_manager_create_table_block_allocator (IFBEngineTableManager* table_manager_ptr);
    const ifb_b8 table_manager_create_table_arena           (IFBEngineTableManager* table_manager_ptr);
    const ifb_b8 table_manager_create_table_shader_program  (IFBEngineTableManager* table_manager_ptr);
};


#endif //IFB_ENGINE_INTERNAL_TABLES_HPP