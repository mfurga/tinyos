#pragma once

#include <tinyos/common/common.h>

#define REGISTER_TERMINAL(t)                           \
  static CONSTRUCTOR void __register_terminal(void) {  \
    register_terminal(t);                              \
  }

enum terminal_type {
  TERMINAL_SERIAL,
  TERMINAL_VIDEO_TEXT,
  TERMINAL_VIDEO_GRAPHICAL
};

struct terminal {
  enum terminal_type type;
  void (*init)(void);
  void (*write)(const char *buf, size_t size);
};

void register_terminal(const struct terminal *term);

void early_init_terminals(void);

void set_curr_video_terminal(const struct terminal *term);

void terminal_write(const char *buf, size_t size);
