#include "common.h"

unsigned str_to_uint(const char *s) {
  unsigned n = 0;
  while (*s != '\0') {
    if (*s < '0' || *s > '9') {
      break;
    }
    n *= 10;
    n += *s - '0';
    s++;
  }
  return n;
}

size_t strlen(const char *s) {
  size_t l;
  for (l = 0; *s != '\0'; s++, l++)
    ;
  return l;
}

void *memset(void *s, u8 v, size_t n) {
  u8 *p = s;
  for (size_t i = 0; i < n; i++) {
    *p++ = v;
  }
  return s;
}

void *memset16(void *s, u16 v, size_t n) {
  u16 *p = s;
  for (size_t i = 0; i < n; i++) {
    *p++ = v;
  }
  return s;
}

void *memset32(void *s, u32 v, size_t n) {
  u32 *p = s;
  for (size_t i = 0; i < n; i++) {
    *p++ = v;
  }
  return s;
}

void *memcpy(void *dst, const void *src, size_t n) {
  u8 *d = dst;
  const u8 *s = src;
  for (size_t i = 0; i < n; i++) {
    *d++ = *s++;
  }
  return dst;
}

void *memcpy16(void *dst, const void *src, size_t n) {
  return memcpy(dst, src, n * 2);
}

void *memcpy32(void *dst, const void *src, size_t n) {
  return memcpy(dst, src, n * 4);
}
