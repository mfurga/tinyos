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
#define PH_FILESZ 0x10

#define PT_LOAD 1

extern boot_params_t boot_params;

extern u8 BOOTLOADER_SIZE;
extern u8 DRIVE_NUMBER;

static u32 sector_no;
static u32 load_addr;

static void read_sectors(u32 no) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.ah = 2;                // Read sectors into memeory
  in.al = no;               // Number of sectors
  in.ch = 0;                // Cylinder number
  in.dh = 0;                // Head number
  in.cl = (u8)sector_no;    // Sector number
  in.dl = DRIVE_NUMBER;     // Drive number
  in.bx = (u16)load_addr;   // ES:BX destination address (ES = 0)

  biosint(0x13, &in, &out);

  if (out.flags & FLAG_CF) {
    FATAL("Failed to read kernel into memory.");
  }

  sector_no += no;
  load_addr += 512 * no;

  if (sector_no > 63) {
    FATAL("Sector number too big!");
  }
}

static void check_addr(u32 addr) {
  if (addr >= load_addr) {
    read_sectors((addr - load_addr + 511) / 512);
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

void NORETURN load_kernel(void) {
  u32 base_addr = BOOTLOADER_BASE + 512 * BOOTLOADER_SIZE;

  //INFO("Reading kernel to %x\n", base_addr);

  sector_no = BOOTLOADER_SIZE + 1;
  load_addr = base_addr;

  u16 phsize = fetch_word(base_addr + ELF_PHENTSIZE);
  u16 phnum = fetch_word(base_addr + ELF_PHNUM);
  u32 ph_addr = base_addr + fetch_dword(base_addr + ELF_PHOFF);

  while (phnum-- > 0) {
    u32 ph_type = fetch_dword(ph_addr + PH_TYPE);

    if (ph_type == PT_LOAD) {
      u32 ph_filesz = fetch_dword(ph_addr + PH_FILESZ);
      u32 ph_offset = base_addr + fetch_dword(ph_addr + PH_OFFSET);
      u32 ph_vaddr = fetch_dword(ph_addr + PH_VADDR);

      check_addr(ph_offset + ph_filesz);
      memcpy((void *)ph_vaddr, (void *)ph_offset, ph_filesz);
    }

    ph_addr += phsize;
  }

  u32 entry_point = fetch_dword(base_addr + ELF_ENTRY);

  // Save current video state.
  store_video();

  // Jump to kernel entry point.
  __asm__ __volatile__(
    "mov esi, ebx;"
    "jmp eax;"
    :
    : "a" (entry_point), "b" (&boot_params)
  );

  // So dirty ...
  //((void (*)(void))entry_point)();

  // Never goes here, hopefully.
  for (;;);
}

