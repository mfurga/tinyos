#include "common.h"

void regsinit(struct regs *r) {
  memset(r, 0, sizeof(*r));
  r->eflags = get_eflags();
  r->fs = get_fs();
  r->gs = get_gs();
}

