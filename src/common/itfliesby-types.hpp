#ifndef ITFLIESBY_TYPES_HPP
#define ITFLIESBY_TYPES_HPP

#include <cstdint>

#define internal static
#define local    static
#define global   static
#define external extern "C" __declspec(dllexport)
#define api      extern "C" __declspec(dllimport)
#define null     0

#define ifb_assert(condition) if(!(condition)) *((int*)(NULL)) = 1
#define ifb_nop()   ifb_assert(true)
#define ifb_panic() ifb_assert(false)

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef s8  b8;
typedef s16 b16;
typedef s32 b32;
typedef s64 b64;

typedef float  f32;
typedef double f64;

typedef void* handle;

typedef u8    byte;
typedef byte* memory;

struct ItfliesbyViewportDimensions {
    u32 width;
    u32 height;
};


#endif //ITFLIESBY_TYPES_HPP