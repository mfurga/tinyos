#pragma once

#include <tinyos/common/common.h>
#include <multiboot.h>

enum mem_region_type {
  MEMORY_AVAIABLE = 1,
  MEMORY_RESERVED,
  MEMORY_ACPI_RECLAIMABLE,
  MEMORY_NVS,
  MEMORY_BADRAM
};

struct mem_region {
  u64 addr;
  u64 len;
  enum mem_region_type type;
};

void append_mem_region(struct mem_region region);

void init_memory_map(struct multiboot_info *mbi);

void dump_memory_map(void);
