#pragma once

#include <tinyos/common/common.h>

unsigned strlen(const char *s);

void *memset(void *s, int c, size_t n);

void *memcpy(void *dst, const void *src, size_t n);
