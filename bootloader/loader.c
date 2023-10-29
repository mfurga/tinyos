#include "common.h"

#define BOOTLOADER_BASE 0x7c00

// ELF header consts.
#define ELF_ENTRY 0x18
#define ELF_PHOFF 0x1c
#define ELF_PHENTSIZE 0x2a
#define ELF_PHNUM 0x2c

// ELF PH consts.
#define PH_TYPE 0x00
#define PH_OFFSET 0x04
#define PH_VADDR 0x08
#define PH_PADDR 0x0c
#define PH_FILESZ 0x10
#define PH_MEMSZ 0x14

#define PT_LOAD 1

extern u8 BOOTLOADER_SIZE;
extern u8 DRIVE_NUMBER;

static u32 sector_no;
static u32 head_no;
static u32 load_addr;

static void read_sectors(u32 no) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.ah = 2;                // Read sectors into memeory
  in.al = no;               // Number of sectors
  in.ch = 0;                // Cylinder number
  in.dh = (u8)head_no;      // Head number
  in.cl = (u8)sector_no;    // Sector number
  in.dl = DRIVE_NUMBER;     // Drive number
  in.bx = (u16)load_addr;   // ES:BX destination address (ES = 0)

  biosint(0x13, &in, &out);

  if (out.flags & X86_FLAGS_CF) {
    panic("Failed to read kernel into memory");
  }

  sector_no += no;
  load_addr += 512 * no;

  // FIXME: Works only for floppy disks!
  /*
  if (sector_no > 18) {
    head_no += sector_no / 18;
    sector_no = sector_no % 18;
  }
  */

  if (sector_no > 63) {
    head_no += sector_no / 63;
    sector_no = sector_no % 63;
  }

  if (sector_no > 63) {
    panic("Sector number too big");
  }
}

static void check_addr(u32 addr) {
  if (addr >= load_addr) {
    read_sectors((addr - load_addr + 512) / 512);
  }
}

static u32 fetch_dword(u32 addr) {
  check_addr(addr);
  return *(u32 *)addr;
}

static u16 fetch_word(u32 addr) {
  check_addr(addr);
  return *(u16 *)addr;
}

void *load_kernel(void) {
  printf("Loading kernel ...\n");

  u32 base_addr = BOOTLOADER_BASE;

  sector_no = BOOTLOADER_SIZE + 1;
  head_no = 0;
  load_addr = base_addr;

  u32 entry_point = fetch_dword(base_addr + ELF_ENTRY);

  u16 phsize = fetch_word(base_addr + ELF_PHENTSIZE);
  u16 phnum = fetch_word(base_addr + ELF_PHNUM);
  u32 ph_addr = base_addr + fetch_dword(base_addr + ELF_PHOFF);

  while (phnum-- > 0) {
    u32 ph_type = fetch_dword(ph_addr + PH_TYPE);

    if (ph_type == PT_LOAD) {
      u32 ph_filesz = fetch_dword(ph_addr + PH_FILESZ);
      u32 ph_memsz = fetch_dword(ph_addr + PH_MEMSZ);
      u32 ph_offset = fetch_dword(ph_addr + PH_OFFSET);
      u32 ph_paddr = fetch_dword(ph_addr + PH_PADDR);
      u32 ph_vaddr = fetch_dword(ph_addr + PH_VADDR);

      check_addr(base_addr + ph_offset + ph_filesz);

      memset((void *)ph_paddr, 0, ph_memsz);
      memcpy((void *)ph_paddr, (void *)(base_addr + ph_offset), ph_filesz);

      if (entry_point >= ph_vaddr && entry_point <= (ph_vaddr + ph_memsz)) {
        entry_point = ph_paddr + (entry_point - ph_vaddr);
      }
    }
    ph_addr += phsize;
  }

  return (void *)entry_point;
}
