#ifndef IFB_TYPES_HPP
#define IFB_TYPES_HPP

#include <cstdint>
#include <xmmintrin.h>

/**********************************************************************************/
/* PRIMITIVES                                                                     */
/**********************************************************************************/

//signed integers
typedef int8_t   IFBS8;
typedef int16_t  IFBS16;
typedef int32_t  IFBS32;
typedef int64_t  IFBS64;

//unsigned integ 
typedef uint8_t  IFBU8;
typedef uint16_t IFBU16;
typedef uint32_t IFBU32;
typedef uint64_t IFBU64;

//floats
typedef float    IFBF32;
typedef double   IFBF64;

//booleans
typedef int8_t   IFBB8;
typedef int16_t  IFBB16;
typedef int32_t  IFBB32;
typedef int64_t  IFBB64;

//characters
typedef char     IFBChar;
typedef wchar_t  IFBWChar;

//memory
typedef void     IFBVoid;
typedef void*    IFBPtr;
typedef uint8_t  IFBByte;
typedef size_t   IFBSize;
typedef IFBU32   IFBIndex;
typedef IFBPtr   IFBHandle;
typedef intptr_t IFBAddr;

//time
typedef uint64_t IFBTimems;

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

//handles
struct IFBHND8;
struct IFBHND16;
struct IFBHND32;
struct IFBHND64;
struct IFBHNDPTR;

//system
struct IFBSystemInfo;
struct IFBSystemMemory;
struct IFBSystemCPU;

//memory
struct IFBHNDMemoryContext;
struct IFBHNDMemoryReservation;
struct IFBHNDMemoryArena;
struct IFBMemoryContext;
struct IFBMemoryArena;
struct IFBMemoryReservation;

//threading
struct IFBThread;
struct IFBThreadTask;
struct IFBThreadPool;
struct IFBThreadPlatformContext;

//math
struct IFBVec2;
struct IFBVec3;
struct IFBMat3;
struct IFBPoint;
struct IFBLine;
struct IFBTransform;

//data structures
struct IFBDataStructure;
struct IFBArray;
struct IFBStack;
struct IFBQueue;
struct IFBArrayList;
struct IFBLinkedList;
struct IFBHash;
struct IFBHashTable;

//graphics
struct IFBColorNormalized;
struct IFBColorHex;
struct IFBColor32;
struct IFBDimensions;
struct IFBPosition;
struct IFBDimensionsAndPosition;
struct IFBGraphicsContexts;
struct IFBWindow;
struct IFBMonitor;

//files
struct IFBHNDPlatformFile;
struct IFBFileReadOnly;
struct IFBFileReadWrite;

/**********************************************************************************/
/* HANDLES                                                                        */
/**********************************************************************************/

struct IFBHND8   { IFBU8  offset;  };
struct IFBHND16  { IFBU16 offset;  };
struct IFBHND32  { IFBU32 offset;  };
struct IFBHND64  { IFBU64 offset;  };
struct IFBHNDPTR { IFBPtr pointer; };

#define IFB_HANDLE_INVALID_U8  0xFF
#define IFB_HANDLE_INVALID_U16 0xFFFF 
#define IFB_HANDLE_INVALID_U32 0xFFFFFFFF 
#define IFB_HANDLE_INVALID_U64 0xFFFFFFFFFFFFFFFF 

/**********************************************************************************/
/* SYSTEM                                                                         */
/**********************************************************************************/

struct IFBSystemMemoryInfo {
    IFBU32 page_size;
    IFBU32 allocation_granularity;
};

struct IFBSystemCPUCacheInfo {
    IFBU32 size_total;
    IFBU32 size_line;
};

struct IFBSystemCPUInfo {
    IFBU32                parent_core_number;
    IFBU32                speed_mhz;
    IFBU32                core_count_physical;
    IFBU32                core_count_logical;
    IFBSystemCPUCacheInfo cache_l1;
    IFBSystemCPUCacheInfo cache_l2;
    IFBSystemCPUCacheInfo cache_l3;
};

struct IFBSystemInfo {
    IFBSystemCPUInfo    cpu;
    IFBSystemMemoryInfo memory;
};

/**********************************************************************************/
/* MEMORY                                                                         */
/**********************************************************************************/

struct IFBHNDMemoryContext     : IFBHNDPTR { };
struct IFBHNDMemoryReservation : IFBHNDPTR { };
struct IFBHNDMemoryArena       : IFBHNDPTR { };

struct IFBMemoryContext {
    IFBMemoryReservation* ptr_reservation_first;
    IFBMemoryReservation* ptr_reservation_last;
    IFBMemoryArena*       ptr_arena_first;
    IFBMemoryArena*       ptr_arena_last;
    IFBAddr               stack_start;
    IFBU32                stack_size;
    IFBU32                stack_position;
    IFBU32                count_reservations;
    IFBU32                count_arenas;
    IFBU32                system_page_size;
    IFBU32                system_granularity;
};

struct IFBMemoryReservation {
    IFBMemoryContext*     ptr_context;
    IFBMemoryReservation* ptr_next;
    IFBAddr               start;
    IFBU32                page_count_total;
    IFBU32                page_count_committed;
};

struct IFBMemoryArena {
    IFBMemoryReservation* ptr_reservation;
    IFBMemoryArena*       ptr_next;
    IFBAddr               start;
    IFBU32                size;
    IFBU32                position_committed;
    IFBU32                position_reserved;
};

