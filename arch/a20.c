/*
 * Test and enable A20 line.
 */

#include "types.h"
#include "common.h"
#include "io.h"

#define _str(a) #a
#define STR(a) _str(a)

/* Int 90h */
#define A20_TEST_ADDR (4 * 0x90)
#define MAX_8042_LOOPS 10000
#define MAX_TEST_A20_LOOPS 10000

/*
static void disable_a20(void) {
  struct regs in;
  regsinit(&in);
  in.ax = 0x2400;
  biosint(0x15, &in, NULL);
}
*/

/* Returns 1 if A20 line is enabled, 0 otherwise. */
static int test_a20_single(void) {
  int r;
  __asm__ __volatile__(
    "xor eax, eax;"
    "mov esi, " STR(A20_TEST_ADDR) ";"
    "mov edi, " STR(A20_TEST_ADDR + 0x100000) ";"
    "cmpsd;"
    "jne .exit_%=;"

    /* Save value of A20_TEST_ADDR. */
    "mov edx, dword ptr [esi];"

    "mov dword ptr [esi], 0xdeadbeef;"
    "cmpsd;"

    /* Restore value of A20_TEST_ADDR. */
    "mov dword ptr [esi], edx;"

    ".exit_%=:"
    "setne al;"
    : "=a" (r)
    :
    : "edx", "esi", "edi"
  );
  return r;
}

int test_a20(void) {
  int loops = MAX_TEST_A20_LOOPS;

  while (loops--) {
    if (test_a20_single()) {
      return 1;
    }
    io_delay();
  }
  return 0;
}

static void enable_a20_bios(void) {
  struct regs in;
  regsinit(&in);
  in.ax = 0x2401;
  biosint(0x15, &in, NULL);
}

static int empty_8042(void) {
  u8 status;
  int loops = MAX_8042_LOOPS;

  while (loops--) {
    status = inb(0x64);

    if ((status & 2) == 0) {
      return 0;
    }

    if (status & 1) {
      /* Do not care. */
      (void)inb(0x60);
    }
  }

  return -1;
}

static void enable_a20_kbc(void) {
  /* TODO: cli() */

  empty_8042();
  outb(0x64, 0xd1);       /* Command write */

  empty_8042();
  outb(0x60, 0xdf);       /* A20 on */

  empty_8042();
  outb(0x64, 0xff);       /* Null command */

  empty_8042();

  /* TODO: sti() */
}

static void enable_a20_fast(void) {
  u8 port_a;

  port_a = inb(0x92);     /* Configuration port A */
  port_a |= 2;            /* Enable A20 */
  port_a &= ~1;           /* Do not reset machine */
  outb(0x92, port_a);
}

int enable_a20(void) {
  int loops = 4;

  while (loops--) {

    if (test_a20()) {
      return 0;
    }

    enable_a20_bios();
    if (test_a20()) {
      return 0;
    }

    enable_a20_kbc();
    if (test_a20()) {
      return 0;
    }

    enable_a20_fast();
    if (test_a20()) {
      return 0;
    }
  }

  return 1;
}

