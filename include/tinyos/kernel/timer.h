#pragma once

#include <tinyos/common/common.h>

#define TIMER_SCALE         1000000000  /* 1 bilion = 1e9 */

u64 hw_timer_init(u32 freq);

void init_timer(void);

u64 timer_get_time(void);

u64 timer_get_ticks(void);

