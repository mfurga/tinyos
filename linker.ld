/*
 * Linker script for the kernel.
 */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(entry32)

SECTIONS
{
  . = 0x100000;

  __KERNEL_START = .;

  .text :
  {
    *(.entry)
    *(.text)
  }

  . = ALIGN(0x10);
  .rodata : { *(.rodata) }

  . = ALIGN(0x10);
  .data : { *(.data) }

  . = ALIGN(0x10);
  .bss :
  {
    __BSS_START = .;
    *(.bss)
    __BSS_END = .;
  }

  /DISCARD/ :
  {
    *(.note*)
    *(.comment)
  }

  __KERNEL_END = .;
}

