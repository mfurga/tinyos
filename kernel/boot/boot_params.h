#pragma once

#include <kernel/common.h>

#define MEMORY_MAP_MAX_SIZE        128

#define MEMORY_MAP_TYPE_AVAILABLE  1
#define MEMORY_MAP_TYPE_RESERVED   2
#define MEMORY_MAP_TYPE_ACPI       3
#define MEMORY_MAP_TYPE_NVS        4

typedef struct {
  u64 base;
  u64 length;
  u32 type;
} PACKED memory_map_entry_t;

typedef struct boot_params_s {
  struct {
    u8 mode;
    u8 page;
    u8 x;
    u8 y;
    u8 cols;
    u8 lines;
  } video;

  memory_map_entry_t memory_map[MEMORY_MAP_MAX_SIZE];
  u16 memory_map_size;

} PACKED boot_params_t;

