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
    IFBEngineMemoryArena      arena;
    size_t                    index_count;
    IFBEngineAssetsFileIndex* indexes;    
};

struct IFBEngineAssetsFileReadBuffer {
    IFBEngineMemoryArena arena;
    memory               start;
    size_t               size;
};

struct IFBEngineAssetsFile {
    IFBEngineAssetsType           type;
    handle                        file_handle;
    IFBEngineAssetsFileIndexTable index_table;
    IFBEngineAssetsFileReadBuffer read_buffer
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

    internal void file_table_create ();
    internal void file_open         (const IFBEngineAssetsType type);
    internal void file_close        (const IFBEngineAssetsType type);
};

/********************************************************************************************/
/* MEMORY                                                                                   */
/********************************************************************************************/

struct IFBEngineAssetsMemoryRegions {
    IFBEngineMemoryRegion index;
    IFBEngineMemoryRegion read_buffer;
    IFBEngineMemoryRegion data;
};

struct IFBEngineAssetsMemory {
    IFBEngineMemoryReservation   reservation;
    IFBEngineAssetsMemoryRegions regions;
};  

struct IFBEngineAssetsContext {
    IFBEngineAssetsMemory memory;
    IFBEngineAssetsFiles  files;    
};

struct IFBEngineAssetsMemoryBuffer {
    IFBEngineMemoryArena arena;
    size_t               size;
    memory               start;
};

namespace ifb_engine_assets {

    internal void memory_reserve();
    internal void memory_release();

    internal void memory_allocate_read_buffer (IFBEngineAssetsMemoryBuffer&   read_buffer, const size_t size);
    internal void memory_allocate_data_buffer (IFBEngineAssetsMemoryBuffer&   data_buffer, const size_t size);
    internal void memory_allocate_index_table (IFBEngineAssetsFileIndexTable& index_table, const size_t index_count); 

    internal void memory_free_read_buffer (IFBEngineAssetsMemoryBuffer&   read_buffer);
    internal void memory_free_data_buffer (IFBEngineAssetsMemoryBuffer&   data_buffer);
    internal void memory_free_index_table (IFBEngineAssetsFileIndexTable& index_table); 
};

/********************************************************************************************/
/* CONTEXT                                                                                  */
/********************************************************************************************/

namespace ifb_engine_assets {
    
}

#endif //IFB_ENGINE_ASSETS_INTERNAL_HPP