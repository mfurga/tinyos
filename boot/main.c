/*
  Kernel 16-bit (real mode) entry point.
 */

#include "common.h"

#define NULL ((void *)0)

void __attribute__((noreturn)) main(void) {

  struct biosreg in;
  struct biosreg out;

  in.ah = 0;
  in.al = 3;
  biosint(0x10, &in, &out);

  in.ah = 0xe;
  in.al = 'A';
  biosint(0x10, &in, &out);

  for (;;);
}

