#pragma once

#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_PURPLE        5
#define COLOR_BROWN         6
#define COLOR_GRAY          7
#define COLOR_DARK_GRAY     8
#define COLOR_LIGHT_BLUE    9
#define COLOR_LIGHT_GREEN   10
#define COLOR_LIGHT_CYAN    11
#define COLOR_LIGHT_RED     12
#define COLOR_LIGHT_PURPLE  13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

#define VGA_ENTRY_MAKE(ch, col) ((((col) & 0xff) << 8) | \
                                 (((ch) & 0xff)))

#define VGA_ENTRY_CHAR(e) ((e) & 0xff)
#define VGA_ENTRY_FG(e) (((e) >> 8) & 0xf)
#define VGA_ENTRY_BG(e) (((e) >> 12) & 0xf)

#define VGA_MAKE_COLOR(f, b) (((b) & 0xf) << 4 | \
                              ((f) & 0xf))
