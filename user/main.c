/* User mode program. */

void user_main(void) {

  __asm__ __volatile__(
    "int3;"
  );

  for (;;);
}

