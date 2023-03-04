#pragma once

#include <kernel/common.h>

#define MEMORY_MAX_ENTRIES 128

typedef struct {
  u64 addr;
  u64 length;
  u32 type;
} PACKED memory_entry_t;

typedef struct boot_params_s {
  struct {
    u8 mode;
    u8 page;
    u8 x;
    u8 y;
    u8 cols;
    u8 lines;
  } video;

  memory_entry_t memory_map[MEMORY_MAX_ENTRIES];
  u16 memory_entries;

} PACKED boot_params_t;

