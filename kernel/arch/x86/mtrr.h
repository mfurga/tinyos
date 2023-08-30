#pragma once

#include <tinyos/common/common.h>

#define MEM_TYPE_UC 0
#define MEM_TYPE_WC 1
#define MEM_TYPE_WT 4
#define MEM_TYPE_WP 5
#define MME_TYPE_WB 6

bool is_mtrr_supported(void);

int find_free_mtrr(void);

void set_mtrr(u32 no, u64 paddr, u32 size, u8 mem_type);
