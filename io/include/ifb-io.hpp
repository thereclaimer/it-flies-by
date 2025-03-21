#ifndef IFB_IO_HPP
#define IFB_IO_HPP

#include <ifb.hpp>
#include <ifb-memory.hpp>
#include <ifb-data-structures.hpp>


/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

//handles
struct IFBHNDFile;
struct IFBHNDFileTable;

//context
struct IFBFile;
struct IFBFileBuffer;
struct IFBFileContext;

//tables
struct IFBFileTableArgs;
struct IFBFileTableReadOnly;
struct IFBFileTableReadOnlyHeader;
struct IFBFileTableReadOnlyHandles;
struct IFBFileTableReadWrite;
struct IFBFileTableReadWriteHeader;
struct IFBFileTableReadWriteHandles;

//requests
struct IFBFileRequest;
struct IFBFileRequestHeader;
struct IFBFileRequestHandles;

//callbacks
typedef IFBU32 (*IFBFileAsyncCallback) (IFBFileContext* file_context);
struct IFBFileAsyncCallbacks;

/**********************************************************************************/
/* HANDLES                                                                        */
/**********************************************************************************/

struct IFBHNDFile      : IFBHND16 { };
struct IFBHNDFileTable : IFBHND32 { };

/**********************************************************************************/
/* FILE CONTEXT                                                                   */
/**********************************************************************************/

struct IFBFile {
    IFBHNDFileTable file_table;
    IFBU32          size;
    IFBU16          table_index;
    IFBB16          read_only;
};

struct IFBFileBuffer {
    IFBByte* start;
    IFBU64   size;
};

struct IFBFileAsyncCallbacks {
    IFBFileAsyncCallback read;
    IFBFileAsyncCallback write;
};

struct IFBFileContext {
    IFBMemoryBlock        platform_memory;
    IFBFileAsyncCallbacks callbacks;
    IFBU64                bytes_transferred;
};

/**********************************************************************************/
/* REQUESTS                                                                       */
/**********************************************************************************/

struct IFBFileRequestHeader {
    IFBMemoryBlock memory;
    IFBU32         file_count;
    IFBU32         file_path_stride;
};

struct IFBFileRequestHandles {
    IFBHND32 context;
    union {
        IFBHND32 buffer;
        IFBHND32 file_path;
    };
};

struct IFBFileRequest {
    IFBFileRequestHeader  header;
    IFBFileRequestHandles handles;
};

/**********************************************************************************/
/* TABLES                                                                         */
/**********************************************************************************/

struct IFBFileTableArgs {
    IFBMemoryArena*      arena;
    IFBFileAsyncCallback file_callback_read;
    IFBFileAsyncCallback file_callback_write;
    IFBChar*             file_path_buffer;
    IFBU32               file_path_stride; 
    IFBU32               file_count;
};

struct IFBFileTableReadOnlyHeader {
    IFBMemoryBlock       memory;
    IFBFileAsyncCallback callback;
    IFBU32               file_count;
    IFBU32               file_path_stride;
};

struct IFBFileTableReadOnlyHandles {
    IFBHND16 file_path_buffer;
    IFBHND16 array_file_context;
    IFBHND16 array_last_bytes_read;
    IFBHND16 array_list_files_open;
    IFBHND16 array_list_files_closed;
    IFBHND16 array_list_files_locked;
    IFBHND16 context_data;
};

struct IFBFileTableReadOnly {
    IFBFileTableReadOnlyHeader  header;
    IFBFileTableReadOnlyHandles handles;
};


/**********************************************************************************/
/* FILE TABLE ARGS                                                                */
/**********************************************************************************/

namespace ifb_file_table {

    //read only
    const IFBHNDFileTable commit_read_only(const IFBFileTableArgs* args); 
};

/**********************************************************************************/
/* FILE TABLE READ ONLY                                                           */
/**********************************************************************************/

namespace ifb_file_ro {

    const IFBB8 file_open           (const IFBFileRequest* request);
    const IFBB8 file_close          (const IFBFileRequest* request);
    const IFBB8 file_read_immediate (const IFBFileRequest* request);
    const IFBB8 file_read_async     (const IFBFileRequest* request);
};

#endif //IFB_IO_HPP