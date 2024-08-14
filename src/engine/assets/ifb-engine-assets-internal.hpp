#ifndef IFB_ENGINE_ASSETS_INTERNAL_HPP
#define IFB_ENGINE_ASSETS_INTERNAL_HPP

#include "ifb-engine-assets-new.hpp"

/********************************************************************************************/
/* FORWARD DECLARATIONS                                                                     */
/********************************************************************************************/

struct IFBEngineAssetsContext;

struct IFBEngineAssetsMemory;

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
    char*                     path
    handle                    handle;
    size_t                    size;
    size_t                    index_count;
    IFBEngineAssetsFileIndex* index_array;
};

namespace ifb_engine_assets {

    internal const IFBEngineAssetsFile* file_open  (const char* file_path);
    internal void                       file_close (const IFBEngineAssetsFile* asset_file);
};

/********************************************************************************************/
/* FILE                                                                                     */
/********************************************************************************************/



#endif //IFB_ENGINE_ASSETS_INTERNAL_HPP