/**********************************************************************************/
/* THREADING                                                                      */
/**********************************************************************************/

struct IFBHNDThread     : IFBHND32 { };
struct IFBHNDThreadPool : IFBHND32 { };

//task
typedef const IFBB8 (*IFBThreadTaskFunction) (IFBPtr data_ptr);

struct IFBThreadPool {
    struct {
        IFBThreadPlatformContext* array_context;                                           
        IFBU64*                   array_id;
        IFBU64*                   array_core_mask;
        IFBChar*                  description_buffer;
        IFBStack*                 tmp_stack;
    } pointers;
    IFBU32 thread_count_total;
    IFBU32 thread_count_running;
    IFBU32 description_stride;
};

struct IFBThreadTask {
    IFBPtr                data;
    IFBThreadTaskFunction func;
};

struct IFBThreadPlatformContext {
    IFBPtr                platform_data_pointer;
    IFBU64                platform_data_size;   
    IFBPtr                task_data_pointer;
    IFBThreadTaskFunction task_func_pointer;
};


struct IFBThread {
    IFBThreadPlatformContext platform_context;
    IFBChar*                 description;
    IFBU64                   id;
    IFBU64                   core_mask;
};

struct IFBThreadStatus {
    IFBU64 id;
    IFBU64 core_mask;
    IFBU32 active_core;
    IFBB32 running;
};

/**********************************************************************************/
/* SIMD                                                                           */
/**********************************************************************************/

#if _MSC_VER
    #if !defined(__clang__)
        #define INSTRUCTION_REORDER_BARRIER _ReadWriteBarrier()
    #endif //!defined(__clang__)
    #include <intrin.h>
#else //_MSC_VER
    #include <x86intrin.h>
#endif //_MSC_VER

typedef __m128 IFBXMMF128;

/**********************************************************************************/
/* MATH                                                                           */
/**********************************************************************************/

struct IFBVec2 {
    union {
        struct {
            IFBF32 x;
            IFBF32 y;
        };

        IFBF32 xy[2];

    };

};

struct IFBVec3 {
    union {
        struct {
            IFBF32 x;
            IFBF32 y;
            IFBF32 z;
        };
        IFBF32 xyz[3];
    };
};

struct IFBMat3 {
    union {
        struct {
            IFBF32 r0c0;
            IFBF32 r0c1;
            IFBF32 r0c2;
            IFBF32 r1c0;
            IFBF32 r1c1;
            IFBF32 r1c2;
            IFBF32 r2c0;
            IFBF32 r2c1;
            IFBF32 r2c2;
        };
        struct {
            IFBVec3 row_vec_0;
            IFBVec3 row_vec_1;
            IFBVec3 row_vec_2;
        };
        IFBF32 array[9];        
    };
};

struct IFBTransform {
    IFBVec2 translation;
    IFBVec2 scale;
    IFBF32 rotation_radians;
};

struct IFBPoint {
    IFBF32 x;
    IFBF32 y;
};

struct IFBLine {
    IFBPoint point_a;
    IFBPoint point_b;
};

/**********************************************************************************/
/* DATA STRUCTURES                                                                */
/**********************************************************************************/

struct IFBDataStructure {
    IFBAddr             data_start;
    IFBU32              data_size;
};

struct IFBStack : IFBDataStructure {
    IFBU32 position;
};


/**********************************************************************************/
/* GRAPHICS                                                                       */
/**********************************************************************************/

struct IFBDimensions {
    IFBU32 width;
    IFBU32 height;
};

struct IFBPosition {
    IFBU32 x;
    IFBU32 y;
};

struct IFBDimensionsAndPosition {
    IFBDimensions dimensions;
    IFBPosition   position;    
};

struct IFBColorNormalized {
    IFBF32 red;
    IFBF32 blue;
    IFBF32 green;
    IFBF32 alpha;
};

struct IFBColorHex {
    IFBU8 red;
    IFBU8 blue;
    IFBU8 green;
    IFBU8 alpha;
};

struct IFBColor32 {
    IFBU32 value;
};

enum IFBColorFormat : IFBU32 {
     IFBColorFormat_RGBA = 0,
     IFBColorFormat_ARGB = 1,
     IFBColorFormat_ABGR = 2,
     IFBColorFormat_BGRA = 3
};

#define IFB_WINDOW_TITLE_LENGTH_MAX 255

typedef IFBPtr IFBGLContext;
typedef IFBPtr IFBImGuiContext;

struct IFBGraphicsContexts {
    IFBGLContext    opengl;
    IFBImGuiContext imgui;
};

struct IFBWindow {
    IFBPosition         position;
    IFBDimensions       dimensions;
    IFBGraphicsContexts graphics_contexts;
    IFBB32              visible;
    IFBB32              quit_received;
    IFBChar*            title;
};

struct IFBMonitor {
    IFBDimensions dimensions;
    IFBPosition   position;
    IFBU32        refresh_hz;
    IFBU32        index;
};

/**********************************************************************************/
/* FILES                                                                          */
/**********************************************************************************/

struct IFBFileReadOnly {
    IFBStack* platform_context_stack;
    IFBB32    platform_lock;
    IFBU32    size;
    IFBU32    bytes_read;
};

struct IFBFileReadWrite {
    IFBStack* platform_context_stack;
    IFBB32    platform_lock;
    IFBU32    size;
    IFBU32    bytes_read;
    IFBU32    bytes_written;
};

#endif //IFB_TYPES_HPP