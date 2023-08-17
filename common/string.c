#include <tinyos/common/string.h>

size_t strlen(const char *s) {
  size_t l;
  for (l = 0; *s != '\0'; s++, l++)
    ;
  return l;
}

void *memset(u8 *s, u8 v, size_t n) {
  for (size_t i = 0; i < n; i++) {
    *s++ = v;
  }
  return s;
}

void *memset16(u16 *s, u16 v, size_t n) {
  for (size_t i = 0; i < n; i++) {
    *s++ = v;
  }
  return s;
}

void *memset32(u32 *s, u32 v, size_t n) {
  for (size_t i = 0; i < n; i++) {
    *s++ = v;
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
