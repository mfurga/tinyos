#include <kernel/memory/malloc.h>
#include <kernel/memory/pmm.h>
#include <lib/common.h>

#define HEAP_MAX_SIZE 1024 * 1024 * 4  /* 4 MB */

static u8 buf[HEAP_MAX_SIZE];
static unsigned buf_ptr = 0;

void *kmalloc(unsigned size) {
  if (buf_ptr + size >= sizeof(buf)) {
    return NULL;
  }
  void *p = &buf + buf_ptr;
  buf_ptr += size;
  return p;
}

void kfree(const void *ptr) {
  UNUSED(ptr);
}

