#ifndef ARCH_TYPES_H
#define ARCH_TYPES_H

#define NORETURN __attribute__((noreturn))
#define PACKED __attribute__((packed))
#define CDECL __attribute__((__cdecl__))

#define NULL ((void *)0)

typedef unsigned char u8;
typedef char s8;

typedef unsigned short u16;
typedef short s16;

typedef unsigned int u32;
typedef int s32;

#endif  // ARCH_TYPES_H

