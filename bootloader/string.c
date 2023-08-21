#include "common.h"

void *memset(void *src, unsigned char c, unsigned n) {
  asm_volatile(
    "mov edi, edx;"
    "rep stosb;"
    :
    : "a" (c), "c" (n), "d" (src)
    : "edi"
  );
  return src;
}

void *memcpy(void *dst, void *src, unsigned n) {
  asm_volatile(
    "mov esi, eax;"
    "mov edi, edx;"
    "rep movsb;"
    :
    : "a" (src), "c" (n), "d" (dst)
    : "edi", "esi"
  );
  return dst;
}
