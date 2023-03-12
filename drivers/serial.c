#include <drivers/serial.h>

static inline void serial_set_baud_rate(u16 port, u16 baud_rate) {
  u8 t = inb(port | SERIAL_LCR_REG);

  outb(port + SERIAL_LCR_REG, t | SERIAL_DLAB);   /* Set DLAB */

  outb(port + SERIAL_DIVISOR_LO_REG, baud_rate & 0xff);
  outb(port + SERIAL_DIVISOR_HI_REG, baud_rate >> 8);

  outb(port + SERIAL_LCR_REG, t & ~SERIAL_DLAB);  /* Clear DLAB */
}

static inline void serial_set_format(u16 port, u8 flags) {
  outb(port | SERIAL_LCR_REG, flags);
}

void serial_init(void) {

  /* Disable interrupts */
  outb(SERIAL_COM1_PORT + SERIAL_INTERRUPT_REG, 0);

  /* Baud rate 9600 */
  serial_set_baud_rate(SERIAL_COM1_PORT, SERIAL_BAUD_RATE_9600);

  /* 8N1 */
  serial_set_format(SERIAL_COM1_PORT, SERIAL_DATA_BITS_8 |
                                      SERIAL_PARITY_NONE |
                                      SERIAL_STOP_BIT_1);

  outb(SERIAL_COM1_PORT + SERIAL_FIFO_REG, 0);     /* No FIFO */
  outb(SERIAL_COM1_PORT + SERIAL_MODEM_REG, 0x3);  /* RTS + DSR */
}

static inline u8 serial_data_ready(void) {
  return inb(SERIAL_COM1_PORT + SERIAL_LSR_REG) & 1;
}

u8 serial_read(void) {
  while (serial_data_ready() == 0);
  return inb(SERIAL_COM1_PORT + SERIAL_DATA_REG);
}

static inline u8 serial_buffer_empty(void) {
  return inb(SERIAL_COM1_PORT + SERIAL_LSR_REG) & 0x20;
}

void serial_write(u8 ch) {
  while (serial_buffer_empty() == 0);
  outb(SERIAL_COM1_PORT + SERIAL_DATA_REG, ch);
}

