#include <tinyos/common/common.h>
#include <tinyos/common/string.h>
#include <tinyos/common/utils.h>

static u8 memory[2 * 1024 * 1024] ALIGNED(4096);  /* 2 MiB */
static size_t ptr = 0;

void *kmalloc(size_t sz) {
  if (is_power_of_2(sz) && ptr % sz != 0) {
    ptr += ptr - (ptr % sz);
  }

  if (unlikely(ptr + sz > ARRAY_SIZE(memory))) {
    return NULL;
  }

  void *p = &memory[ptr];
  ptr += sz;
  return p;
}

void *kzmalloc(size_t sz) {
  void *p = kmalloc(sz);
  if (unlikely(p == NULL)) {
    return NULL;
  }
  bzero(p, sz);
  return p;
}

void kfree(void *p) {
  UNUSED(p);
  /* Do nothing */
}
