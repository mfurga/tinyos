#include "common.h"
#include "multiboot.h"

#define SMAP 0x534d4150  /* "SMAP" */
#define MAX_MEM_ENTRIES 32

extern struct multiboot_info mbi;
static struct multiboot_mmap_entry entries[MAX_MEM_ENTRIES];

struct e820_entry {
  u64 addr;
  u64 len;
  u32 type;
} PACKED;

void detect_memory_e820(void) {
  struct regs in, out;
  regsinit(&in);

  struct e820_entry entry;

  u16 count = 0;

  in.eax = 0xe820;
  in.edx = SMAP;
  in.ebx = 0;
  in.ecx = sizeof(entry);
  in.di = (u32)&entry;

  do {
    biosint(0x15, &in, &out);
    in.ebx = out.ebx;

    if (out.flags & X86_FLAGS_CF) {
      panic("Failed to get the memory map");
    }

    if (out.edx != SMAP) {
      panic("Failed to get the memory map");
    }

    entries[count].addr = entry.addr;
    entries[count].len = entry.len;
    entries[count].type = entry.type;
    count++;
  
  } while (in.ebx != 0 && count < MAX_MEM_ENTRIES);

  /* Save into multiboot info */
  mbi.flags |= MULTIBOOT_INFO_MEM_MAP;
  mbi.mmap_addr = (u32)&entries;
  mbi.mmap_length = count * sizeof(struct multiboot_mmap_entry);
}
