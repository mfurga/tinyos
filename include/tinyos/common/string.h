#pragma once

#include <tinyos/common/common.h>

size_t strlen(const char *s);

void *memset(u8 *s, u8 v, size_t n);
void *memset16(u16 *s, u16 v, size_t n);
void *memset32(u32 *s, u32 v, size_t n);

void *memcpy(void *dst, const void *src, size_t n);
void *memcpy16(void *dst, const void *src, size_t n);
void *memcpy32(void *dst, const void *src, size_t n);
