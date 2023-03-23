#pragma once

#include <kernel/common.h>

#define MMAP_MAX_LENGTH 128

typedef struct {
  u64 base;
  u64 length;
  u32 type;
} PACKED mmap_entry_t;

typedef struct boot_params_s {
  struct {
    u8 mode;
    u8 page;
    u8 x;
    u8 y;
    u8 cols;
    u8 lines;
  } video;

  mmap_entry_t mmap[MMAP_MAX_LENGTH];
  u16 mmap_length;

} PACKED boot_params_t;

