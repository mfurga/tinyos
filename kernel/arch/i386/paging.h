#pragma once

#include <tinyos/common/common.h>

#define PAGE_TAB_SIZE       1024
#define PAGE_DIR_SIZE       1024

#define PAGE_FAULT_PRESENT  (1 << 0)
#define PAGE_FAULT_RW       (1 << 1)
#define PAGE_FAULT_US       (1 << 2)

#define PAGE_DIR_SHIFT      22
#define PAGE_DIR_MASK       (1024 - 1)
#define PAGE_TAB_SHIFT      12
#define PAGE_TAB_MASK       (1024 - 1)

#define PAGE_ENTRY_PRESENT  (1 << 0)
#define PAGE_ENTRY_RW       (1 << 1)

#define IS_PAGE_TAB_ALIGNED(addr) ((((u32)addr) & 0xfff) == 0)

union page_tab_entry {
  struct {
    u32 present : 1;
    u32 rw : 1;         /* 0 = read only, 1 = read/write */
    u32 us : 1;         /* user / supiervisor */
    u32 wt : 1;         /* write-through */
    u32 cd : 1;         /* cache disable */
    u32 accessed : 1;
    u32 dirty : 1;
    u32 pat : 1;
    u32 global : 1;
    u32 _ignored2 : 3;
    u32 paddr : 20;
  };

  u32 raw;
};

union page_dir_entry {
  struct {
    u32 present : 1;
    u32 rw : 1;         /* 0 = read only, 1 = read/write */
    u32 us : 1;         /* user / supiervisor */
    u32 wt : 1;         /* write-through */
    u32 cd : 1;         /* cache disable */
    u32 accessed : 1;
    u32 _ignored1 : 1;
    u32 ps : 1;         /* 0 = 4 KiB */
    u32 _ignored2 : 4;
    u32 ptaddr : 20;
  };

  struct {
    u32 present : 1;
    u32 rw : 1;         /* 0 = read only, 1 = read/write */
    u32 us : 1;         /* user / supiervisor */
    u32 wt : 1;         /* write-through */
    u32 cd : 1;         /* cache disable */
    u32 accessed : 1;
    u32 dirty : 1;
    u32 ps : 1;         /* 1 = 4 MiB */
    u32 global : 1;
    u32 _ignored2 : 3;
    u32 pat : 1;
    u32 _reserved : 9;
    u32 paddr : 10;
  } big;

  u32 raw;
};

typedef struct page_dir {
  union page_dir_entry entries[1024];
} page_dir_t;

typedef struct page_tab {
  union page_tab_entry entries[1024];
} page_tab_t;

STATIC_ASSERT(sizeof(page_dir_t) == 4096);
STATIC_ASSERT(sizeof(page_tab_t) == 4096);
