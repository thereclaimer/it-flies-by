#ifndef IFB_ENGINE_ASSETS_HPP
#define IFB_ENGINE_ASSETS_HPP

#include <ifb.hpp>

#include "ifb-engine-memory.hpp"

struct  IFBEngineAssets;
typedef IFBEngineAssets* IFBEngineAssetsPtr;
typedef IFBEngineAssets& IFBEngineAssetsRef;

#define IFB_ENGINE_ASSET_ARENA_TAG_SHADER "ENGINE ASSETS SHADER"
#define IFB_ENGINE_ASSET_ARENA_TAG_IMAGE  "ENGINE ASSETS IMAGE"

enum IFBEngineAssetFileId_ : s32 {
    IFBEngineAssetFileId_invalid = -1,
    IFBEngineAssetFileId_shaders =  0,
    IFBEngineAssetFileId_images  =  1,
    IFBEngineAssetFileId_count   =  2
};

const char* ITFLIESBY_ENGINE_ASSETS_FILE_PATHS[] = {
    "ItFliesBy.Assets.Shaders.ifb", // IFBEngineAssetFileId_shaders
    "ItFliesBy.Assets.Images.ifb"   // IFBEngineAssetFileId_images
};

#define IFB_ENGINE_ASSET_MEMORY_REGION_SIZE  IFB_MATH_MEGABYTES(64)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_8KB    IFB_MATH_KILOBYTES(8)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_64KB   IFB_MATH_KILOBYTES(64)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_128KB  IFB_MATH_KILOBYTES(128)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_1MB    IFB_MATH_MEGABYTES(1)
#define IFB_ENGINE_ASSET_MEMORY_ARENA_4MB    IFB_MATH_MEGABYTES(4)


struct IFBEngineAssetMemory {
    IFBEngineMemoryRegionPtr         region;
    IFBEngineMemoryArenaAllocatorPtr asset_arena_allocator_8kb;
    IFBEngineMemoryArenaAllocatorPtr asset_arena_allocator_64kb;
    IFBEngineMemoryArenaAllocatorPtr asset_arena_allocator_128kb;
    IFBEngineMemoryArenaAllocatorPtr asset_arena_allocator_1mb;
    IFBEngineMemoryArenaAllocatorPtr asset_arena_allocator_4mb;
};

struct IFBEngineAssetIndex {
    char tag[32];         // plaintext identifier for the entity the asset belongs to
    u32  file_size;       // size of the data is stored in the file
    u32  allocation_size; // the size of the space we need to allocate when storing the asset data in memory
    u32  offset;          // the index of the first byte of asset data in the file
};

inline u64
ifb_engine_asset_indexes_allocation_size(
    u32 indexes_count) {

    u64 index_allocation_size = 
        sizeof(IFBEngineAssetIndex) *
        indexes_count;

    return(index_allocation_size);
}

typedef IFBEngineAssetIndex* IFBEngineAssetIndexPtr; 
typedef IFBEngineAssetIndex& IFBEngineAssetIndexRef;

struct IFBEngineAssetFile {
    u32                     indexes_count;
    IFBEngineAssetIndexPtr  indexes;
    handle                  file_handle;
    IFBEngineMemoryArenaPtr arena;
};

struct IFBEngineAssetFiles {
    union {
        struct {
            IFBEngineAssetFile shader; // IFBEngineAssetFileId_shaders
            IFBEngineAssetFile image;  // IFBEngineAssetFileId_images
        };

        IFBEngineAssetFile array[IFBEngineAssetFileId_count];
    };
};

struct IFBEngineAssets {
    IFBEngineAssetMemory memory;
    IFBEngineAssetFiles  files;
};

IFBEngineAssetsPtr
ifb_engine_assets_create_and_initialize();

#endif //IFB_ENGINE_ASSETS_HPP