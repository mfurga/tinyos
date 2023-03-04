#include "common.h"

extern boot_params_t boot_params;

#define SMAP 0x534d4150  /* "SMAP" */

void detect_memory_e820(void) {
  struct regs in, out;
  regsinit(&in);

  memory_entry_t *dst = boot_params.memory_map;
  memory_entry_t buf;
  u16 count = 0;

  in.eax = 0xe820;
  in.edx = SMAP;
  in.ebx = 0;
  in.ecx = sizeof(buf);
  in.di = (u32)&buf;

  do {
    biosint(0x15, &in, &out);
    in.ebx = out.ebx;

    if (out.flags & FLAG_CF) {
      /* error */
      FATAL("Failed to get memory map.");
    }

    if (out.edx != SMAP) {
      /* error */
      FATAL("Failed to get memory map.");
    }

    count++;
    *dst++ = buf;
  } while (in.ebx != 0 && count < sizeof(boot_params.memory_map));

  boot_params.memory_entries = count;
  OK("Memory map detected.");
}

