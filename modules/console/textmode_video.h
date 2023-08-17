#pragma once

#include <tinyos/common/common.h>

void textmode_init(u8 x, u8 y);

void textmode_putc(u8 ch);

void textmode_clear_screen(void);
