#include <tinyos/common/common.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/mmap.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/kernel/sort.h>

static struct mem_region mem_regions[128];
static size_t mem_region_count = 0;

static const char *mem_region_type_to_str[] = {
  [MEMORY_AVAIABLE] = "AVAIABLE",
  [MEMORY_RESERVED] = "RESERVED",
  [MEMORY_ACPI_RECLAIMABLE] = "ACPI",
  [MEMORY_NVS] = "NVS",
  [MEMORY_BADRAM] = "BADRAM"
};

void append_mem_region(struct mem_region region) {
  if (mem_region_count >= ARRAY_SIZE(mem_regions)) {
    panic("Not enought space to add memory region (max: %u)\n",
      ARRAY_SIZE(mem_regions));
  }
  mem_regions[mem_region_count++] = region;
}

static int sort_cmp_func(const void *a, const void *b) {
  const struct mem_region *r1 = a;
  const struct mem_region *r2 = b;

  if (r1->addr > r2->addr) {
    return 1;
  }

  if (r1->addr < r2->addr) {
    return -1;
  }

  /* equal */

  if (r1->len < r2->len) {
    return 1;
  }

  if (r1->len > r2->len) {
    return -1;
  }

  return 0;
}

static void sort_mem_regions(void) {
  insertion_sort(&mem_regions,
                 sizeof(struct mem_region),
                 mem_region_count,
                 &sort_cmp_func);
}

static void fix_mem_regions(void) {
  /* Assumes that memory regions are sorted */

  for (size_t i = 0; i < mem_region_count; i++) {
    for (size_t j = i + 1; j < mem_region_count; j++) {
      struct mem_region *r1 = &mem_regions[i];
      struct mem_region *r2 = &mem_regions[j];

      u64 r1_end = r1->addr + r1->len - 1;
      u64 r2_begin = r2->addr;

      if (r1_end < r2_begin) {
        continue;
      }

      if (r1->type == MEMORY_AVAIABLE && r2->type == MEMORY_RESERVED) {
        if (r1->addr == r2->addr) {
          r1->addr = r2->addr + r2->len;
        }
      }

      // TODO ...
    }
  }
}

static void append_mem_region_kernel(void) {
  // extern int __KERNEL_START_PADDR;
  // extern int __KERNEL_END_PADDR;

  // append_mem_region((struct mem_region) {
  //   .addr = (u32)&__KERNEL_START_PADDR,
  //   .len = (u32)&__KERNEL_END_PADDR - (u32)&__KERNEL_START_PADDR + 1,
  //   .type = MEMORY_RESERVED
  // });
}

void init_memory_map(struct multiboot_info *mbi) {
  multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mbi->mmap_addr;
  u32 mmap_length = mbi->mmap_length;

  assert(mmap_length % sizeof(multiboot_memory_map_t) == 0);
  size_t len = mmap_length / sizeof(multiboot_memory_map_t);

  for (size_t i = 0; i < len; i++) {
    append_mem_region((struct mem_region) {
      .addr = mmap[i].addr,
      .len = mmap[i].len,
      .type = mmap[i].type
    });
  }

  append_mem_region_kernel();

  sort_mem_regions();
  fix_mem_regions();
  sort_mem_regions();
}

void dump_memory_map(void) {
  printk("NO  START                END                TYPE\n");
  printk("------------------------------------------------\n");

  for (size_t i = 0; i < mem_region_count; i++) {
    printk("%02d: 0x%016llx - 0x%016llx %s\n",
           i,
           mem_regions[i].addr,
           mem_regions[i].addr + mem_regions[i].len - 1,
           mem_region_type_to_str[mem_regions[i].type]);
  }
}
