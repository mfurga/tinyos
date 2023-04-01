/*
 * Physical Memory Manager.
 */

#include <kernel/memory/pmm.h>
#include <kernel/panic.h>
#include <kernel/string.h>

#define BLOCK_INFO_SIZE   (PMM_BLOCK_COUNT / 32)

extern u32 __KERNEL_END;
static u32 memory_start;
static u32 block_info[BLOCK_INFO_SIZE];

static u32 find_free_block(void) {
  for (u32 i = 0; i < BLOCK_INFO_SIZE; i++) {
    if (block_info[i] != 0xffffffff) {
      u32 j = 0;
      while ((block_info[i] & (1 << j)) == 1) {
        j++;
      }
      return i * 32 + j;
    }
  }
  return -1U;
}

static void mark_block(u32 offset) {
  block_info[offset / 32] |= (1 << (offset % 32));
}

static void unmark_block(u32 offset) {
  block_info[offset / 32] &= ~(1 << (offset % 32));
}

void pmm_init(const memory_map_entry_t *map, u32 size) {
  u32 kernel_end = (u32)&__KERNEL_END - 1;
  u32 block_start = 0, block_end;

  for (u32 i = 0; i < size; i++) {
    if (map[i].type == MEMORY_MAP_TYPE_AVAILABLE &&
        map[i].base <= kernel_end &&
        map[i].base + map[i].length >= kernel_end) {
      block_start = kernel_end;
      block_end = map[i].base + map[i].length - 1;
      break;
    }
  }

  if (!block_start) {
    panic("Memory block not found.\n");
  }

  if ((block_start & PMM_BLOCK_MASK) != 0) {
    block_start = (block_start & ~PMM_BLOCK_MASK) + PMM_BLOCK_SIZE;
  }

  if ((block_end & PMM_BLOCK_MASK) != PMM_BLOCK_MASK) {
    block_end = (block_end | PMM_BLOCK_SIZE) - PMM_BLOCK_SIZE;
  }

  u32 sz = block_end - block_start + 1;
  if (sz < PMM_MEMORY_SIZE) {
    panic("Not enough memory!\n"
          "Detected %u MiB. Required %u MiB\n",
          sz / (1024 * 1024),
          PMM_MEMORY_SIZE / (1024 * 1024));
  }

  memory_start = block_start;
  memset(block_info, 0, sizeof(block_info));
}

void *pmm_alloc(void) {
  u32 offset = find_free_block();
  if (offset == -1U) {
    return NULL;
  }
  mark_block(offset);
  return (void *)(memory_start + PMM_BLOCK_SIZE * offset);
}

void pmm_free(void *p) {
  u32 offset = ((u32)p - memory_start) / PMM_BLOCK_SIZE;
  unmark_block(offset);
}

/*
static u32 find_free_blocks(u32 size) {
  u32 i = 0, k, c;

  while (i < BLOCK_INFO_SIZE) {
    if (block_info[i] != 0xffffffff) {
      c = 0;

      // Find first free
      k = 0;
      while (k < 32 && (block_info[i] & (1 << k)) == 1) {
        k++;
      }

      // Check if size is ok
      do {
        c++;
        if (c == size) {
          return 32 * i + k;
        }
        if (++k == 32) {
          k = 0;
          i++;
        }
      } while (i < BLOCK_INFO_SIZE && (block_info[i] & (1 << k)) == 0);
    }
    i++;
  }

  return -1U;
}
*/

