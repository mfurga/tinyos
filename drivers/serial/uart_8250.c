/*
  Based on:
  https://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming
*/

#include <lib/common.h>
#include <lib/x86.h>

#define THR     0  /* [ W] Transmitter Holding Buffer */
#define RBR     0  /* [R ] Receiver Buffer */
#define DLL     0  /* [RW] (DLAB) Divisor Latch Low Byte */
#define IER     1  /* [RW] Interrupt Enable Register */
#define DLH     1  /* [RW] (DLAB) Divisor Latch High Byte */
#define IIR     2  /* [R ] Interrupt Identification Register */
#define FCR     2  /* [ W] FIFO Control Register */
#define LCR     3  /* [RW] Line Control Register */
#define MCR     4  /* [RW] Modem Control Register */
#define LSR     5  /* [R ] Line Status Register */
#define MSR     6  /* [R ] Modem Status Register */
#define SR      7  /* [RW] Scratch Register */


/* Line Status Register (LSR) */
#define LSR_DATA_READY           0b00000001
#define LSR_OVERRUN_ERROR        0b00000010
#define LSR_PARITY_ERROR         0b00000100
#define LSR_FRAMING_ERROR        0b00001000
#define LSR_BREAK_INTERRUPT      0b00010000
#define LSR_EMPTY_TH_REG         0b00100000
#define LSR_EMPTY_DH_REG         0b01000000
#define LSR_RECV_FIFO_ERROR      0b10000000

/* Line Control Register (LCR) */
#define LCR_5_BITS               0b00000000
#define LCR_6_BITS               0b00000001
#define LCR_7_BITS               0b00000010
#define LCR_8_BITS               0b00000011

#define LCR_1_STOP_BIT           0b00000000
#define LCR_1_5_OR_2_STOP_BIT    0b00000100

#define LCR_NO_PARITY            0b00000000
#define LCR_ODD_PARITY           0b00001000
#define LCR_EVEN_PARITY          0b00011000
#define LCR_MARK                 0b00101000
#define LCR_SPACE                0b00111000

#define LCR_SET_BREAK_ENABLE     0b01000000
#define LCR_SET_DLAB             0b10000000

/* FIFO Control Register (FCR) */
#define FCR_ENABLE_FIFO          0b00000001
#define FCR_CLEAR_RX_FIFO        0b00000010
#define FCR_CLEAR_TX_FIFO        0b00000100
#define FCR_DMA_MODE             0b00001000
#define FCR_64_BYTES_FIFO        0b00100000
#define FCR_INT_LEVEL_1          0b00000000  /* 1B / 1B */
#define FCR_INT_LEVEL_2          0b01000000  /* 4B / 16B */
#define FCR_INT_LEVEL_3          0b10000000  /* 8B / 32B */
#define FCR_INT_LEVEL_4          0b11000000  /* 14B / 56B */

/* Modem Control Register (MCR) */
#define MCR_DTR                  0b00000001  /* Data Terminal Ready */
#define MCR_RTS                  0b00000010  /* Request to Send */
#define MCR_AUX_OUTPUT_1         0b00000100
#define MCR_AUX_OUTPUT_2         0b00001000  /* Needed to enable interrupts */
#define MCR_LOOPBACK_MODE        0b00010000
#define MCR_AUTOFLOW_CTRL        0b00100000  /* 16750 only */

#define UART_BOUD_RATE_115200    1
#define UART_BOUD_RATE_9600      12

static inline void uart_set_dlab(u16 port, bool value) {
  u8 t = inb(port + LCR);
  t = (t & 0b01111111) | (value << 7);
  outb(port + LCR, t);
}

static inline void uart_set_baud_rate(u16 port, u16 baud_rate) {
  /* Set DLAB */
  uart_set_dlab(port, 1);

  outb(port + DLL, baud_rate & 0xff);
  outb(port + DLH, baud_rate >> 8);

  /* Clear DLAB */
  uart_set_dlab(port, 0);
}

void serial_init_port(u16 port) {
  /* Disable interrupts */
  outb(port + IER, 0);

  /* Baud rate 115200 */
  uart_set_baud_rate(port, UART_BOUD_RATE_115200);

  /* 8N1 */
  outb(port + LCR, LCR_8_BITS |
                   LCR_NO_PARITY |
                   LCR_1_STOP_BIT);

  /* Disable FIFO */
  outb(port + FCR, 0);

  /* DTR + RTS */
  outb(port + MCR, MCR_DTR | MCR_RTS);
}

static inline bool serial_data_ready(u16 port) {
  return !!(inb(port + LSR) & LSR_DATA_READY);
}

u8 serial_read(u16 port) {
  while (!serial_data_ready(port))
    ;
  return inb(port + RBR);
}

static inline bool serial_write_ready(u16 port) {
  return !!(inb(port + LSR) & LSR_EMPTY_TH_REG);
}

void serial_write(u16 port, u8 ch) {
  while (!serial_write_ready(port))
    ;
  outb(port + THR, ch);
}

