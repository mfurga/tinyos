#include <tinyos/common/string.h>

unsigned strlen(const char *s) {
  unsigned l;
  for (l = 0; *s != '\0'; s++, l++)
    ;
  return l;
}

void *memset(void *s, int c, size_t n) {
  u8 *p = s;
  for (size_t i = 0; i < n; i++) {
    *p++ = (u8)c;
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
