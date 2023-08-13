/*
 * Linker script for the kernel.
 */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

__KERNEL_PADDR = 0x100000;

SECTIONS
{
  . = __KERNEL_PADDR;

  .text :
  {
    *(.text.start)
    *(.text.startup .text.startup.*)
    *(.text)
  }

  .ctors ALIGN(0x10) :
  {
    __CTORS_START_PADDR = .;
    *(.ctors .ctors.*)
    __CTORS_END_PADDR = .;
  }

  .rodata ALIGN(0x10) :
  {
    *(.rodata .rodata.*)
  }

  .data ALIGN(0x10) :
  {
    *(.data .data.*)
  }

  .bss ALIGN(0x10) :
  {
    __BSS_START_PADDR = .;
    *(.bss)
    __BSS_END_PADDR = .;
  }

  /DISCARD/ :
  {
    *(.note*)
    *(.comment*)
    *(.eh_frame*)
  }

  __KERNEL_END = .;
}

