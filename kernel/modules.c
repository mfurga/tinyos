#include <tinyos/kernel/module.h>
#include <tinyos/kernel/panic.h>

static const struct module *modules[64];
static size_t module_idx = 0;

void register_module(const struct module *m) {
  assert(module_idx < ARRAY_SIZE(modules));
  modules[module_idx++] = m;
}

void init_modules(void) {
  /* TODO: sort modules by their priority */
  for (size_t i = 0; i < module_idx; i++) {
    const struct module *m = modules[i];
    m->init();
  }
}
