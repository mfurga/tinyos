#pragma once

#include <tinyos/common/common.h>

#define REGISTER_MODULE(m)                            \
  static CONSTRUCTOR void __register_module(void) {   \
    register_module(m);                               \
  }

struct module {
  const char *name;
  int priority;
  void (*init)(void);
};

void init_modules(void);

void register_module(const struct module *m);
