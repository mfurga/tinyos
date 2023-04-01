#pragma once

#include <kernel/boot/boot_params.h>

#define PMM_MEMORY_SIZE       (128 * 1024 * 1024)  /* 128MiB */

#define PMM_BLOCK_SIZE        4096
#define PMM_BLOCK_MASK        0xfff
#define PMM_BLOCK_COUNT       (PMM_MEMORY_SIZE / PMM_BLOCK_SIZE)

void pmm_init(const memory_map_entry_t *map, u32 size);

void *pmm_alloc(void);

void pmm_free(void *p);

