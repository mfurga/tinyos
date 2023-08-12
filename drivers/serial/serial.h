#pragma once

#include <lib/common.h>

#define COM1      0x3f8
#define COM2      0x2f8
#define COM3      0x3e8
#define COM4      0x2e8

void early_init_serial(void);

void serial_init_port(u16 port);

void serial_write(u16 port, u8 ch);

u8 serial_read(u16 port);

