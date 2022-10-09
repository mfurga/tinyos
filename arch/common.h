/*
 * Common headers.
 */

#ifndef ARCH_COMMON_H
#define ARCH_COMMON_H

#define NORETURN __attribute__((noreturn))
#define NOINLINE __attribute__((noinline))
#define PACKED   __attribute__((packed))
#define CDECL    __attribute__((__cdecl__))

#define NULL 0

typedef unsigned char u8;
typedef char s8;

typedef unsigned short u16;
typedef short s16;

typedef unsigned int u32;
typedef int s32;

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

static inline void outb(u16 port, u8 data) {
  __asm__ __volatile__ ("out dx, al" : : "a" (data), "d" (port));
}

static inline void outw(u16 port, u16 data) {
  __asm__ __volatile__ ("out dx, ax" : : "a" (data), "d" (port));
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
void biosint(u16 no_int, const struct regs *in, struct regs *out);

/* regs.c */
void regsinit(struct regs *r);

#endif  // ARCH_COMMON_H

