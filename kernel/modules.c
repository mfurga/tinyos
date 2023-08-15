#include <tinyos/common/common.h>
#include <tinyos/kernel/module.h>

static const struct module *modules[64];
static unsigned module_idx = 0;

void register_module(const struct module *m) {
  ASSERT(module_idx < ARRAY_SIZE(modules));
  modules[module_idx++] = m;
}

void init_modules(void) {
  /* TODO: sort modules by their priority */

  for (unsigned i = 0; i < module_idx; i++) {
    modules[i]->init();
  }
}