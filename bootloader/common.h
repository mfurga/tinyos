#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

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

#define X86_FLAGS_CF   0x0001
#define X86_FLAGS_PF   0x0004
#define X86_FLAGS_AF   0x0010
#define X86_FLAGS_ZF   0x0040
#define X86_FLAGS_SF   0x0080
#define X86_FLAGS_TF   0x0100
#define X86_FLAGS_IF   0x0200
#define X86_FLAGS_DF   0x0400
#define X86_FLAGS_OV   0x0800
#define X86_FLAGS_IOPL 0x3000
#define X86_FLAGS_NT   0x4000
#define X86_FLAGS_MD   0x8000

static inline u8 inb(u16 port) {
  u8 result;
  asm_volatile("in al, dx" : "=a" (result) : "d" (port));
  return result;
}

static inline u16 inw(u16 port) {
  u16 result;
  asm_volatile("in ax, dx" : "=a" (result) : "d" (port));
  return result;
}

static inline u32 ind(u16 port) {
  u32 result;
  asm_volatile("in eax, dx" : "=a" (result) : "d" (port));
  return result;
}

static inline void outb(u16 port, u8 data) {
  asm_volatile("out dx, al" : : "a" (data), "d" (port));
}

static inline void outw(u16 port, u16 data) {
  asm_volatile("out dx, ax" : : "a" (data), "d" (port));
}

static inline void outd(u16 port, u32 data) {
  asm_volatile("out dx, eax" : : "a" (data), "d" (port));
}

static inline void io_delay(void) {
  const u16 delay_port = 0x80;
  outb(delay_port, 0);
}

static inline void cli(void) {
  asm_volatile("cli");
}

static inline void sti(void) {
  asm_volatile("sti");
}

static inline void set_fs(u16 seg) {
  asm_volatile("mov fs, ax;" : : "a" (seg));
}

static inline void set_gs(u16 seg) {
  asm_volatile("mov gs, ax;" : : "a" (seg));
}

static inline u16 get_fs(void) {
  u16 result;
  asm_volatile("mov ax, fs;" : "=a" (result));
  return result;
}

static inline u16 get_gs(void) {
  u16 result;
  asm_volatile("mov ax, gs;" : "=a" (result));
  return result;
}

static inline u32 get_eflags(void) {
  u32 result;
  asm_volatile("pushfd;"
               "pop eax;"
               : "=a" (result)
               : /* no input */
               : /* no clobber */);
  return result;
}

static inline void halt(void) {
  asm_volatile("hlt");
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

#define assert(x) \
  do { if (!(x)) panic("assert failed: %s\n", #x); } while (0)

#define panic(...) _panic(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)

/* panic.c */
NORETURN void _panic(const char *func,
                     const char *file,
                     int line,
                     const char *fmt,
                     ...);

/* biosint.asm */
void biosint(u8 no_int, const struct regs *in, struct regs *out);

/* regs.c */
void regsinit(struct regs *r);

/* string.c */
void *memset(void *src, unsigned char c, unsigned n);

void *memcpy(void *dst, void *src, unsigned n);

/* basic_printf.c */
void putchar(char c);
void puts(const char *s);
void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list ap);

/* memory */
void detect_memory_e820(void);

/* loader.c */
void *load_kernel(void);

/* serial.c */
void early_init_serial(void);
void serial_putchar(u8 ch);

/* a20.c */
void enable_a20(void);
