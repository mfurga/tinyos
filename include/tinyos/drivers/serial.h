#pragma once

#include <tinyos/common/common.h>

void early_init_serial(void);

void serial_init_port(u16 port);

void serial_write(u16 port, u8 ch);

u8 serial_read(u16 port);

