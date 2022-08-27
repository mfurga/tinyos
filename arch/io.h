#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include "types.h"

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

#endif  // KERNEL_IO_H

