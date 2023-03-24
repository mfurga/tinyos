#pragma once

#include <kernel/common.h>

void *heap_alloc(u32 sz);

void heap_free(void *p);

