#include <arch/ints/isr.h>
#include <arch/stdio.h>
#include <arch/types.h>

static char hexs[] = "0123456789abcdef";

CDECL void int_handler(isrparams_t params) {
  char no[3] = { hexs[params.int_no >> 4], hexs[params.int_no & 0xf], '\0' };
  char err[3] = { hexs[params.error_code >> 4], hexs[params.error_code & 0xf], '\0' };

  kprint("Interrupt ");
  kprint(no);
  kprint(" ");
  kprint(err);
  kprint("\r\n");
}
