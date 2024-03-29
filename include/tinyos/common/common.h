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
#define CONSTRUCTOR __attribute__((constructor))

#define UNUSED(x)  ((void)x)
#define STR(s)     STR_(s)
#define STR_(s)    #s

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define ASSERT(x) do { /* nothing */ } while (0)
#define STATIC_ASSERT(x) _Static_assert(x, "Static assertion failed")

#define asm(x) __asm__(x)
#define asm_volatile(...) __asm__ __volatile__(__VA_ARGS__)

#define likely(x) __builtin_expect(!!(x), true)
#define unlikely(x) __builtin_expect(!!(x), false)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
