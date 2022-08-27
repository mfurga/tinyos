#include "common.h"
#include "string.h"

void regsinit(struct regs *r) {
  memset(r, 0, sizeof(*r));
}

