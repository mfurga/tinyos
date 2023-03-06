#pragma once

#include <kernel/common.h>

void video_init(u8 video_mode, u8 x, u8 y, u8 cols, u8 lines);

void putc(u8 ch);

void puts(const char *s);

void printf(const char *fmt, ...);


