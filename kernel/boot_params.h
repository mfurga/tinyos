#pragma once

#include <kernel/common.h>

typedef struct boot_params_s {
  struct {
    u8 mode;
    u8 page;
    u8 x;
    u8 y;
    u8 cols;
    u8 lines;
  } video;
} boot_params_t;



