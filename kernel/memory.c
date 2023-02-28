#include <kernel/memory.h>

struct {
  u64 address;
  u64 length;
  u32 type;
  u32 acpi;
} PACKED memory_map[MEMORY_MAP_SIZE];
/*
void memory_map_init(void) {
  struct regs in;
  struct regs out;

  regsinit(&in);
  in.eax = 0xe820;
  in.edx = 0x534d4150;
  in.ebx = 0;
  in.ecx = sizeof(memory_map);
  in.edi = (u32)&memory_map;
  //in.edi = 0x5000;

  kprintf("%x\n", &memory_map);

  biosint(0x15, &in, &out);

  if ((out.eflags & 1) == 0) {
    kprintf("OK\n");
  } else {
    kprintf("NIE OK\n");
  }

  kprintf("eax: %x\n", out.eax);
  kprintf("ebx: %x\n", out.ebx);
  kprintf("ecx: %x\n", out.ecx);
}

void memory_map_print(void) {
  for (int i = 0; i < MEMORY_MAP_SIZE; i++) {
    kprintf("%x - %x : %d\n", memory_map[i].address, memory_map[i].address + memory_map[i].length, memory_map[i].type);
  }
}

*/
