/*
 * Common headers.
 */

#pragma once

#include <stdint.h>

#define NORETURN    __attribute__((noreturn))
#define NOINLINE    __attribute__((noinline))
#define PACKED      __attribute__((packed))
#define ALIGNED(x)  __attribute__((aligned(x)))
#define CDECL       __attribute__((__cdecl__))

#define NULL       0
#define UNUSED(x)  ((void)x)
#define STR(s)     STR_(s)
#define STR_(s)    #s

typedef uint8_t u8;
typedef int8_t s8;

typedef uint16_t u16;
typedef int16_t s16;

typedef uint32_t u32;
typedef int32_t s32;

typedef uint64_t u64;
typedef int64_t s64;

