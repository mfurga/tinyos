#include <kernel/heap.h>

/* Dirty alloc */

#define HEAP_SIZE 1

static u8 heap[HEAP_SIZE];  /* goes to BSS section */
static u32 heap_ptr = 0;

void *heap_alloc(u32 sz) {
  if (heap_ptr + sz >= sizeof(heap)) {
    return NULL;
  }
  void *p = (void *)((u32)&heap + heap_ptr);
  heap_ptr += sz;
  return p;
}

void heap_free(void *p) {
  /* Do nothing. */
  UNUSED(p);
  return;
}

