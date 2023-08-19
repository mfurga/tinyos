#pragma once

#include <tinyos/common/common.h>
#include <multiboot.h>

void init_memory_map(multiboot_memory_map_t *mmap, u32 mmap_length);

void dump_memory_map(void);
