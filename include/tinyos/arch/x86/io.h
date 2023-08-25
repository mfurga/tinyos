#pragma once

#if !defined(__i386__) && !defined(__x86_64__)
  #error The header can be used only for x86 architectures.
#endif

#include <tinyos/common/common.h>

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
