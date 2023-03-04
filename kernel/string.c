#include <kernel/string.h>

void *memset(void *s, unsigned char c, unsigned n) {
  __asm__ __volatile__(
    "mov edi, edx;"
    "rep stosb;"
    :
    : "a" (c), "c" (n), "d" (s)
    : "edi"
  );
  return s;
}

void *memcpy(void *dst, void *src, unsigned n) {
  __asm__ __volatile__(
    "mov esi, eax;"
    "mov edi, edx;"
    "rep movsb;"
    :
    : "a" (src), "c" (n), "d" (dst)
    : "edi", "esi"
  );
  return dst;
}

