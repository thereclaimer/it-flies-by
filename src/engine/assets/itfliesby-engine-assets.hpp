#ifndef ITFLIESBY_ENGINE_ASSETS_HPP
#define ITFLIESBY_ENGINE_ASSETS_HPP

#include <stdio.h>
#include <string.h>

#include "itfliesby-engine.hpp"
#include "itfliesby-engine-globals.hpp"

#define ITFLIESBY_ENGINE_ASSETS_MEMORY_PARTITION_SIZE         ITFLIESBY_MATH_MEGABYTES(64)
#define ITFLIESBY_ENGINE_ASSETS_MEMORY_BLOCK_SIZE_INDEX       ITFLIESBY_MATH_KILOBYTES(1)
#define ITFLIESBY_ENGINE_ASSETS_MEMORY_BLOCK_SIZE_ASSET_DATA  ITFLIESBY_MATH_MEGABYTES(8)
#define ITFLIESBY_ENGINE_ASSETS_MEMORY_ALLOCATOR_HEADER_SIZE  ITFLIESBY_MATH_KILOBYTES(1)
#define ITFLIESBY_ENGINE_ASSETS_MEMORY_ALLOCATOR_IMAGE_SIZE   ITFLIESBY_MATH_MEGABYTES(8)

enum ItfliesbyEngineAssetsFileId : s32 {
    ITFLIESBY_ASSETS_FILE_ID_INVALID = -1,
    ITFLIESBY_ASSETS_FILE_ID_SHADERS =  0,
    ITFLIESBY_ASSETS_FILE_ID_IMAGES  =  1,
    ITFLIESBY_ASSETS_FILE_ID_COUNT   =  2
};

//these have to match the order of the indexes in assets-shader-table.csv
enum ItfliesbyEngineAssetsShader : s32 {
    ITFLIESBY_ENGINE_ASSETS_SHADER_INVALID                       = -1,
    ITFLIESBY_ENGINE_ASSETS_SHADER_TEXTURED_QUAD_VERTEX_SHADER   =  0,
    ITFLIESBY_ENGINE_ASSETS_SHADER_TEXTURED_QUAD_FRAGMENT_SHADER =  1,
    ITFLIESBY_ENGINE_ASSETS_SHADER_SOLID_QUAD_VERTEX_SHADER      =  2,
    ITFLIESBY_ENGINE_ASSETS_SHADER_SOLID_QUAD_FRAGMENT_SHADER    =  3,
    ITFLIESBY_ENGINE_ASSETS_SHADER_TEST_VERTEX_SHADER            =  4,
    ITFLIESBY_ENGINE_ASSETS_SHADER_TEST_FRAGMENT_SHADER          =  5,
    ITFLIESBY_ENGINE_ASSETS_SHADER_COUNT                         =  6
};

enum ItfliesbyEngineAssetsImage : s32 {
    ITFLIESBY_ENGINE_ASSETS_IMAGE_INVALID            = -1, 
    ITFLIESBY_ENGINE_ASSETS_IMAGE_CALIBRATION_CONNOR =  0,
    ITFLIESBY_ENGINE_ASSETS_IMAGE_CALIBRATION_JIG    =  1,
    ITFLIESBY_ENGINE_ASSETS_IMAGE_COUNT              =  2
};

typedef itfliesby_memory_allocator_block  itfliesby_engine_assets_allocator_index;
typedef itfliesby_memory_allocator_block  itfliesby_engine_assets_allocator_data;
typedef itfliesby_memory_allocator_linear itfliesby_engine_assets_allocator_header;

struct ItfliesbyEngineAssetsMemory {
    itfliesby_memory_partition               partition;
    itfliesby_engine_assets_allocator_index  index_allocator;
    itfliesby_engine_assets_allocator_data   asset_data_allocator;
    itfliesby_engine_assets_allocator_header asset_header_allocator;
    itfliesby_memory_allocator_linear        asset_allocator_image;    
};

PACK(
    struct ItfliesbyEngineAssetsFileindex {
        char tag[32];         // plaintext identifier for the entity the asset belongs to
        u32  file_size;       // size of the data is stored in the file
        u32  allocation_size; // the size of the space we need to allocate when storing the asset data in memory
        u32  offset;          // the index of the first byte of asset data in the file
    };
);

#define ITFLIESBY_ASSET_FILE_INDEX_SIZE                    ((sizeof(u32) * 3) + 32)
#define ITFLIESBY_ASSET_FILE_INDEX_SIZE_TOTAL(num_indexes) (ITFLIESBY_ASSET_FILE_INDEX_SIZE * num_indexes)
#define ITFLIESBY_ASSET_FILE_VERIFICATION_SIZE             (sizeof(u32) + 3)
#define ITFLIESBY_ASSET_FILE_HEADER_SIZE(num_indexes)      (ITFLIESBY_ASSET_FILE_VERIFICATION_SIZE + ITFLIESBY_ASSET_FILE_INDEX_SIZE_TOTAL(num_indexes))

struct ItfliesbyEngineAssetsFileIndexStore {
    ItfliesbyEngineAssetsFileindex shader_indexes[ITFLIESBY_ENGINE_ASSETS_SHADER_COUNT];
    ItfliesbyEngineAssetsFileindex image_indexes[ITFLIESBY_ENGINE_ASSETS_IMAGE_COUNT];
};

const char* ITFLIESBY_ENGINE_ASSETS_FILE_PATHS[] = {
    "ItFliesBy.Assets.Shaders.ifb",
    "ItFliesBy.Assets.Images.ifb"
};

struct ItfliesbyEngineAssetsFileHandles {
    union {
        struct {
            handle shader_asset_file;
            handle image_asset_file;
        };
        handle array[ITFLIESBY_ASSETS_FILE_ID_COUNT];
    }; 
};

struct ItfliesbyEngineAssets {
    ItfliesbyEngineAssetsFileHandles    file_handles;
    ItfliesbyEngineAssetsFileIndexStore file_index_store;
};

void
itfliesby_engine_assets_init(
    ItfliesbyEngineAssets* assets,
    itfliesby_memory_arena arena
);

void
itfliesby_engine_assets_update(
    ItfliesbyEngineAssets* assets
);

void
itfliesby_engine_assets_file_handles_load(
    ItfliesbyEngineAssetsFileHandles* assets);

u64
itfliesby_engine_assets_index_allocation_size(
    ItfliesbyEngineAssetsFileindex* indexes,
    s32*                            index_ids,
    s32                             index_ids_count);

u64
itfliesby_engine_assets_shader_allocation_size(
    ItfliesbyEngineAssets*       assets,
    ItfliesbyEngineAssetsShader* shader_index_ids,
    s32                          shader_index_ids_count);

void
itfliesby_engine_assets_load_shaders(
    ItfliesbyEngineAssets*       assets,
    ItfliesbyEngineAssetsShader* shader_index_ids,
    memory                       shader_memory,
    u64*                         shader_offsets,
    u32                          shader_count);

//TODO: way to load multiple images at once

struct ItfliesbyEngineAssetsImageData {
    u32    width_pixels;
    u32    height_pixels;
    memory pixels;
};

u64
itfliesby_engine_assets_image_allocation_size(
    ItfliesbyEngineAssets*     assets,
    ItfliesbyEngineAssetsImage image
);

void
itfliesby_engine_assets_image_load(
    ItfliesbyEngineAssets*          assets,
    ItfliesbyEngineAssetsImage      image,
    ItfliesbyEngineAssetsImageData* image_data
);


#endif //ITFLIESBY_ENGINE_ASSETS_HPP
