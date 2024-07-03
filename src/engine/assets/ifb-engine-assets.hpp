#ifndef IFB_ENGINE_ASSETS_HPP
#define IFB_ENGINE_ASSETS_HPP

#include <ifb.hpp>

#include "ifb-engine-memory.hpp"

//---------------------------------
// TYPES
//---------------------------------

struct  IFBEngineAssets;
typedef IFBEngineAssets* IFBEngineAssetsPtr;
typedef IFBEngineAssets& IFBEngineAssetsRef;

struct  IFBEngineAssetsMemory;
typedef IFBEngineAssetsMemory* IFBEngineAssetsMemoryPtr;
typedef IFBEngineAssetsMemory& IFBEngineAssetsMemoryRef;

struct  IFBEngineAssetMemoryBlock;
typedef IFBEngineAssetMemoryBlock* IFBEngineAssetMemoryBlockPtr;
typedef IFBEngineAssetMemoryBlock& IFBEngineAssetMemoryBlockRef;


struct  IFBEngineAssetTableIndex;
typedef IFBEngineAssetTableIndex* IFBEngineAssetTableIndexPtr;
typedef IFBEngineAssetTableIndex& IFBEngineAssetTableIndexRef;


struct  IFBAssetIndexStore;
typedef IFBAssetIndexStore* IFBAssetIndexStorePtr;
typedef IFBAssetIndexStore& IFBAssetIndexStoreRef;

struct  IFBEngineAssetTables;
typedef IFBEngineAssetTables* IFBEngineAssetTablesPtr;
typedef IFBEngineAssetTables& IFBEngineAssetTablesRef;

typedef s16 IFBEngineAssetType;
typedef s16 IFBEngineAssetIndexId;
typedef u64 IFBEngineAssetHandle;

struct IFBEngineAsset;

//---------------------------------
// MEMORY
//---------------------------------

#define IFB_ENGINE_ASSET_MEMORY_REGION_SIZE  IFB_MATH_MEGABYTES(64)

#define IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_8KB   IFB_MATH_KILOBYTES(8)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_64KB  IFB_MATH_KILOBYTES(64)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_128KB IFB_MATH_KILOBYTES(128)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_1MB   IFB_MATH_MEGABYTES(1)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_SIZE_4MB   IFB_MATH_MEGABYTES(4)

struct IFBEngineAssetMemoryBlock {
    IFBEngineMemoryArenaPtr          arena;
    memory                           memory;
    u32                              size;  
    IFBEngineAssetMemoryBlockPtr     next;
    IFBEngineAssetMemoryBlockPtr     previous;
};

struct IFBEngineAssetMemoryBlockAllocator {
    IFBEngineAssetMemoryBlockPtr blocks_8KB;
    IFBEngineAssetMemoryBlockPtr blocks_64KB;
    IFBEngineAssetMemoryBlockPtr blocks_128KB;
    IFBEngineAssetMemoryBlockPtr blocks_1MB;
    IFBEngineAssetMemoryBlockPtr blocks_4MB;
};


struct IFBEngineAssetMemoryArenaAllocator {
    IFBEngineMemoryArenaAllocatorPtr arena_8KB;
    IFBEngineMemoryArenaAllocatorPtr arena_64KB;
    IFBEngineMemoryArenaAllocatorPtr arena_128KB;
    IFBEngineMemoryArenaAllocatorPtr arena_1MB;
    IFBEngineMemoryArenaAllocatorPtr arena_4MB;
};

struct IFBEngineAssetMemoryIndexAllocator {
    IFBEngineMemoryArenaPtr arena;
    memory                  stack_ptr;
};

struct IFBEngineAssetMemoryAllocators {
    IFBEngineAssetMemoryBlockAllocator block;
    IFBEngineAssetMemoryArenaAllocator arena;    
    IFBEngineAssetMemoryIndexAllocator index;
};

struct IFBEngineAssetsMemory {
    IFBEngineMemoryRegionPtr       region;
    IFBEngineAssetMemoryAllocators allocators;
};

IFBEngineAssetsMemoryPtr
ifb_engine_assets_memory_create_and_initialize();

IFBEngineAssetMemoryBlockPtr
ifb_engine_assets_memory_block_allocate(
    const u64 size);

void
ifb_engine_assets_memory_block_free(
    IFBEngineAssetMemoryBlockPtr block);

IFBEngineAssetTableIndexPtr 
ifb_engine_assets_memory_index_array_push(
    u32 index_count);

IFBEngineAssetMemoryBlockPtr*
ifb_engine_assets_memory_index_block_array_push(
    u32 index_count);


//---------------------------------
// INDEX
//---------------------------------

#define IFB_ASSETS_INDEX_TABLE_SIZE IFB_MATH_KILOBYTES(8)

const char* IFB_ENGINE_ASSETS_FILE_PATHS[] = {
    "ItFliesBy.Assets.Shaders.ifb",
    "ItFliesBy.Assets.Images.ifb"
};

struct IFBEngineAssetIndex {
    IFBTag                       tag;
    u32                          size;
    u32                          offset;
    IFBEngineAssetMemoryBlockPtr block;
};

struct IFBEngineAssetTableIndex {
    u32 size;
    u32 offset;
};

struct IFBEngineAssetTable {
    IFBEngineAssetType            type;
    u32                           index_count;
    handle                        asset_file_handle;
    IFBEngineAssetTableIndexPtr   index_array;
    IFBEngineAssetMemoryBlockPtr* index_block_array;
};

typedef s16 ItfliesbyEngineAssetsType;

enum ItfliesbyEngineAssetsType_ {
     ItfliesbyEngineAssetsType_Invalid = -1,
     ItfliesbyEngineAssetsType_Shader  =  0,
     ItfliesbyEngineAssetsType_Image   =  1,
     ItfliesbyEngineAssetsType_Count   =  2
};

enum ItfliesbyEngineAssetsShader_ {
     ItfliesbyEngineAssetsShader_Invalid                    = -1,
     ItfliesbyEngineAssetsShader_TexturedQuadVertexShader   =  0, 
     ItfliesbyEngineAssetsShader_TexturedQuadFragmentShader =  1,
     ItfliesbyEngineAssetsShader_SolidQuadVertexShader      =  2,
     ItfliesbyEngineAssetsShader_SolidQuadFragmentShader    =  3,
     ItfliesbyEngineAssetsShader_TestVertexShader           =  4,
     ItfliesbyEngineAssetsShader_TestFragmentShader         =  5,
     ItfliesbyEngineAssetsShader_Count                      =  6
};

enum ItfliesbyEngineAssetsImage_ {
     ItfliesbyEngineAssetsImage_Invalid            = -1, 
     ItfliesbyEngineAssetsImage_ConnorCalibaration =  0,
     ItfliesbyEngineAssetsImage_JigCalibratrion    =  1,
     ItfliesbyEngineAssetsImage_Count              =  2
};

struct IFBEngineAssetTables {
    union {
        struct {
            IFBEngineAssetTable shader; 
            IFBEngineAssetTable image; 
        };
        
        IFBEngineAssetTable array[ItfliesbyEngineAssetsType_Count]; 
    };
};

IFBEngineAssetTablesPtr 
ifb_engine_assets_tables_create_and_initialize();

//---------------------------------
// ASSETS
//---------------------------------

struct IFBEngineAssets {
    IFBEngineAssetsMemoryPtr memory;
    IFBEngineAssetTablesPtr  tables;
};

IFBEngineAssetsPtr
ifb_engine_assets_create_and_initialize();

#endif //IFB_ENGINE_ASSETS_HPP