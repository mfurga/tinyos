#pragma once

#include <tinyos/common/common.h>

#define REGISTER_SERIAL_TERMINAL(i, t)                 \
  static CONSTRUCTOR void __register_terminal(void) {  \
    register_terminal(TERMINAL_SERIAL, (i), (t));      \
  }

#define REGISTER_VIDEO_TERMINAL(i, t)                  \
  static CONSTRUCTOR void __register_terminal(void) {  \
    register_terminal(TERMINAL_VIDEO, (i), (t));       \
  }

enum terminal_type {
  TERMINAL_SERIAL,
  TERMINAL_VIDEO
};

struct vterm {
  u16 rows;
  u16 cols;
  u16 r;  /* current row */
  u16 c;  /* current col */
  u8 bg;  /* background color */

  /* functions */
  void (*write_char_at)(u16 row, u16 col, u16 entry);
  void (*clear_row)(u16 row, u8 bg);
  void (*scroll_up)(u8 bg);
  void (*move_cursor)(u16 row, u16 col);
};

struct sterm {
  /* functions */
  void (*write_char)(char c);
};

struct term {
  bool is_initialized;
  enum terminal_type type;
  void (*init)(struct term *t);
  void (*write)(struct term *t, const char *buf, size_t size, u8 color);

  union {
    struct vterm *vt;
    struct sterm *st;
  };
};

void register_terminal(enum terminal_type type,
                       void (*init)(struct term *t),
                       void *term);

void early_init_serial_terminal(void);
void init_video_terminal(void);

void terminal_write(const char *buf, size_t size, u8 color);

void video_terminal_init(struct vterm *vt, u16 rows, u16 cols);
void video_terminal_write(struct term *t, const char *buf, size_t size, u8 color);
void serial_terminal_write(struct term *t, const char *buf, size_t size, u8 color);
