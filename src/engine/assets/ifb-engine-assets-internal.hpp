#ifndef IFB_ENGINE_ASSETS_INTERNAL_HPP
#define IFB_ENGINE_ASSETS_INTERNAL_HPP

#include "ifb-engine-assets-new.hpp"

/********************************************************************************************/
/* FORWARD DECLARATIONS                                                                     */
/********************************************************************************************/

//context
struct IFBEngineAssetsContext;

//file
struct IFBEngineAssetsFile;
struct IFBEngineAssetsFileReadBuffer;
struct IFBEngineAssetsFileTable;
struct IFBEngineAssetsFileIndex;
struct IFBEngineAssetsFileIndexTable;

//memory
struct IFBEngineAssetsMemory;
struct IFBEngineAssetsMemoryRegions;
struct IFBEngineAssetsMemoryBlock_Impl;
struct IFBEngineAssetsMemoryBlockAllocator;

/********************************************************************************************/
/* FILE                                                                                     */
/********************************************************************************************/

struct IFBEngineAssetsFileIndex {
    char tag[32];         // plaintext identifier for the entity the asset belongs to
    u32  file_size;       // size of the data is stored in the file
    u32  allocation_size; // the size of the space we need to allocate when storing the asset data in memory
    u32  offset;          // the index of the first byte of asset data in the file
};

struct IFBEngineAssetsFileIndexTable {
    IFBEngineAssetsMemoryBlock_Impl* block;
    size_t                           index_count;
    IFBEngineAssetsFileIndex*        indexes;    
};


struct IFBEngineAssetsFile {
    IFBEngineAssetsType              type;
    handle                           file_handle;
    IFBEngineAssetsFileIndexTable    index_table;
};

struct IFBEngineAssetsFileTable {
    union {
        struct {
            IFBEngineAssetsFile shader;
            IFBEngineAssetsFile images;
        };

        IFBEngineAssetsFile array[IFBEngineAssetsType_Count];    
    };
};

namespace ifb_engine_assets {

    internal void file_table_create (void);
    internal void file_open         (const IFBEngineAssetsType type);
    internal void file_close        (const IFBEngineAssetsType type);
};

/********************************************************************************************/
/* MEMORY                                                                                   */
/********************************************************************************************/

struct IFBEngineAssetsMemoryBlock_Impl {
    IFBEngineMemoryArena             arena;
    IFBEngineAssetsMemoryBlock_Impl* next;
    IFBEngineAssetsMemoryBlock_Impl* previous;
    size_t                           size;
    memory                           start;
};

struct IFBEngineAssetsMemoryBlockAllocator {
    IFBEngineMemoryRegion            region;
    size_t                           total_size;
    size_t                           block_size;
    IFBEngineAssetsMemoryBlock_Impl* blocks;
};


struct IFBEngineAssetsMemory {
    IFBEngineMemoryReservation          reservation;
    IFBEngineAssetsMemoryBlockAllocator block_allocator;    
};

namespace ifb_engine_assets {

    internal void memory_reserve(void);
    internal void memory_release(void);
    
    internal const IFBEngineAssetsMemoryBlock* memory_block_commit   (const size_t size);
    internal       void                        memory_block_decommit (const IFBEngineAssetsMemoryBlock* block); 
};

/********************************************************************************************/
/* CONTEXT                                                                                  */
/********************************************************************************************/

struct IFBEngineAssetsContext {
    IFBEngineAssetsMemory    memory;
    IFBEngineAssetsFileTable file_table;    
};

namespace ifb_engine_assets {

    internal IFBEngineAssetsContext& context_get();
}

#endif //IFB_ENGINE_ASSETS_INTERNAL_HPP