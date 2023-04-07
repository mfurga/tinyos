#pragma once

#include <lib/common.h>

#define SERIAL_COM1_PORT         0x3f8
#define SERIAL_COM2_PORT         0x2f8

#define SERIAL_DLAB              0x80

#define SERIAL_BAUD_RATE_115200  1
#define SERIAL_BAUD_RATE_57600   2
#define SERIAL_BAUD_RATE_38400   3
#define SERIAL_BAUD_RATE_19200   6
#define SERIAL_BAUD_RATE_14400   8
#define SERIAL_BAUD_RATE_9600    12

#define SERIAL_DATA_REG          0  /* If DLAB is clear */
#define SERIAL_INTERRUPT_REG     1  /* if DLAB is clear */
#define SERIAL_DIVISOR_LO_REG    0  /* If DLAB is set */
#define SERIAL_DIVISOR_HI_REG    1  /* If DLAB is set */
#define SERIAL_FIFO_REG          2
#define SERIAL_LCR_REG           3
#define SERIAL_MODEM_REG         4
#define SERIAL_LSR_REG           5

#define SERIAL_DATA_BITS_6       1
#define SERIAL_DATA_BITS_7       2
#define SERIAL_DATA_BITS_8       3

#define SERIAL_STOP_BIT_1        (0 << 2)
#define SERIAL_STOP_BIT_2        (1 << 2)

#define SERIAL_PARITY_NONE       (0 << 3)
#define SERIAL_PARITY_ODD        (1 << 3)
#define SERIAL_PARITY_EVEN       (3 << 3)
#define SERIAL_PARITY_MARK       (5 << 3)
#define SERIAL_PARITY_SPACE      (7 << 3)

void serial_init(void);

void serial_write(u8 ch);

u8 serial_read(void);

