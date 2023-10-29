/*
 * Entry point of 32-bit kernel.
 *
 * Kernel supports Multiboot 1.0 standard:
 * https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
 */

.intel_syntax noprefix

#define ASM_FILE 1
#include <tinyos/common/config.h>
#include <multiboot.h>

#define VADDR_TO_PADDR(vaddr) ((vaddr) - KERNEL_VADDR + KERNEL_PADDR)
#define VADDR_TO_PDIR(vaddr) ((vaddr) >> 20)
#define MAKE_DIR_ENTRY_4MB(paddr) ((1 << 0) /* present */ | \
                                   (1 << 1) /* rw */ | \
                                   (1 << 7) /* ps */ | \
                                   ((paddr) & 0xffc00000) /* paddr */)

.code32

.section .text.start
.global _start
_start:
  jmp multiboot_entry

#define MUTIBOOT_FLAGS    (MULTIBOOT_PAGE_ALIGN |  \
                           MULTIBOOT_MEMORY_INFO | \
                           MULTIBOOT_VIDEO_MODE)

/* Multiboot 1.0 header */
.align MULTIBOOT_HEADER_ALIGN

  .long MULTIBOOT_HEADER_MAGIC                      /* magic */
  .long MUTIBOOT_FLAGS                              /* flags */
  .long -(MULTIBOOT_HEADER_MAGIC + MUTIBOOT_FLAGS)  /* checksum */
  .long 0                                           /* header_addr */
  .long 0                                           /* load_addr */
  .long 0                                           /* load_end_addr */
  .long 0                                           /* bss_end_addr */
  .long 0                                           /* entry_addr */
  .long 0                                           /* mode_type */
  .long 0                                           /* width */
  .long 0                                           /* height */
  .long 32                                          /* color depth */

multiboot_entry:
  cli
  cld

  mov edi, VADDR_TO_PADDR(offset page_dir_4mb)
  mov dword ptr [edi + VADDR_TO_PDIR(0)], MAKE_DIR_ENTRY_4MB(0)
  mov dword ptr [edi + VADDR_TO_PDIR(KERNEL_VADDR)], MAKE_DIR_ENTRY_4MB(0)

  /* Set address of page directory in CR3 */
  mov edx, cr3
  or edx, edi
  mov cr3, edx

  /* Enable PSE (Page Size Extension) */
  mov edx, cr4
  or edx, 0x00000010
  mov cr4, edx

  /* Enable paging */
  mov edx, cr0
  or edx, (1 << 31)
  mov cr0, edx

  mov ecx, offset .jmp_virtual_addr
  jmp ecx

.jmp_virtual_addr:
  mov esp, offset kernel_init_stack + KERNEL_INIT_STACK_SZ - 4

  /* Push Multiboot information structure. */
  push ebx

  /* Push magic value. */
  push eax

  /* Call C kernel_main function. */
  call kernel_main

  /* Never go here! */
  jmp $

.section .bss

.comm page_dir_4mb, 4096 /* size */, 4096 /* alignment */ 
.comm kernel_init_stack, KERNEL_INIT_STACK_SZ
