/*
 * Common header.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define NORETURN __attribute__((noreturn))
#define NOINLINE __attribute__((noinline))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define PACKED __attribute__((packed))
#define WAEK __attribute__((weak))
#define ALIGNED(x) __attribute__((aligned(x)))
#define CDECL __attribute__((__cdecl__))

#define UNUSED(x)  ((void)x)
#define STR(s)     STR_(s)
#define STR_(s)    #s

#define asm_volatile(x) __asm__ __volatile__(x)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;


