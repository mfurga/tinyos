#pragma once

#include <multiboot.h>

void setup_fb_from_multiboot(struct multiboot_info *mbi);

void init_fb_terminal(void);
