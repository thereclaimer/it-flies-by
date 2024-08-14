#ifndef IFB_ENGINE_ASSETS_INTERNAL_HPP
#define IFB_ENGINE_ASSETS_INTERNAL_HPP

#include "ifb-engine-assets-new.hpp"

/********************************************************************************************/
/* FORWARD DECLARATIONS                                                                     */
/********************************************************************************************/

struct IFBEngineAssetsContext;
struct IFBEngineAssetsMemory;
struct IFBEngineAssetsDataBlock_Impl;
struct IFBEngineAssetsFile;

/********************************************************************************************/
/* MEMORY                                                                                   */
/********************************************************************************************/

struct IFBEngineAssetsMemory {
    IFBEngineMemoryReservation reservation;
    struct {
        IFBEngineMemoryRegion file;
        IFBEngineMemoryRegion request;
        IFBEngineMemoryRegion data;
    } regions;
};

namespace ifb_engine_assets {

    internal void memory_reserve (void);
    internal void memory_release (void);

    internal const IFBEngineMemoryArena memory_arena_file_commit    (void);
    internal const IFBEngineMemoryArena memory_arena_request_commit (void);
    internal const IFBEngineMemoryArena memory_arena_data_commit    (void);
    
    internal void memory_arena_decommit (const IFBEngineMemoryArena arena);
};

/********************************************************************************************/
/* DATA                                                                                     */
/********************************************************************************************/

struct IFBEngineAssetsDataBlock_Impl {
    IFBEngineMemoryArena           arena;
    IFBEngineAssetsFile*           file;
    IFBEngineAssetsDataBlock_Impl* next;
    IFBEngineAssetsDataBlock_Impl* previous;
    size_t                         size;
    size_t                         index;
    memory                         start;
};

namespace ifb_engine_assets {

    internal IFBEngineAssetsDataBlock_Impl* data_block_commit   (const size_t data_size);
    internal void                           data_block_decommit (IFBEngineAssetsDataBlock_Impl* data_block);
};

/********************************************************************************************/
/* FILE                                                                                  */
/********************************************************************************************/

struct IFBEngineAssetsFileIndex {
    char tag[32];         
    u32  file_size;       
    u32  allocation_size; 
    u32  offset;          
};

struct IFBEngineAssetsFile {
    IFBEngineMemoryArena      arena;
    char*                     path;
    handle                    file_handle;
    size_t                    size;
    size_t                    index_count;
    IFBEngineAssetsFileIndex* index_array;
};

namespace ifb_engine_assets {


    internal const IFBEngineAssetsFile*     file_open  (const char* file_path);
    internal void                           file_close (const IFBEngineAssetsFile* asset_file);
    
    internal IFBEngineAssetsDataBlock_Impl* 
    file_read(
        const IFBEngineAssetsFile* asset_file,
        const size_t               asset_index);
};




#endif //IFB_ENGINE_ASSETS_INTERNAL_HPP