/*
 * Linker script for the kernel.
 */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

__KERNEL_BASE_VADDR = 0xc0000000;
__KERNEL_PADDR = 0x100000;
__KERNEL_VADDR = __KERNEL_BASE_VADDR + __KERNEL_PADDR;

__KERNEL_TEXT_OFFSET = 0x1000;
__KERNEL_TEXT_PADDR = __KERNEL_PADDR + __KERNEL_TEXT_OFFSET;
__KERNEL_TEXT_VADDR = __KERNEL_VADDR + __KERNEL_TEXT_OFFSET;

PHDRS
{
  ro_segment PT_LOAD FILEHDR PHDRS;
  rw_segment PT_LOAD;  
}

SECTIONS
{
  . = __KERNEL_TEXT_VADDR;

  .text : AT(__KERNEL_TEXT_PADDR)
  {
    *(.text.start)
    *(.text.startup .text.startup.*)
    *(.text)
  } : ro_segment

  .ctors ALIGN(0x10) :
  {
    __CTORS_START_PADDR = .;
    *(.ctors .ctors.*)
    __CTORS_END_PADDR = .;
  } : ro_segment

  .rodata ALIGN(0x10) :
  {
    *(.rodata .rodata.*)
  } : ro_segment

  .data ALIGN(0x10) :
  {
    *(.data .data.*)
  } : rw_segment

  .bss ALIGN(0x10) :
  {
    __BSS_START_PADDR = .;
    *(.bss)
    __BSS_END_PADDR = .;
  } : rw_segment

  /DISCARD/ :
  {
    *(.note*)
    *(.comment*)
    *(.eh_frame*)
  }
}
