/*
 * Linker script for the bootloader.
 */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
  . = 0x500;

  .text :
  {
    *(.entry)
    *(.text*)
  }

  . = ALIGN(0x10);
  .rodata : { *(.rodata*) }

  . = ALIGN(0x10);
  .data : { *(.data) }

  . = ALIGN(0x10);
  .bss :
  {
    __bss_start = .;
    *(.bss)
    __bss_end = .;
  }

  /DISCARD/ :
  {
    *(.note*)
    *(.comment)
  }

  __end = .;

  . = ASSERT(__end <= 0xffff, "Bootloader too big!");
}

