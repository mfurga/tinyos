#include <tinyos/common/string.h>

size_t strlen(const char *s) {
  size_t l;
  for (l = 0; *s != '\0'; s++, l++)
    ;
  return l;
}

void *memset(void *s, u8 v, size_t n) {
  asm_volatile("rep stosb;"
               : /* no output */
               : "c" (n), "D" (s), "a" (v)
               : "memory", "cc");
  return s;
}

void *memset16(void *s, u16 v, size_t n) {
  asm_volatile("rep stosw;"
               : /* no output */
               : "c" (n), "D" (s), "a" (v)
               : "memory", "cc");
  return s;
}

void *memset32(void *s, u32 v, size_t n) {
  asm_volatile("rep stosd;"
               : /* no output */
               : "c" (n), "D" (s), "a" (v)
               : "memory", "cc");
  return s;
}

void *memcpy(void *dst, const void *src, size_t n) {
  asm_volatile("rep movsb;"
               : /* no output */
               : "c" (n), "D" (dst), "S" (src)
               : "memory", "cc");
  return dst;
}

void *memcpy16(void *dst, const void *src, size_t n) {
  asm_volatile("rep movsw;"
               : /* no output */
               : "c" (n), "D" (dst), "S" (src)
               : "memory", "cc");
  return dst;
}

void *memcpy32(void *dst, const void *src, size_t n) {
  asm_volatile("rep movsd;"
               : /* no output */
               : "c" (n), "D" (dst), "S" (src)
               : "memory", "cc");
  return dst;
}
