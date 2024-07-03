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

typedef s16 ItfliesbyEngineAssetsType;
typedef s16 ItfliesbyEngineAssetsShader;
typedef s16 ItfliesbyEngineAssetsImage;

//---------------------------------
// ASSET FILE
//---------------------------------


const char* IFB_ENGINE_ASSETS_FILE_PATHS[] = {
    "ItFliesBy.Assets.Shaders.ifb",
    "ItFliesBy.Assets.Images.ifb"
};

struct IFBEngineAssetsFileIndex {
    char tag[32];         // plaintext identifier for the entity the asset belongs to
    u32  file_size;       // size of the data is stored in the file
    u32  allocation_size; // the size of the space we need to allocate when storing the asset data in memory
    u32  offset;          // the index of the first byte of asset data in the file
};

typedef IFBEngineAssetsFileIndex* IFBEngineAssetsFileIndexPtr;
typedef IFBEngineAssetsFileIndex& IFBEngineAssetsFileIndexRef;

struct IFBEngineAssetFileIndexBuffer {
    IFBEngineAssetsFileIndexPtr indexes;    
    u32                         count;
};

struct IFBEngineAssetFileBuffer {
    IFBEngineAssetType file_type;
    u64                buffer_size_bytes;
    memory             buffer;    
};

typedef IFBEngineAssetFileBuffer* IFBEngineAssetFileBufferPtr;
typedef IFBEngineAssetFileBuffer& IFBEngineAssetFileBufferRef;

struct IFBEngineAssetFile {
    handle                        file_handle;
    u64                           file_size_bytes;
    IFBEngineAssetType            file_type;
    u64                           asset_data_start;
    IFBEngineAssetMemoryBlockPtr  file_data_buffer;
    IFBEngineAssetFileIndexBuffer index_buffer;
};

struct IFBEngineAssetFiles {

    union {
        struct {
            IFBEngineAssetFile shader;
            IFBEngineAssetFile images;
        };

        IFBEngineAssetFile array[ItfliesbyEngineAssetsType_Count];        
    };
};

typedef IFBEngineAssetFiles* IFBEngineAssetFilesPtr;
typedef IFBEngineAssetFiles& IFBEngineAssetFilesRef;

IFBEngineAssetFilesPtr
ifb_engine_asset_files_create_and_initialize();

IFBEngineAssetFileBufferPtr
ifb_engine_asset_files_index_buffer_allocate_and_read(
    IFBEngineAssetType asset_type);

#define IFB_ENGINE_ASSETS_FILE_VERIFICATION_SIZE_BYTES (sizeof(u32) + 3)
#define IFB_ENGINE_ASSETS_FILE_INDEX_SIZE_BYTES        ((sizeof(u32) * 3) + 32)

inline u64
ifb_engine_assets_files_index_size_byes(
    u32 index_count) {

    u64 index_size_bytes = IFB_ENGINE_ASSETS_FILE_INDEX_SIZE_BYTES * index_count;

    return(index_size_bytes);
}

inline u64
ifb_engine_assets_files_header_size_bytes(
    u32 index_count) {

    u64 index_size_bytes = IFB_ENGINE_ASSETS_FILE_INDEX_SIZE_BYTES * index_count;

    u64 header_size_bytes = index_size_bytes + IFB_ENGINE_ASSETS_FILE_VERIFICATION_SIZE_BYTES;   

    return(header_size_bytes);
}

//---------------------------------
// ASSET DATA
//---------------------------------

struct IFBEngineAssetData {
    IFBEngineAssetType            type;
    u64                           buffer_size_bytes;
    memory                        buffer;
};

struct IFBEngineAssetDataTable {
    IFBEngineAssetMemoryBlockPtr shader[ItfliesbyEngineAssetsShader_Count];
    IFBEngineAssetMemoryBlockPtr image[ItfliesbyEngineAssetsImage_Count];
};

typedef IFBEngineAssetDataTable* IFBEngineAssetDataTablePtr;
typedef IFBEngineAssetDataTable& IFBEngineAssetDataTableRef;

IFBEngineAssetDataTablePtr
ifb_engine_assets_data_table_create_and_initialize();

//---------------------------------
// ASSETS
//---------------------------------

struct IFBEngineAssets {
    IFBEngineAssetsMemoryPtr   memory;
    IFBEngineAssetDataTablePtr data_table; 
    IFBEngineAssetFilesPtr     files;
};

IFBEngineAssetsPtr
ifb_engine_assets_create_and_initialize();

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

enum IFBEngineAssetMemoryArenaSize_ {
     IFBEngineAssetMemoryArenaSize_8KB   = 0,
     IFBEngineAssetMemoryArenaSize_64KB  = 1,
     IFBEngineAssetMemoryArenaSize_128KB = 2,
     IFBEngineAssetMemoryArenaSize_1MB   = 3,
     IFBEngineAssetMemoryArenaSize_4MB   = 4,
     IFBEngineAssetMemoryArenaSize_Count = 5
};

typedef u32 IFBEngineAssetMemoryArenaSize;

struct IFBEngineAssetMemoryArenaAllocator {

    union {
        
        struct {
            IFBEngineMemoryArenaAllocatorPtr arena_8KB;
            IFBEngineMemoryArenaAllocatorPtr arena_64KB;
            IFBEngineMemoryArenaAllocatorPtr arena_128KB;
            IFBEngineMemoryArenaAllocatorPtr arena_1MB;
            IFBEngineMemoryArenaAllocatorPtr arena_4MB;
        };

        IFBEngineMemoryArenaAllocatorPtr array[IFBEngineAssetMemoryArenaSize_Count];
    };
};

struct IFBEngineAssetMemoryIndexAllocator {
    IFBEngineMemoryArenaAllocatorPtr arena_allocator_8kb;
    IFBEngineMemoryArenaPtr          arena_8kb;
    memory                           stack_ptr;
};

struct IFBEngineAssetMemoryScratchAllocator {
    IFBEngineMemoryArenaAllocatorPtr arena_allocator_8mb;
    IFBEngineMemoryArenaPtr          arena_8mb;
    memory                           stack_ptr;
};

struct IFBEngineAssetMemoryAllocators {
    IFBEngineAssetMemoryBlockAllocator   block;
    IFBEngineAssetMemoryArenaAllocator   arena;    
    IFBEngineAssetMemoryIndexAllocator   index;
    IFBEngineAssetMemoryScratchAllocator scratch;
};

struct IFBEngineAssetsMemory {
    IFBEngineMemoryRegionPtr       region;
    IFBEngineAssetMemoryAllocators allocators;
};

IFBEngineAssetsMemoryPtr
ifb_engine_assets_memory_create_and_initialize();

IFBEngineMemoryArenaPtr
ifb_engine_assets_memory_arena_reserve(
    const u64 size_bytes);

void
ifb_engine_assets_memory_arena_release(
    IFBEngineMemoryArenaPtr asset_arena);

IFBEngineAssetMemoryBlockPtr
ifb_engine_assets_memory_block_allocate(
    const u64 size);

void
ifb_engine_assets_memory_block_free(
    IFBEngineAssetMemoryBlockPtr block);

IFBEngineAssetsFileIndexPtr
ifb_engine_assets_memory_index_buffer_push(
    u32 index_count);

memory 
ifb_engine_assets_memory_scratch_push(
    u64 size_bytes);

memory 
ifb_engine_assets_memory_scratch_pop(
    u64 size_bytes);


#endif //IFB_ENGINE_ASSETS_HPP