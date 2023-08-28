#pragma once

#include <tinyos/common/common.h>

extern bool __use_fb;

struct fb {
  u32 paddr;
  u32 pitch;
  u32 width;
  u32 height;
  u32 bpp;

  /* color positions */
  u32 red_pos;
  u32 red_mask;
  u32 green_pos;
  u32 green_mask;
  u32 blue_pos;
  u32 blue_mask;
};

struct fb_font {
  u32 width;
  u32 width_bytes;
  u32 height;
  u32 glyph_size;
  u8 *glyph_data;
};

u32 fb_width(void);
u32 fb_height(void);
u32 fb_font_width(void);
u32 fb_font_height(void);
u32 fb_color(u8 color);

void fb_draw_pixel(u32 x, u32 y, u32 color);

void fb_draw_line(u32 y, u32 h, u32 color);

void fb_scroll_up(u32 h);

void fb_draw_char(u32 x, u32 y, u16 e);

void register_fb_terminal(void);
