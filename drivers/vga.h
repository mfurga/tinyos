#pragma once

#include <lib/common.h>

void vga_init(u8 video_mode, u8 x, u8 y, u8 cols, u8 lines);

void vga_putc(u8 ch);

