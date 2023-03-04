/*
 * Common headers.
 */

#pragma once

#include <stdint.h>

#define NORETURN __attribute__((noreturn))
#define NOINLINE __attribute__((noinline))
#define PACKED   __attribute__((packed))
#define CDECL    __attribute__((__cdecl__))

#define NULL 0
#define UNUSED(x) ((void)x)

#define FLAG_CF 0b1
#define FLAG_PF 0b100
#define FLAG_ZF 0b1000000

typedef uint8_t u8;
typedef int8_t s8;

typedef uint16_t u16;
typedef int16_t s16;

typedef uint32_t u32;
typedef int32_t s32;

typedef uint64_t u64;
typedef int64_t s64;

static inline u8 inb(u16 port) {
  u8 result;
  __asm__ __volatile__ ("in al, dx" : "=a" (result) : "d" (port));
  return result;
}

static inline u16 inw(u16 port) {
  u16 result;
  __asm__ __volatile__ ("in ax, dx" : "=a" (result) : "d" (port));
  return result;
}

static inline u32 ind(u16 port) {
  u32 result;
  __asm__ __volatile__ ("in eax, dx" : "=a" (result) : "d" (port));
  return result;
}

static inline void outb(u16 port, u8 data) {
  __asm__ __volatile__ ("out dx, al" : : "a" (data), "d" (port));
}

static inline void outw(u16 port, u16 data) {
  __asm__ __volatile__ ("out dx, ax" : : "a" (data), "d" (port));
}

static inline void outd(u16 port, u32 data) {
  __asm__ __volatile__ ("out dx, eax" : : "a" (data), "d" (port));
}

static inline void io_delay(void) {
  const u16 delay_port = 0x80;
  outb(delay_port, 0);
}

static inline void cli(void) {
  __asm__ __volatile__("cli");
}

static inline void sti(void) {
  __asm__ __volatile__("sti");
}

static inline void set_fs(u16 seg) {
  __asm__ __volatile__("mov fs, ax;" : : "a" (seg));
}

static inline void set_gs(u16 seg) {
  __asm__ __volatile__("mov gs, ax;" : : "a" (seg));
}

static inline u16 get_fs(void) {
  u16 result;
  __asm__ __volatile__("mov ax, fs;" : "=a" (result));
  return result;
}

static inline u16 get_gs(void) {
  u16 result;
  __asm__ __volatile__("mov ax, gs;" : "=a" (result));
  return result;
}

static inline u32 get_eflags(void) {
  u32 result;
  __asm__ __volatile__(
    "pushfd;"
    "pop eax;"
      : "=a" (result));
  return result;
}

static inline void halt(void) {
  __asm__ __volatile__("hlt");
}

struct regs {
  union {
    struct {
      u32 edi;
      u32 esi;
      u32 ebp;
      u32 _esp;   /* Do not change! */
      u32 ebx;
      u32 edx;
      u32 ecx;
      u32 eax;
      u32 eflags;
      u32 gsfs;
    };
    struct {
      u16 di, dih;
      u16 si, sih;
      u16 bp, bph;
      u16 _sp, _sph;   /* Do not change! */
      u16 bx, bxh;
      u16 dx, dxh;
      u16 cx, cxh;
      u16 ax, axh;
      u16 flags, flagsh;
      u16 gs, fs;
    };
    struct {
      u8 dill, dilh, dihl, dihh;
      u8 sill, silh, sihl, sihh;
      u8 bpl, bplh, bphl, bphh;
      u8 _spll, _splh, _sphl, _sphh;   /* Do not change! */
      u8 bl, bh, bxhl, bxhh;
      u8 dl, dh, dxhl, dxhh;
      u8 cl, ch, cxhl, cxhh;
      u8 al, ah, axhl, axhh;
    };
  };
};

/* biosint.asm */
void biosint(u8 no_int, const struct regs *in, struct regs *out);

/* regs.c */
void regsinit(struct regs *r);

/* string.c */
void *memset(void *src, unsigned char c, unsigned n);

void *memcpy(void *dst, void *src, unsigned n);

/* stdio.c */

#define DEBUG 1

#ifdef DEBUG
#  define SAYF(x...) printf(x)
#else
#  define SAYF(x...)
#endif

#define INFO(x...) do { \
    SAYF("[*] " x); \
    SAYF("\n"); \
  } while (0)

#define OK(x...) do { \
    SAYF("[+] " x); \
    SAYF("\n"); \
  } while (0)

#define WARN(x...) do { \
    SAYF("[!] WARNING: " x); \
    SAYF("\n"); \
  } while (0)

#define FATAL(x...) do { \
    SAYF("[-] FATAL: " x); \
    SAYF("\n"); \
    for(;;); \
  } while (0)

void putchar(char c);
void puts(const char *s);
void printf(const char *fmt, ...);

/* boot_params */

#define MEMORY_MAX_ENTRIES 128

typedef struct {
  u64 addr;
  u64 length;
  u32 type;
} PACKED memory_entry_t;

typedef struct boot_params_s {
  struct {
    u8 mode;
    u8 page;
    u8 x;
    u8 y;
    u8 cols;
    u8 lines;
  } video;

  memory_entry_t memory_map[MEMORY_MAX_ENTRIES];
  u16 memory_entries;

} PACKED boot_params_t;

/* video.c */
void set_video(void);
void store_video(void);

/* memory */
void detect_memory_e820(void);

/* loader.c */
void NORETURN load_kernel(void);


