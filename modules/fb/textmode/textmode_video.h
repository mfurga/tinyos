#pragma once

#include <tinyos/common/common.h>
#include <multiboot.h>

struct textmode {
  u32 paddr;
  u16 rows;
  u16 cols;
};

void textmode_write_char_at(u16 row, u16 col, u16 entry);

void textmode_clear_screen(void);

void textmode_move_cursor(u16 row, u16 col);

void textmode_clear_row(u16 row, u8 bg);

void textmode_scroll_up(u8 bg);

u16 textmode_cols();

u16 textmode_rows();

void setup_textmode_from_multiboot(struct multiboot_info *mbi);

void register_textmode_terminal(void);
