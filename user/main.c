/* User mode program. */

void user_main(void) {

  __asm__ __volatile__(
    "int 0x80;"
  );

  for (;;);
}

