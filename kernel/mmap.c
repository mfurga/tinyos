#include <tinyos/common/common.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/mmap.h>
#include <tinyos/kernel/printk.h>

static struct mem_region mem_regions[128];
static size_t mem_region_idx = 0;

static const char *mem_region_type_to_str[] = {
  [MEMORY_AVAIABLE] = "AVAIABLE",
  [MEMORY_RESERVED] = "RESERVED",
  [MEMORY_ACPI_RECLAIMABLE] = "ACPI",
  [MEMORY_NVS] = "NVS",
  [MEMORY_BADRAM] = "BADRAM"
};

void mem_add_region(struct mem_region region) {
  if (mem_region_idx >= ARRAY_SIZE(mem_regions)) {
    panic("Not enought space to add memory region (max: %u)\n",
      ARRAY_SIZE(mem_regions));
  }
  mem_regions[mem_region_idx++] = region;
}

static void mem_add_region_kernel(void) {
  extern int __KERNEL_START_PADDR;
  extern int __KERNEL_END_PADDR;

  mem_add_region((struct mem_region) {
    .addr = (u32)&__KERNEL_START_PADDR,
    .len = (u32)&__KERNEL_END_PADDR - (u32)&__KERNEL_START_PADDR + 1,
    .type = MEMORY_RESERVED
  });
}

void init_memory_map(struct multiboot_info *mbi) {
  multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mbi->mmap_addr;
  u32 mmap_length = mbi->mmap_length;

  assert(mmap_length % sizeof(multiboot_memory_map_t) == 0);
  size_t len = mmap_length / sizeof(multiboot_memory_map_t);

  for (size_t i = 0; i < len; i++) {
    mem_add_region((struct mem_region) {
      .addr = mmap[i].addr,
      .len = mmap[i].len,
      .type = mmap[i].type
    });
  }

  mem_add_region_kernel();
}

void dump_memory_map(void) {
  printk("NO  START                END                TYPE\n");
  printk("------------------------------------------------\n");

  for (size_t i = 0; i < mem_region_idx; i++) {
    printk("%02d: 0x%016llx - 0x%016llx %s\n",
           i,
           mem_regions[i].addr,
           mem_regions[i].addr + mem_regions[i].len - 1,
           mem_region_type_to_str[mem_regions[i].type]);
  }
}

