#pragma once

#include <tinyos/common/common.h>

// https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
static inline u32 roundup_next_power_of_2(u32 v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

// https://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
static inline bool is_power_of_2(u32 v) {
  return v && !(v && (v - 1));
}
